#pragma once
#include "Voice.h"
#include <array>
#include <atomic>

namespace drift {

// Manages a pool of polyphonic voices with click-free voice stealing.
// All operations are real-time-safe (no allocation, no locks).
class VoiceManager {
public:
    void prepare(double sampleRate) noexcept;
    void reset() noexcept;

    // Set maximum simultaneous voices (1..kMaxVoices)
    void setMaxVoices(int n) noexcept;

    // MIDI note events
    void noteOn(int midiNote, float velocity) noexcept;
    void noteOff(int midiNote) noexcept;
    void setSustainPedal(bool on) noexcept;
    void allNotesOff() noexcept;
    void allSoundOff() noexcept;

    // Controllers (applied to all active voices)
    void setPitchBend(float semitones) noexcept;
    void setModWheel(float value) noexcept;
    void setAftertouch(float value) noexcept;

    // Render one stereo sample. Caller must use ScopedNoDenormals.
    DRIFT_HOT void processSample(float& outL, float& outR) noexcept;

    // Live access to patch params — written from audio thread only.
    PatchParams& getPatch() noexcept { return mPatch; }
    const PatchParams& getPatch() const noexcept { return mPatch; }

private:
    // Returns index of a free voice, or the best voice to steal.
    [[nodiscard]] int findFreeVoice() noexcept;
    [[nodiscard]] int findVoiceForNote(int midiNote) noexcept;

    std::array<Voice, kMaxVoices> mVoices{};
    PatchParams mPatch{};
    int mMaxVoices = 8;

    // Sustain pedal (CC64)
    bool mSustainPedal = false;
    std::array<bool, 128> mHeldByPedal{};
};

} // namespace drift
