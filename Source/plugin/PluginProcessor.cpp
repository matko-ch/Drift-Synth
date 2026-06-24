#include "PluginProcessor.h"
#include "../ui/PluginEditor.h"
#include "../dsp/MacroEngine.h"
#include "Presets.h"
#include <cmath>

namespace drift {

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
          .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , apvts(*this, nullptr, "DriftState", createParameterLayout())
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int maxBlockSize) {
    juce::ScopedNoDenormals noDenormals;

    mVoiceManager.prepare(sampleRate);
    mArp.prepare(sampleRate);
    mArp.onNoteOn  = [this](int n, float v) { mVoiceManager.noteOn(n, v); };
    mArp.onNoteOff = [this](int n)          { mVoiceManager.noteOff(n); };
    mDrive.prepare(sampleRate);
    mCrush.prepare(sampleRate);
    mPhaser.prepare(sampleRate);
    mChorus.prepare(sampleRate);
    mDelay.prepare(sampleRate);
    mReverb.prepare(sampleRate);
    mEQ.prepare(sampleRate);

    mMasterVol.reset(sampleRate, kParamSmoothSec);
    mMasterVol.setCurrentAndTargetValue(
        *apvts.getRawParameterValue(ParamID::MASTER_VOL));

    (void)maxBlockSize;
}

// ── Presets / programs ────────────────────────────────────────────────────────

int PluginProcessor::getNumPrograms() {
    return static_cast<int>(getFactoryPresets().size());
}

int PluginProcessor::getCurrentProgram() {
    return juce::jlimit(0, getNumPrograms() - 1, currentPreset);
}

void PluginProcessor::setCurrentProgram(int index) {
    loadFactoryPreset(index);
}

const juce::String PluginProcessor::getProgramName(int index) {
    const auto& bank = getFactoryPresets();
    if (index >= 0 && index < static_cast<int>(bank.size()))
        return bank[static_cast<size_t>(index)].name;
    return {};
}

void PluginProcessor::loadFactoryPreset(int index) {
    const auto& bank = getFactoryPresets();
    if (index < 0 || index >= static_cast<int>(bank.size())) return;
    applyPreset(*this, apvts, bank[static_cast<size_t>(index)]);
    currentPreset = index;
    updateHostDisplay();
}

// ── MIDI handling ─────────────────────────────────────────────────────────────

// Route a note through the chord expander then to either the arp or the voices.
void PluginProcessor::routeNoteOn(int note, float vel) noexcept {
    if (mArpOn) mArp.noteOn(note, vel);
    else        mVoiceManager.noteOn(note, vel);
}
void PluginProcessor::routeNoteOff(int note) noexcept {
    if (mArpOn) mArp.noteOff(note);
    else        mVoiceManager.noteOff(note);
}

void PluginProcessor::handleMidi(const juce::MidiMessage& msg) noexcept {
    if (msg.isNoteOn() && msg.getVelocity() > 0) {
        const int root = msg.getNoteNumber();
        const float vel = msg.getFloatVelocity();
        if (mChordOn) {
            const auto sh = chordShape(mChordType);
            for (int i = 0; i < sh.count; ++i) {
                const int n = root + sh.iv[i];
                if (n >= 0 && n <= 127) routeNoteOn(n, vel);
            }
        } else {
            routeNoteOn(root, vel);
        }
    } else if (msg.isNoteOff() || (msg.isNoteOn() && msg.getVelocity() == 0)) {
        const int root = msg.getNoteNumber();
        if (mChordOn) {
            const auto sh = chordShape(mChordType);
            for (int i = 0; i < sh.count; ++i) {
                const int n = root + sh.iv[i];
                if (n >= 0 && n <= 127) routeNoteOff(n);
            }
        } else {
            routeNoteOff(root);
        }
    } else if (msg.isPitchWheel()) {
        const float bend = (msg.getPitchWheelValue() - 8192) / 8192.0f;
        mVoiceManager.setPitchBend(bend * mPitchBendRange);
    } else if (msg.isController()) {
        const int cc  = msg.getControllerNumber();
        const float v = msg.getControllerValue() / 127.0f;
        if (cc == 1)  mVoiceManager.setModWheel(v);
        if (cc == 64) mVoiceManager.setSustainPedal(v > 0.5f);
        if (cc == 120 || cc == 123) mVoiceManager.allNotesOff();
        if (cc == 121) { mVoiceManager.allSoundOff(); mVoiceManager.reset(); }
    } else if (msg.isAftertouch()) {
        mVoiceManager.setAftertouch(msg.getAfterTouchValue() / 127.0f);
    } else if (msg.isAllNotesOff()) {
        mVoiceManager.allNotesOff();
    } else if (msg.isAllSoundOff()) {
        mVoiceManager.allSoundOff();
    }
}

// ── Sync PatchParams from APVTS ───────────────────────────────────────────────

void PluginProcessor::syncPatchFromParams() noexcept {
    using namespace ParamID;
    auto& p = mVoiceManager.getPatch();
    auto* vt = &apvts;

    // Helper lambdas — load from atomic so return type is plain float
    auto f = [&](const char* id) { return vt->getRawParameterValue(id)->load(std::memory_order_relaxed); };
    auto b = [&](const char* id) { return *vt->getRawParameterValue(id) > 0.5f; };
    auto i = [&](const char* id) { return static_cast<int>(*vt->getRawParameterValue(id)); };

    // Osc 1
    p.osc1Shape   = static_cast<OscShape>(i(OSC1_SHAPE));
    p.osc1Octave  = i(OSC1_OCTAVE);
    p.osc1Detune  = static_cast<float>(i(OSC1_SEMITONE));
    p.osc1Fine    = f(OSC1_FINE);
    p.osc1Level   = f(OSC1_LEVEL);
    p.osc1PW      = f(OSC1_PW);
    p.osc1Pan     = f(OSC1_PAN);

    // Osc 2
    p.osc2Shape   = static_cast<OscShape>(i(OSC2_SHAPE));
    p.osc2Octave  = i(OSC2_OCTAVE);
    p.osc2Detune  = static_cast<float>(i(OSC2_SEMITONE));
    p.osc2Fine    = f(OSC2_FINE);
    p.osc2Level   = f(OSC2_LEVEL);
    p.osc2PW      = f(OSC2_PW);
    p.osc2Pan     = f(OSC2_PAN);
    p.osc2Sync    = b(OSC2_SYNC);
    p.oscMix      = f(OSC_MIX);

    // Oscillator extras
    p.oscFM       = f(OSC_FM);
    p.oscRing     = f(OSC_RING);
    p.noiseLevel  = f(OSC_NOISE);
    p.subLevel    = f(SUB_LEVEL);
    p.subShape    = static_cast<OscShape>(i(SUB_SHAPE) == 2 ? static_cast<int>(OscShape::Square)
                                          : (i(SUB_SHAPE) == 1 ? static_cast<int>(OscShape::Triangle)
                                                               : static_cast<int>(OscShape::Sine)));
    p.subOctave   = i(SUB_OCTAVE);

    // Unison
    p.unisonVoices = i(UNISON_VOICES);
    p.unisonDetune = f(UNISON_DETUNE);
    p.unisonSpread = f(UNISON_SPREAD);

    // Filter 1
    p.filter1Cutoff   = f(FILT1_CUTOFF);
    p.filter1Res      = f(FILT1_RES);
    p.filter1Drive    = f(FILT1_DRIVE);
    p.filter1Type     = static_cast<FilterType>(i(FILT1_TYPE));
    p.filter1EnvAmt   = f(FILT1_ENV_AMT);
    p.filter1KeyTrack = f(FILT1_KEYTRACK);

    // Filter 2
    p.filter2Cutoff   = f(FILT2_CUTOFF);
    p.filter2Res      = f(FILT2_RES);
    p.filter2Drive    = f(FILT2_DRIVE);
    p.filter2Type     = static_cast<FilterType>(i(FILT2_TYPE));
    p.filter2EnvAmt   = f(FILT2_ENV_AMT);
    p.filter2KeyTrack = f(FILT2_KEYTRACK);
    p.filterParallel  = b(FILT_ROUTING);

    // Envelopes (DAHDSR + shared attack/decay curve)
    p.ampEnv   = { 0.0f, f(AMPENV_A),  f(AMPENV_H),  f(AMPENV_D),  f(AMPENV_S),  f(AMPENV_R),  f(AMPENV_CURVE),  f(AMPENV_CURVE)  };
    p.filterEnv= { 0.0f, f(FILTENV_A), f(FILTENV_H), f(FILTENV_D), f(FILTENV_S), f(FILTENV_R), f(FILTENV_CURVE), f(FILTENV_CURVE) };
    p.modEnv   = { 0.0f, f(MODENV_A),  f(MODENV_H),  f(MODENV_D),  f(MODENV_S),  f(MODENV_R),  f(MODENV_CURVE),  f(MODENV_CURVE)  };

    // LFOs
    p.lfo1Shape  = static_cast<LFOShape>(i(LFO1_SHAPE));
    p.lfo1Rate   = f(LFO1_RATE);
    p.lfo1Depth  = f(LFO1_DEPTH);
    p.lfo1Retrig = b(LFO1_RETRIG);

    p.lfo2Shape  = static_cast<LFOShape>(i(LFO2_SHAPE));
    p.lfo2Rate   = f(LFO2_RATE);
    p.lfo2Depth  = f(LFO2_DEPTH);
    p.lfo2Retrig = b(LFO2_RETRIG);

    // Mod matrix
    for (int slot = 0; slot < kNumModSlots; ++slot) {
        ModSlot ms;
        ms.source = static_cast<ModSource>(i(MOD_SRC[slot]));
        ms.dest   = static_cast<ModDest>(i(MOD_DST[slot]));
        ms.amount = f(MOD_AMT[slot]);
        p.modSlots[slot] = ms;
    }

    // Character
    p.driftAmount = f(DRIFT_AMOUNT);

    // Master
    p.masterPitch = f(MASTER_TUNE) * (1.0f / 100.0f); // cents → semitones
    // Glide is active from the Vibe Glide fader (time > 0), or forced by the
    // Advanced Glide toggle (which applies a sensible default time).
    p.glideTime   = std::max(f(GLIDE_TIME), b(GLIDE_ON) ? 0.05f : 0.0f);

    mPitchBendRange = f(PITCH_BEND_RANGE);
    mVoiceManager.setMaxVoices(i(POLY_VOICES));
}

MacroParams PluginProcessor::readMacroParams() const noexcept {
    using namespace ParamID;
    auto f = [&](const char* id) {
        return apvts.getRawParameterValue(id)->load(std::memory_order_relaxed);
    };
    MacroParams m;
    m.morphX       = f(MORPH_X);       m.morphY       = f(MORPH_Y);
    m.orbitMotion  = f(ORBIT_MOTION);  m.orbitSpace   = f(ORBIT_SPACE);
    m.orbitTexture = f(ORBIT_TEXTURE); m.orbitDrive   = f(ORBIT_DRIVE);
    m.orbitTone    = f(ORBIT_TONE);
    m.vibeDream    = f(VIBE_DREAM);    m.vibeWarm     = f(VIBE_WARM);
    m.vibeAir      = f(VIBE_AIR);      m.vibeMove     = f(VIBE_MOVE);
    m.vibeGrit     = f(VIBE_GRIT);     m.vibeWide     = f(VIBE_WIDE);
    m.vibeFocus    = f(VIBE_FOCUS);
    return m;
}

// ── processBlock ──────────────────────────────────────────────────────────────

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // Silence input channels we don't use
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear(ch, 0, buffer.getNumSamples());

    const int numSamples = buffer.getNumSamples();
    float* outL = buffer.getWritePointer(0);
    float* outR = buffer.getWritePointer(1 < buffer.getNumChannels() ? 1 : 0);

    // Merge on-screen keyboard events into the incoming MIDI stream.
    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);

    // Sync patch once per block (cheap — just atomic reads)
    syncPatchFromParams();

    // Apply the creative-control (macro) layer on top of the base patch.
    const MacroResult macro = MacroEngine::apply(mVoiceManager.getPatch(),
                                                 readMacroParams());
    const float stereoWidth = macro.stereoWidth;

    mMasterVol.setTargetValue(*apvts.getRawParameterValue(ParamID::MASTER_VOL));

    auto pf = [&](const char* id) {
        return apvts.getRawParameterValue(id)->load(std::memory_order_relaxed);
    };
    auto pOn = [&](const char* id) { return *apvts.getRawParameterValue(id) > 0.5f; };

    // Effects parameters (per-block, not per-sample — acceptable for FX).
    // Macro deltas are added on top of the user values and can force-enable a slot.
    mDrive.setParameters(clamp(pf(ParamID::DRIVE_AMOUNT) + macro.driveAmountAdd, 0.0f, 1.0f),
                          pf(ParamID::DRIVE_MIX));
    mDrive.setBypassed(!(pOn(ParamID::DRIVE_ON) || macro.forceDriveOn));

    mChorus.setParameters(pf(ParamID::CHORUS_RATE),
                           clamp(pf(ParamID::CHORUS_DEPTH) + macro.chorusDepthAdd, 0.0f, 1.0f),
                           clamp(pf(ParamID::CHORUS_MIX)   + macro.chorusMixAdd,   0.0f, 1.0f));
    mChorus.setBypassed(!(pOn(ParamID::CHORUS_ON) || macro.forceChorusOn));

    mDelay.setParameters(pf(ParamID::DELAY_TIME),
                          pf(ParamID::DELAY_FDBK),
                          clamp(pf(ParamID::DELAY_MIX) + macro.delayMixAdd, 0.0f, 1.0f));
    mDelay.setBypassed(!(pOn(ParamID::DELAY_ON) || macro.forceDelayOn));

    mReverb.setParameters(clamp(pf(ParamID::REVERB_SIZE) + macro.reverbSizeAdd, 0.0f, 1.0f),
                           pf(ParamID::REVERB_DAMP),
                           clamp(pf(ParamID::REVERB_MIX) + macro.reverbMixAdd, 0.0f, 1.0f));
    mReverb.setBypassed(!(pOn(ParamID::REVERB_ON) || macro.forceReverbOn));

    mCrush.setParameters(pf(ParamID::CRUSH_BITS), pf(ParamID::CRUSH_AMT), pf(ParamID::CRUSH_MIX));
    mCrush.setBypassed(!pOn(ParamID::CRUSH_ON));

    mPhaser.setParameters(pf(ParamID::PHASER_RATE), pf(ParamID::PHASER_DEPTH), pf(ParamID::PHASER_MIX));
    mPhaser.setBypassed(!pOn(ParamID::PHASER_ON));

    mEQ.setParameters(pf(ParamID::EQ_LOW), pf(ParamID::EQ_MID), pf(ParamID::EQ_HIGH));
    mEQ.setBypassed(!pOn(ParamID::EQ_ON));

    // ── Arpeggiator / chord mode ────────────────────────────────────────────────
    double bpm = 120.0;
    if (auto* ph = getPlayHead())
        if (auto pos = ph->getPosition())
            if (auto b = pos->getBpm()) bpm = *b;

    const bool      newArpOn   = pOn(ParamID::ARP_ON);
    const bool      newChordOn = pOn(ParamID::CHORD_ON);
    const auto      newChord   = static_cast<ChordType>(static_cast<int>(pf(ParamID::CHORD_TYPE)));
    // A change in performance mode could otherwise strand held notes — flush first.
    if (newArpOn != mArpOn || newChordOn != mChordOn || newChord != mChordType) {
        mArp.clearHeld();
        mVoiceManager.allNotesOff();
    }
    mArpOn = newArpOn; mChordOn = newChordOn; mChordType = newChord;

    static constexpr double kArpBeats[8] = { 2.0, 1.0, 2.0/3.0, 0.5, 1.0/3.0, 0.25, 1.0/6.0, 0.125 };
    const int rateIdx = juce::jlimit(0, 7, static_cast<int>(pf(ParamID::ARP_RATE)));
    mArp.setStepSamples(kArpBeats[rateIdx] * (60.0 / bpm) * getSampleRate());
    mArp.setMode(static_cast<int>(pf(ParamID::ARP_MODE)));
    mArp.setOctaves(static_cast<int>(pf(ParamID::ARP_OCT)));
    mArp.setGate(pf(ParamID::ARP_GATE));

    // Process MIDI events and audio sample-by-sample for event accuracy
    auto midiIt = midiMessages.findNextSamplePosition(0);
    int pos = 0;
    bool blewUp = false;

    while (pos < numSamples) {
        // Process MIDI messages up to current sample position
        while (midiIt != midiMessages.cend() && (*midiIt).samplePosition <= pos) {
            handleMidi((*midiIt).getMessage());
            ++midiIt;
        }

        // Advance the arpeggiator clock (generates internal note events).
        if (mArpOn) mArp.tick();

        float l = 0.0f, r = 0.0f;
        mVoiceManager.processSample(l, r);

        // Master volume (smoothed)
        const float vol = mMasterVol.getNextValue();
        l *= vol;
        r *= vol;

        // Effects chain: tone/grit → modulation → time → space → final EQ
        mDrive.processStereo(l, r);
        mCrush.processStereo(l, r);
        mPhaser.processStereo(l, r);
        mChorus.processStereo(l, r);
        mDelay.processStereo(l, r);
        mReverb.processStereo(l, r);
        mEQ.processStereo(l, r);

        // Macro stereo width (mid/side)
        if (stereoWidth != 1.0f) {
            const float mid  = (l + r) * 0.5f;
            const float side = (l - r) * 0.5f * stereoWidth;
            l = mid + side;
            r = mid - side;
        }

        // Gentle console-style saturation: transparent at low level, smoothly
        // tames peaks so dense unison/poly stacks never clip harshly.
        l = softSaturate(l);
        r = softSaturate(r);

        // Final safety net: a non-finite sample must never reach the output or
        // recirculate. If one ever appears, silence it and flag a full flush.
        if (!std::isfinite(l) || !std::isfinite(r)) { l = 0.0f; r = 0.0f; blewUp = true; }

        outL[pos] = l;
        outR[pos] = r;
        ++pos;
    }

    // Drain any remaining MIDI at the end of the block
    while (midiIt != midiMessages.cend()) {
        handleMidi((*midiIt).getMessage());
        ++midiIt;
    }

    // Guaranteed recovery: if anything blew up this block, flush every stateful
    // unit (voices + all effect buffers) so the plugin heals itself instantly —
    // never a stuck howl that needs a restart.
    if (blewUp) {
        mArp.clearHeld();
        mVoiceManager.reset();
        mDrive.reset();
        mChorus.reset();
        mDelay.reset();
        mReverb.reset();
        buffer.clear();
    }

    // Update peak meters (lock-free)
    float peakL = 0.0f, peakR = 0.0f;
    for (int i = 0; i < numSamples; ++i) {
        peakL = std::max(peakL, std::abs(outL[i]));
        peakR = std::max(peakR, std::abs(outR[i]));
    }
    meterL.store(peakL, std::memory_order_relaxed);
    meterR.store(peakR, std::memory_order_relaxed);
}

// ── State persistence ─────────────────────────────────────────────────────────

void PluginProcessor::getStateInformation(juce::MemoryBlock& data) {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, data);
}

void PluginProcessor::setStateInformation(const void* data, int size) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size));
    if (xml && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

// ── Editor ────────────────────────────────────────────────────────────────────

juce::AudioProcessorEditor* PluginProcessor::createEditor() {
    return new PluginEditor(*this);
}

} // namespace drift

// Required factory function
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new drift::PluginProcessor();
}
