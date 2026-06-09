#pragma once
#include "../Constants.h"
#include <array>

namespace drift {

// BBD-style stereo chorus with two LFO-modulated delay taps per channel.
// No dynamic allocation — circular buffer is statically sized.
class Chorus {
public:
    void prepare(double sampleRate) noexcept;
    void reset() noexcept;

    // rate:  [0.1, 5] Hz    – LFO frequency
    // depth: [0, 1]         – modulation depth (maps to delay variation)
    // mix:   [0, 1]         – wet/dry
    void setParameters(float rate, float depth, float mix) noexcept;
    void setBypassed(bool b) noexcept { mBypassed = b; }

    void processStereo(float& L, float& R) noexcept;

private:
    static constexpr float kBaseDelayMs  = 12.0f;
    static constexpr float kMaxDepthMs   = 8.0f;

    [[nodiscard]] float readInterp(const float* buf, int size, float readPos) const noexcept;

    std::array<float, kChorusDelayMax> mBufL{}, mBufR{};
    int   mWritePos = 0;

    float mSampleRate = 44100.0f;
    float mBaseDelay  = 0.0f;    // in samples
    float mMaxDepth   = 0.0f;    // in samples

    // Two LFO phases for L and R (90° apart for pseudo-stereo)
    float mLFOPhaseL  = 0.0f;
    float mLFOPhaseR  = kHalfPi / kTwoPi;  // 90° offset in normalised phase
    float mLFOInc     = 0.0f;

    float mDepth  = 0.0f;
    float mMix    = 0.0f;
    bool  mBypassed = false;
};

} // namespace drift
