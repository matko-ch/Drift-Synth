#include "Phaser.h"
#include <cmath>

namespace drift {

void Phaser::prepare(double sampleRate) noexcept {
    mSampleRate = static_cast<float>(sampleRate);
    setParameters(0.3f, 0.6f, 0.5f);
    reset();
}

void Phaser::reset() noexcept {
    mApxL.fill(0.0f); mApyL.fill(0.0f);
    mApxR.fill(0.0f); mApyR.fill(0.0f);
    mFbL = mFbR = 0.0f;
    mLfoPhase = 0.0f;
}

void Phaser::setParameters(float rateHz, float depth, float mix) noexcept {
    mLfoInc = clamp(rateHz, 0.05f, 8.0f) / mSampleRate;
    mDepth  = clamp(depth, 0.0f, 1.0f);
    mMix    = clamp(mix, 0.0f, 1.0f);
}

void Phaser::processStereo(float& L, float& R) noexcept {
    if (mBypassed) return;

    // LFO (two phases 90° apart for stereo motion)
    mLfoPhase += mLfoInc;
    if (mLfoPhase >= 1.0f) mLfoPhase -= 1.0f;
    const float lfoL = 0.5f - 0.5f * std::cos(kTwoPi * mLfoPhase);
    const float lfoR = 0.5f - 0.5f * std::cos(kTwoPi * (mLfoPhase + 0.25f));

    // Sweep break frequency 200 Hz → ~1.8 kHz
    auto coeff = [&](float lfo) {
        const float f = 200.0f + (1600.0f * mDepth) * lfo;
        const float t = std::tan(kPi * f / mSampleRate);
        return (t - 1.0f) / (t + 1.0f);
    };
    const float gL = coeff(lfoL), gR = coeff(lfoR);

    auto chain = [](float in, float g, std::array<float, kStages>& xs,
                    std::array<float, kStages>& ys) {
        float v = in;
        for (int i = 0; i < kStages; ++i) {
            const float y = g * v + xs[i] - g * ys[i];  // 1st-order all-pass
            xs[i] = v; ys[i] = y; v = y;
        }
        return v;
    };

    const float inL = L + mFbL * mFeedback;
    const float inR = R + mFbR * mFeedback;
    const float wetL = chain(inL, gL, mApxL, mApyL);
    const float wetR = chain(inR, gR, mApxR, mApyR);
    mFbL = wetL; mFbR = wetR;

    L = L + mMix * (wetL - L);
    R = R + mMix * (wetR - R);
}

} // namespace drift
