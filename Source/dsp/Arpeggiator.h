#pragma once
#include <array>
#include <functional>

namespace drift {

// A real note-generating arpeggiator: it captures held keys and emits internal
// note-on/note-off events on a tempo-synced clock. Output is routed to the voice
// engine via the onNoteOn / onNoteOff callbacks. RT-safe (no allocation).
class Arpeggiator {
public:
    enum Mode { Up = 0, Down, UpDown, DownUp, AsPlayed, Random, NumModes };

    void prepare(double sampleRate) noexcept;
    void reset() noexcept;          // stop + forget everything (silences current note)

    void setStepSamples(double s) noexcept { mStepSamples = s < 1.0 ? 1.0 : s; }
    void setMode(int m) noexcept { mMode = m; }
    void setOctaves(int o) noexcept;
    void setGate(float g) noexcept { mGate = g < 0.05f ? 0.05f : (g > 1.0f ? 1.0f : g); }

    void noteOn(int note, float vel) noexcept;
    void noteOff(int note) noexcept;
    void clearHeld() noexcept;      // release held keys + stop playing note

    // Advance the clock by one sample; may fire callbacks.
    void tick() noexcept;

    [[nodiscard]] bool hasHeldNotes() const noexcept { return mHeldCount > 0; }

    std::function<void(int, float)> onNoteOn;
    std::function<void(int)>        onNoteOff;

private:
    void rebuildSequence() noexcept;
    void step() noexcept;

    double mSampleRate = 44100.0;
    double mStepSamples = 11025.0;
    double mPhase = 0.0;            // counts up to mStepSamples
    int    mMode = Up;
    int    mOctaves = 1;
    float  mGate = 0.5f;

    // Held keys (insertion-ordered for As-Played), with velocities.
    static constexpr int kMaxHeld = 32;
    std::array<int, kMaxHeld>   mHeld{};
    std::array<float, kMaxHeld> mHeldVel{};
    int mHeldCount = 0;

    // Expanded playback sequence (held × octaves, ordered by mode).
    static constexpr int kMaxSeq = kMaxHeld * 4 * 2;
    std::array<int, kMaxSeq>   mSeq{};
    std::array<float, kMaxSeq> mSeqVel{};
    int mSeqLen = 0;
    int mSeqPos = 0;

    int  mPlayingNote = -1;        // currently sounding arp note (-1 = none)
    bool mGateOpen = false;
    double mGateSamples = 0.0;     // remaining samples until note-off (gate)
    uint32_t mRng = 0x2545F491u;
};

} // namespace drift
