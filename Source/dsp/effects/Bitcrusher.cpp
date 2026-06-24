#include "Bitcrusher.h"
#include <cmath>

namespace drift {

void Bitcrusher::prepare(double /*sampleRate*/) noexcept { reset(); }

void Bitcrusher::reset() noexcept {
    mPhase = 1.0f;  // force a sample on the first call
    mHoldL = mHoldR = 0.0f;
}

void Bitcrusher::setParameters(float bits, float crush, float mix) noexcept {
    const float b = clamp(bits, 1.0f, 16.0f);
    mLevels = std::pow(2.0f, b) - 1.0f;
    // crush 0→1 maps the effective sample rate from full down to ~1/30.
    mStep = 1.0f / (1.0f + clamp(crush, 0.0f, 1.0f) * 29.0f);
    mMix  = clamp(mix, 0.0f, 1.0f);
}

void Bitcrusher::processStereo(float& L, float& R) noexcept {
    if (mBypassed) return;

    mPhase += mStep;
    if (mPhase >= 1.0f) {
        mPhase -= 1.0f;
        // Quantise to the reduced bit depth, then sample & hold.
        mHoldL = std::round(clamp(L, -1.0f, 1.0f) * mLevels) / mLevels;
        mHoldR = std::round(clamp(R, -1.0f, 1.0f) * mLevels) / mLevels;
    }

    L = L + mMix * (mHoldL - L);
    R = R + mMix * (mHoldR - R);
}

} // namespace drift
