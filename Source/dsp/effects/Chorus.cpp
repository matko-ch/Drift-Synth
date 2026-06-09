#include "Chorus.h"
#include <cmath>
#include <algorithm>

namespace drift {

void Chorus::prepare(double sampleRate) noexcept {
    mSampleRate = static_cast<float>(sampleRate);
    mBaseDelay  = kBaseDelayMs * 0.001f * mSampleRate;
    mMaxDepth   = kMaxDepthMs  * 0.001f * mSampleRate;
    reset();
}

void Chorus::reset() noexcept {
    mBufL.fill(0.0f);
    mBufR.fill(0.0f);
    mWritePos  = 0;
    mLFOPhaseL = 0.0f;
    mLFOPhaseR = 0.25f;  // 90° offset
}

void Chorus::setParameters(float rate, float depth, float mix) noexcept {
    mLFOInc = std::clamp(rate, 0.05f, 10.0f) / mSampleRate;
    mDepth  = std::clamp(depth, 0.0f, 1.0f);
    mMix    = std::clamp(mix,   0.0f, 1.0f);
}

float Chorus::readInterp(const float* buf, int size, float readPos) const noexcept {
    // Linear interpolation for fractional delay read
    const int i0 = static_cast<int>(readPos) % size;
    const int i1 = (i0 + 1) % size;
    const float frac = readPos - std::floor(readPos);
    return buf[i0] * (1.0f - frac) + buf[i1] * frac;
}

void Chorus::processStereo(float& L, float& R) noexcept {
    if (mBypassed) return;

    const int bufSize = static_cast<int>(mBufL.size());

    // Write
    mBufL[mWritePos] = L;
    mBufR[mWritePos] = R;

    // LFO — sine modulated delay
    const float lfoL = std::sin(mLFOPhaseL * kTwoPi);
    const float lfoR = std::sin(mLFOPhaseR * kTwoPi);

    const float delayL = mBaseDelay + lfoL * mDepth * mMaxDepth;
    const float delayR = mBaseDelay + lfoR * mDepth * mMaxDepth;

    // Read positions (wrapped)
    float readPosL = static_cast<float>(mWritePos) - delayL;
    if (readPosL < 0.0f) readPosL += static_cast<float>(bufSize);
    float readPosR = static_cast<float>(mWritePos) - delayR;
    if (readPosR < 0.0f) readPosR += static_cast<float>(bufSize);

    const float wetL = readInterp(mBufL.data(), bufSize, readPosL);
    const float wetR = readInterp(mBufR.data(), bufSize, readPosR);

    L = L + mMix * (wetL - L);
    R = R + mMix * (wetR - R);

    // Advance
    mWritePos = (mWritePos + 1) % bufSize;
    mLFOPhaseL += mLFOInc;
    if (mLFOPhaseL >= 1.0f) mLFOPhaseL -= 1.0f;
    mLFOPhaseR += mLFOInc;
    if (mLFOPhaseR >= 1.0f) mLFOPhaseR -= 1.0f;
}

} // namespace drift
