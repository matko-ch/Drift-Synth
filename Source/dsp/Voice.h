#pragma once
#include "Oscillator.h"
#include "Filter.h"
#include "Envelope.h"
#include "LFO.h"
#include "ModMatrix.h"
#include <array>

namespace drift {

// Shared patch parameters — owned by VoiceManager, read by Voice each sample.
// All floats; no dynamic allocation.
struct PatchParams {
    // ── Oscillators ───────────────────────────────────────────────────────────
    OscShape osc1Shape   = OscShape::Saw;
    float    osc1Detune  = 0.0f;  // semitones offset
    float    osc1Fine    = 0.0f;  // cents
    float    osc1Level   = 1.0f;
    float    osc1PW      = 0.5f;
    float    osc1Pan     = 0.0f;
    int      osc1Octave  = 0;     // -2..+2

    OscShape osc2Shape   = OscShape::Saw;
    float    osc2Detune  = 0.0f;
    float    osc2Fine    = 0.0f;
    float    osc2Level   = 1.0f;
    float    osc2PW      = 0.5f;
    float    osc2Pan     = 0.0f;
    int      osc2Octave  = 0;
    bool     osc2Sync    = false;

    float    oscMix      = 0.5f;   // 0=all Osc1, 1=all Osc2

    // ── Oscillator extras ──────────────────────────────────────────────────────
    float    oscFM       = 0.0f;   // osc2 → osc1 frequency modulation index
    float    oscRing     = 0.0f;   // ring-mod blend
    float    noiseLevel  = 0.0f;   // white-noise layer
    float    subLevel    = 0.0f;
    OscShape subShape    = OscShape::Square;
    int      subOctave   = -1;     // -2 or -1

    // ── Unison ────────────────────────────────────────────────────────────────
    int   unisonVoices  = 1;       // 1..kMaxUnisonVoices
    float unisonDetune  = 0.0f;    // semitones peak spread ÷ voice pairs
    float unisonSpread  = 0.5f;    // stereo spread [0,1]
    float unisonBlend   = 1.0f;    // unison dry/wet [0,1]

    // ── Filter 1 ──────────────────────────────────────────────────────────────
    float      filter1Cutoff   = 8000.0f;
    float      filter1Res      = 0.0f;
    float      filter1Drive    = 0.0f;
    FilterType filter1Type     = FilterType::LowPass12;
    float      filter1EnvAmt   = 0.0f;   // bipolar [-1,1]
    float      filter1KeyTrack = 0.0f;   // 0=off, 1=1:1 key tracking
    float      filter1VelAmt   = 0.0f;

    // ── Filter 2 ──────────────────────────────────────────────────────────────
    float      filter2Cutoff   = 8000.0f;
    float      filter2Res      = 0.0f;
    float      filter2Drive    = 0.0f;
    FilterType filter2Type     = FilterType::LowPass12;
    float      filter2EnvAmt   = 0.0f;
    float      filter2KeyTrack = 0.0f;
    float      filter2VelAmt   = 0.0f;
    bool       filterParallel  = false;  // false=serial, true=parallel

    // ── Envelopes ─────────────────────────────────────────────────────────────
    Envelope::Params ampEnv;
    Envelope::Params filterEnv;
    Envelope::Params modEnv;

    // ── LFOs ─────────────────────────────────────────────────────────────────
    LFOShape lfo1Shape   = LFOShape::Sine;
    float    lfo1Rate    = 1.0f;
    float    lfo1Depth   = 0.0f;
    bool     lfo1Retrig  = true;

    LFOShape lfo2Shape   = LFOShape::Sine;
    float    lfo2Rate    = 0.5f;
    float    lfo2Depth   = 0.0f;
    bool     lfo2Retrig  = false;

    // ── Mod matrix ────────────────────────────────────────────────────────────
    std::array<ModSlot, kNumModSlots> modSlots{};

    // ── Character ─────────────────────────────────────────────────────────────
    float driftAmount    = 0.35f;  // analog pitch wander [0,1] — the "Drift" soul

    // ── Macro modulation (driven by Orbit/Vibe "movement") ────────────────────
    float macroMotionCutoff = 0.0f; // LFO1 → filter cutoff, in octaves
    float macroVibrato      = 0.0f; // LFO1 → pitch, in semitones

    // ── Master ────────────────────────────────────────────────────────────────
    float masterPitch    = 0.0f;   // semitones (global transpose)
    float glideTime      = 0.0f;   // seconds; 0=off
};

// ── Voice ─────────────────────────────────────────────────────────────────────

class Voice {
public:
    void prepare(double sampleRate) noexcept;
    void reset() noexcept;

    // Start a new note. Steals if currently active (smooth fade handled by VoiceManager).
    void noteOn(int midiNote, float velocity, const PatchParams& patch) noexcept;
    void noteOff() noexcept;
    void panic() noexcept;  // Immediate silence

    // Render one stereo sample pair. Returns {left, right}.
    [[nodiscard]] DRIFT_HOT
    std::pair<float, float> process(const PatchParams& patch) noexcept;

    [[nodiscard]] bool isIdle()      const noexcept;
    [[nodiscard]] bool isReleasing() const noexcept;
    [[nodiscard]] int  getMidiNote() const noexcept { return mMidiNote; }
    [[nodiscard]] float getAge()     const noexcept { return mAgeSamples; }

    // For velocity-based voice stealing
    [[nodiscard]] float getAmplitude() const noexcept { return mAmpEnv.getValue(); }

    // Pitch bend: ±semitones
    void setPitchBend(float semitones) noexcept { mPitchBendSemitones = semitones; }
    void setModWheel(float v) noexcept { mModWheel = v; }
    void setAftertouch(float v) noexcept { mAftertouch = v; }

private:
    double mSampleRate = 44100.0;
    int    mMidiNote   = 60;
    float  mVelocity   = 1.0f;
    float  mAgeSamples = 0.0f;

    // Per-unison-voice oscillators (pairs for stereo spread)
    std::array<Oscillator, kMaxUnisonVoices> mOsc1, mOsc2;
    Oscillator mSub;   // monophonic sub-oscillator
    // Per-unison-voice analog drift state (slow bounded random walk, [-1,1])
    std::array<float, kMaxUnisonVoices> mDrift1{}, mDrift2{};
    XorShift32 mRng;
    // Separate L/R filter instances so each channel has independent state
    Filter  mFilter1L, mFilter1R;
    Filter  mFilter2L, mFilter2R;
    Envelope mAmpEnv, mFilterEnv, mModEnv;
    LFO     mLFO1, mLFO2;
    ModMatrix mModMatrix;

    // Controllers
    float mPitchBendSemitones = 0.0f;
    float mModWheel           = 0.0f;
    float mAftertouch         = 0.0f;

    // Glide
    float mCurrentNote = 60.0f;  // smoothed MIDI note
    float mGlideCoeff  = 0.0f;

    // Soft-kill for voice-stealing transitions
    float mKillGain    = 1.0f;
    float mKillCoeff   = 0.0f;
    bool  mKilling     = false;

    // Compute pitch for a unison voice index
    [[nodiscard]] float computePhaseInc(float notePitch, int unisonIdx,
                                         int totalUnison, float detuneSemitones,
                                         float baseHz) const noexcept;
};

} // namespace drift
