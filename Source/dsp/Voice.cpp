#include "Voice.h"
#include <cmath>
#include <algorithm>

namespace drift {

void Voice::prepare(double sampleRate) noexcept {
    mSampleRate = sampleRate;

    for (auto& o : mOsc1) o.prepare(sampleRate);
    for (auto& o : mOsc2) o.prepare(sampleRate);
    mSub.prepare(sampleRate);

    mFilter1L.prepare(sampleRate); mFilter1R.prepare(sampleRate);
    mFilter2L.prepare(sampleRate); mFilter2R.prepare(sampleRate);

    mAmpEnv.prepare(sampleRate);
    mFilterEnv.prepare(sampleRate);
    mModEnv.prepare(sampleRate);

    mLFO1.prepare(sampleRate);
    mLFO2.prepare(sampleRate);

    // Kill ramp: 10 ms
    mKillCoeff = std::exp(-1.0f / static_cast<float>(sampleRate * 0.010));

    // Seed the per-voice RNG from the instance address so every voice drifts
    // independently (no shared mutable state on the audio thread).
    mRng.seed(static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(this)) | 1u);

    reset();
}

void Voice::reset() noexcept {
    for (auto& o : mOsc1) o.reset();
    for (auto& o : mOsc2) o.reset();
    mSub.reset();
    mFilter1L.reset(); mFilter1R.reset();
    mFilter2L.reset(); mFilter2R.reset();
    mAmpEnv.reset();
    mFilterEnv.reset();
    mModEnv.reset();
    mLFO1.reset();
    mLFO2.reset();
    mKillGain  = 1.0f;
    mKilling   = false;
    mAgeSamples = 0.0f;
    mCurrentNote = static_cast<float>(mMidiNote);
    mDrift1.fill(0.0f);
    mDrift2.fill(0.0f);
}

void Voice::panic() noexcept {
    mAmpEnv.reset();
    mFilterEnv.reset();
    mModEnv.reset();
    mKillGain = 0.0f;
    mKilling  = false;
}

void Voice::noteOn(int midiNote, float velocity, const PatchParams& patch) noexcept {
    // Retrigger or new start
    const bool wasActive = !mAmpEnv.isIdle();

    mMidiNote  = midiNote;
    mVelocity  = velocity;
    mAgeSamples = 0.0f;

    // If glide is off or this is a fresh start, snap pitch
    if (patch.glideTime <= 0.0f || !wasActive)
        mCurrentNote = static_cast<float>(midiNote);

    // Glide coefficient: one-pole smoother
    if (patch.glideTime > 0.0f)
        mGlideCoeff = std::exp(-1.0f / static_cast<float>(mSampleRate * patch.glideTime));
    else
        mGlideCoeff = 0.0f;

    // Configure envelopes
    mAmpEnv.setParams(patch.ampEnv);
    mFilterEnv.setParams(patch.filterEnv);
    mModEnv.setParams(patch.modEnv);

    mAmpEnv.noteOn(velocity);
    mFilterEnv.noteOn(velocity);
    mModEnv.noteOn(velocity);

    // LFOs
    mLFO1.setShape(patch.lfo1Shape);
    mLFO1.setRate(patch.lfo1Rate);
    mLFO1.setRetrigger(patch.lfo1Retrig);
    mLFO1.noteOn();

    mLFO2.setShape(patch.lfo2Shape);
    mLFO2.setRate(patch.lfo2Rate);
    mLFO2.setRetrigger(patch.lfo2Retrig);
    mLFO2.noteOn();

    // Random phase for oscillators (prevent unison phase stacking)
    for (int i = 0; i < kMaxUnisonVoices; ++i) {
        // Each unison voice gets a random starting phase to avoid comb cancellation
        float ph1 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        float ph2 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        mOsc1[i].reset(ph1);
        mOsc2[i].reset(ph2);
    }

    // Mod matrix
    for (int i = 0; i < kNumModSlots; ++i)
        mModMatrix.setSlot(i, patch.modSlots[i]);

    mKilling = false;
    mKillGain = 1.0f;
}

void Voice::noteOff() noexcept {
    mAmpEnv.noteOff();
    mFilterEnv.noteOff();
    mModEnv.noteOff();
}

bool Voice::isIdle() const noexcept {
    return mAmpEnv.isIdle() && !mKilling;
}

bool Voice::isReleasing() const noexcept {
    return mAmpEnv.isReleasing();
}

float Voice::computePhaseInc(float notePitch, int unisonIdx, int totalUnison,
                              float detuneSemitones, float /*baseHz*/) const noexcept {
    float detuneOffset = 0.0f;
    if (totalUnison > 1) {
        // Spread evenly: [-detune, ..., +detune]
        const float spread = detuneSemitones;
        detuneOffset = (totalUnison == 1)
            ? 0.0f
            : (static_cast<float>(unisonIdx) / static_cast<float>(totalUnison - 1) - 0.5f) * 2.0f * spread;
    }
    const float hz = midiNoteToHz(notePitch + detuneOffset);
    return hz / static_cast<float>(mSampleRate);
}

std::pair<float, float> Voice::process(const PatchParams& patch) noexcept {
    ++mAgeSamples;

    // ── Pitch glide ───────────────────────────────────────────────────────────
    const float targetNote = static_cast<float>(mMidiNote);
    mCurrentNote = targetNote + mGlideCoeff * (mCurrentNote - targetNote);

    // ── Envelopes ─────────────────────────────────────────────────────────────
    const float ampEnvVal    = mAmpEnv.process();
    const float filterEnvVal = mFilterEnv.process();
    const float modEnvVal    = mModEnv.process();

    // ── LFOs ─────────────────────────────────────────────────────────────────
    // Compute the raw bipolar LFO once, then scale: user depth feeds the mod
    // matrix, while the macro layer taps the full-depth signal independently.
    const float lfo1Raw = mLFO1.process(1.0f);
    const float lfo2Raw = mLFO2.process(1.0f);
    const float lfo1Val = lfo1Raw * patch.lfo1Depth;
    const float lfo2Val = lfo2Raw * patch.lfo2Depth;

    // ── Mod matrix ────────────────────────────────────────────────────────────
    const float keyTrackNorm = (mCurrentNote - 60.0f) / 60.0f;  // ±1 at C±5 octaves

    std::array<float, static_cast<int>(ModSource::Count)> srcVals{};
    srcVals[static_cast<int>(ModSource::AmpEnv)]    = ampEnvVal;
    srcVals[static_cast<int>(ModSource::FilterEnv)] = filterEnvVal;
    srcVals[static_cast<int>(ModSource::ModEnv)]    = modEnvVal;
    srcVals[static_cast<int>(ModSource::LFO1)]      = lfo1Val;
    srcVals[static_cast<int>(ModSource::LFO2)]      = lfo2Val;
    srcVals[static_cast<int>(ModSource::Velocity)]  = mVelocity;
    srcVals[static_cast<int>(ModSource::ModWheel)]  = mModWheel;
    srcVals[static_cast<int>(ModSource::KeyTrack)]  = keyTrackNorm;
    srcVals[static_cast<int>(ModSource::Aftertouch)]= mAftertouch;

    const ModValues mod = mModMatrix.process(srcVals);

    // ── Pitch ─────────────────────────────────────────────────────────────────
    const float basePitch = mCurrentNote
        + patch.masterPitch
        + mPitchBendSemitones
        + mod.masterPitch
        + lfo1Raw * patch.macroVibrato;   // macro "movement" vibrato

    const float osc1BasePitch = basePitch
        + static_cast<float>(patch.osc1Octave) * 12.0f
        + patch.osc1Detune
        + patch.osc1Fine * 0.01f
        + mod.osc1Pitch;

    const float osc2BasePitch = basePitch
        + static_cast<float>(patch.osc2Octave) * 12.0f
        + patch.osc2Detune
        + patch.osc2Fine * 0.01f
        + mod.osc2Pitch;

    // ── Oscillator mix ────────────────────────────────────────────────────────
    const float mixRaw   = clamp(patch.oscMix + mod.oscMix, 0.0f, 1.0f);
    const float osc1Gain = patch.osc1Level * (1.0f - mixRaw);
    const float osc2Gain = patch.osc2Level * mixRaw;

    // ── Pulse widths ──────────────────────────────────────────────────────────
    const float pw1 = clamp(patch.osc1PW + mod.osc1PW, 0.05f, 0.95f);
    const float pw2 = clamp(patch.osc2PW + mod.osc2PW, 0.05f, 0.95f);

    // ── Unison summing ────────────────────────────────────────────────────────
    const int nUni = std::clamp(patch.unisonVoices, 1, kMaxUnisonVoices);
    float sumL = 0.0f, sumR = 0.0f;

    // Equal-power normalisation so unison count doesn't change perceived loudness.
    const float uniNorm = 1.0f / std::sqrt(static_cast<float>(nUni));

    // Analog drift: each oscillator wanders slowly in pitch. The soul of "Drift".
    const float driftSemis = patch.driftAmount * kMaxDriftCents * 0.01f;

    for (int ui = 0; ui < nUni; ++ui) {
        // Advance the per-oscillator bounded random walk
        mDrift1[ui] = clamp(mDrift1[ui] * kDriftLeak + mRng.bipolar() * kDriftStep, -1.0f, 1.0f);
        mDrift2[ui] = clamp(mDrift2[ui] * kDriftLeak + mRng.bipolar() * kDriftStep, -1.0f, 1.0f);
        const float d1 = mDrift1[ui] * driftSemis;
        const float d2 = mDrift2[ui] * driftSemis;

        // Unison detune per voice (+ analog drift)
        const float pi1 = computePhaseInc(osc1BasePitch + d1, ui, nUni, patch.unisonDetune, 0.0f);
        const float pi2 = computePhaseInc(osc2BasePitch + d2, ui, nUni, patch.unisonDetune, 0.0f);

        // Osc2 first so it can frequency-modulate Osc1 (FM).
        float o2 = mOsc2[ui].process(pi2, patch.osc2Shape, pw2);

        // FM: scale osc1's phase increment by osc2's output. Clamp well below
        // Nyquist so it can never destabilise.
        float pi1m = pi1;
        if (patch.oscFM > 0.0001f)
            pi1m = clamp(pi1 * (1.0f + o2 * patch.oscFM * 4.0f), 0.0f, 0.49f);

        float o1 = mOsc1[ui].process(pi1m, patch.osc1Shape, pw1);

        // Hard sync: osc2 slave to osc1 (reset on osc1 wrap)
        if (patch.osc2Sync) {
            if (mOsc1[ui].getPhase() < pi1m)
                mOsc2[ui].syncReset(0.0f);
        }

        // Ring modulation blends in the product of the two oscillators.
        const float dry  = o1 * osc1Gain + o2 * osc2Gain;
        const float ring = (o1 * o2) * (osc1Gain + osc2Gain);
        const float sig  = dry * (1.0f - patch.oscRing) + ring * patch.oscRing;

        // Stereo pan per unison voice
        float panPos = 0.0f;
        if (nUni > 1) {
            const float spreadSign = (static_cast<float>(ui) / static_cast<float>(nUni - 1) - 0.5f) * 2.0f;
            panPos = spreadSign * patch.unisonSpread;
        }

        // Constant-power pan (L+R sum preserved)
        const float panAngle = (panPos * 0.5f + 0.5f) * kHalfPi;
        const float panL = std::cos(panAngle);
        const float panR = std::sin(panAngle);

        sumL += sig * panL * uniNorm;
        sumR += sig * panR * uniNorm;
    }

    // ── Sub-oscillator (mono) + noise layer ─────────────────────────────────────
    if (patch.subLevel > 0.0001f) {
        const float subPitch = osc1BasePitch + static_cast<float>(patch.subOctave) * 12.0f;
        const float subPi    = midiNoteToHz(subPitch) / static_cast<float>(mSampleRate);
        const float sub      = mSub.process(clamp(subPi, 0.0f, 0.49f), patch.subShape, 0.5f)
                             * patch.subLevel;
        sumL += sub; sumR += sub;
    }
    if (patch.noiseLevel > 0.0001f) {
        sumL += mRng.bipolar() * patch.noiseLevel;
        sumR += mRng.bipolar() * patch.noiseLevel;
    }

    // Osc panning additive (Osc1 and Osc2 individual pan settings)
    // (already summed into stereo above; could refine per-osc pan here)

    // ── Filter ────────────────────────────────────────────────────────────────
    // Key tracking: cutoff shifts with note (in octaves relative to A4=69)
    const float keyOctaves = (mCurrentNote - 69.0f) / 12.0f;

    // Macro "movement": LFO1 wobble on cutoff, shared by both filters.
    const float macroCutoffMod = lfo1Raw * patch.macroMotionCutoff;

    // Filter 1 cutoff modulation (additive in log/octave domain → multiply)
    const float f1CutoffOctaveMod = mod.filter1Cutoff
        + filterEnvVal * patch.filter1EnvAmt * 5.0f   // ±5 octaves at max
        + keyOctaves   * patch.filter1KeyTrack * 2.0f // ±2 oct key track
        + macroCutoffMod;

    const float f1Cutoff = clamp(
        patch.filter1Cutoff * std::exp2(f1CutoffOctaveMod),
        kMinCutoffHz, static_cast<float>(mSampleRate) * 0.48f);

    const float f1Res  = clamp(patch.filter1Res  + mod.filter1Res,  0.0f, 1.0f);

    // Compute coefficients once on L; copy to R to halve trig cost
    mFilter1L.setParameters(f1Cutoff, f1Res, patch.filter1Drive, patch.filter1Type);
    mFilter1R.copyCoeffsFrom(mFilter1L);

    // Filter 2
    const float f2CutoffOctaveMod = mod.filter2Cutoff
        + filterEnvVal * patch.filter2EnvAmt * 5.0f
        + keyOctaves   * patch.filter2KeyTrack * 2.0f
        + macroCutoffMod;

    const float f2Cutoff = clamp(
        patch.filter2Cutoff * std::exp2(f2CutoffOctaveMod),
        kMinCutoffHz, static_cast<float>(mSampleRate) * 0.48f);

    const float f2Res  = clamp(patch.filter2Res  + mod.filter2Res,  0.0f, 1.0f);

    mFilter2L.setParameters(f2Cutoff, f2Res, patch.filter2Drive, patch.filter2Type);
    mFilter2R.copyCoeffsFrom(mFilter2L);

    // Apply filters — separate L/R instances preserve independent channel state
    float fL, fR;
    if (patch.filterParallel) {
        fL = (mFilter1L.process(sumL) + mFilter2L.process(sumL)) * 0.5f;
        fR = (mFilter1R.process(sumR) + mFilter2R.process(sumR)) * 0.5f;
    } else {
        fL = mFilter2L.process(mFilter1L.process(sumL));
        fR = mFilter2R.process(mFilter1R.process(sumR));
    }

    // ── Amp envelope + volume ─────────────────────────────────────────────────
    const float vol = clamp(ampEnvVal + mod.ampVolume, 0.0f, 1.0f);

    // ── Pan modulation ────────────────────────────────────────────────────────
    const float pan = clamp(mod.pan, -1.0f, 1.0f);
    const float panA = (pan * 0.5f + 0.5f) * kHalfPi;
    const float finalL = fL * vol * std::cos(panA);
    const float finalR = fR * vol * std::sin(panA);

    // ── Kill ramp (voice stealing) ─────────────────────────────────────────────
    if (mKilling) {
        mKillGain *= mKillCoeff;
        if (mKillGain < 1e-4f) {
            mKillGain = 0.0f;
            mKilling  = false;
        }
        return { finalL * mKillGain, finalR * mKillGain };
    }

    return { finalL, finalR };
}

} // namespace drift
