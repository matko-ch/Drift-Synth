#include "StereoDelay.h"
#include <cmath>
#include <algorithm>

namespace drift {

void StereoDelay::prepare(double sampleRate) noexcept {
    mSampleRate = static_cast<float>(sampleRate);
    reset();
}

void StereoDelay::reset() noexcept {
    mBufL.fill(0.0f);
    mBufR.fill(0.0f);
    mWritePos  = 0;
    mFBFilterL = 0.0f;
    mFBFilterR = 0.0f;
}

void StereoDelay::setParameters(float timeSec, float feedback, float mix) noexcept {
    const int maxSamp = static_cast<int>(mBufL.size()) - 1;
    mDelaySamp = std::clamp(static_cast<int>(timeSec * mSampleRate), 1, maxSamp);
    mFeedback  = std::clamp(feedback, 0.0f, 0.98f);
    mMix       = std::clamp(mix, 0.0f, 1.0f);
}

void StereoDelay::processStereo(float& L, float& R) noexcept {
    if (mBypassed) return;

    const int bufSize = static_cast<int>(mBufL.size());

    // Read delayed samples (ping-pong: L reads from R buffer and vice versa)
    const int readPos = (mWritePos - mDelaySamp + bufSize) % bufSize;
    float delayedL = mBufR[readPos];  // ping-pong swap
    float delayedR = mBufL[readPos];

    // Feedback low-pass filter on the delay path
    mFBFilterL = mFBFilterL + mFBCoeff * (delayedL - mFBFilterL);
    mFBFilterR = mFBFilterR + mFBCoeff * (delayedR - mFBFilterR);

    // Write input + feedback. Sanitise so a single non-finite sample can never
    // get trapped recirculating in the buffer (which would loop forever).
    float wL = L + mFBFilterL * mFeedback;
    float wR = R + mFBFilterR * mFeedback;
    if (!std::isfinite(wL)) { wL = 0.0f; mFBFilterL = 0.0f; }
    if (!std::isfinite(wR)) { wR = 0.0f; mFBFilterR = 0.0f; }
    mBufL[mWritePos] = std::clamp(wL, -8.0f, 8.0f);
    mBufR[mWritePos] = std::clamp(wR, -8.0f, 8.0f);

    // Mix wet/dry
    L = L + mMix * (delayedL - L);
    R = R + mMix * (delayedR - R);

    mWritePos = (mWritePos + 1) % bufSize;
}

} // namespace drift
