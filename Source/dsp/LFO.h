#pragma once
#include "Constants.h"

namespace drift {

enum class LFOShape : int {
    Sine = 0,
    Triangle,
    Saw,
    Square,
    SampleAndHold,
    Count
};

// Tempo-sync divisions (index into kLFOSyncRatios)
enum class LFOSync : int {
    Free = 0,
    Div1_1, Div1_2, Div1_4, Div1_8, Div1_16,
    Div3_4, Div3_8, Div3_16,
    Count
};

// Bar fractions for each LFOSync value
inline constexpr float kLFOSyncRatios[] = {
    0.0f,    // Free (unused)
    1.0f,    // 1/1
    0.5f,    // 1/2
    0.25f,   // 1/4
    0.125f,  // 1/8
    0.0625f, // 1/16
    0.75f,   // 3/4
    0.375f,  // 3/8
    0.1875f  // 3/16
};

class LFO {
public:
    void prepare(double sampleRate) noexcept;
    void reset() noexcept;

    // Retrigger phase on note-on
    void noteOn() noexcept;

    // Call once per block to update rate (Hz or BPM-derived).
    void setRate(float hz) noexcept;
    void setShape(LFOShape s) noexcept { mShape = s; }

    // depth: [0, 1]. Returns value in [-depth, +depth]
    [[nodiscard]] DRIFT_HOT float process(float depth) noexcept;

    void setRetrigger(bool r) noexcept { mRetrigger = r; }

private:
    float mSampleRate  = 44100.0f;
    float mPhaseInc    = 0.0f;
    float mPhase       = 0.0f;
    LFOShape mShape    = LFOShape::Sine;
    bool  mRetrigger   = true;

    // S&H state
    float mSHOutput    = 0.0f;
    float mSHPrevPhase = 0.0f;
};

} // namespace drift
