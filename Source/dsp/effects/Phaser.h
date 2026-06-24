#pragma once
#include "../Constants.h"
#include <array>

namespace drift {

// Classic 6-stage stereo phaser: cascaded first-order all-pass filters whose
// break frequency is swept by an LFO, with mild feedback. Unconditionally
// stable (all-pass magnitude is unity; feedback kept < 1).
class Phaser {
public:
    void prepare(double sampleRate) noexcept;
    void reset() noexcept;

    // rate: [0.05, 8] Hz · depth: [0,1] · mix: [0,1]
    void setParameters(float rateHz, float depth, float mix) noexcept;
    void setBypassed(bool b) noexcept { mBypassed = b; }

    void processStereo(float& L, float& R) noexcept;

private:
    static constexpr int kStages = 6;

    float mSampleRate = 44100.0f;
    float mLfoPhase = 0.0f, mLfoInc = 0.0f;
    float mDepth = 0.5f, mMix = 0.5f;
    float mFeedback = 0.5f;
    bool  mBypassed = false;

    std::array<float, kStages> mApxL{}, mApyL{}, mApxR{}, mApyR{};
    float mFbL = 0.0f, mFbR = 0.0f;
};

} // namespace drift
