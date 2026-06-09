#pragma once
#include "../Constants.h"
#include <array>

namespace drift {

// Ping-pong stereo delay with feedback low-pass filter.
// Statically allocated circular buffer — no heap on audio thread.
class StereoDelay {
public:
    void prepare(double sampleRate) noexcept;
    void reset() noexcept;

    // time:     [0.01, 2.0] seconds (or tempo-derived)
    // feedback: [0, 0.98]
    // mix:      [0, 1]
    void setParameters(float timeSec, float feedback, float mix) noexcept;
    void setBypassed(bool b) noexcept { mBypassed = b; }

    void processStereo(float& L, float& R) noexcept;

private:
    static constexpr int kBufSize = kDelayBufferMax;

    std::array<float, kBufSize> mBufL{}, mBufR{};
    int   mWritePos  = 0;
    int   mDelaySamp = 0;

    float mFeedback  = 0.5f;
    float mMix       = 0.5f;
    bool  mBypassed  = false;

    float mSampleRate = 44100.0f;

    // Feedback low-pass (one-pole, softens repeats)
    float mFBFilterL = 0.0f;
    float mFBFilterR = 0.0f;
    float mFBCoeff   = 0.7f;  // LP coefficient (lower = darker)
};

} // namespace drift
