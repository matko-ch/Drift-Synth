#include "Envelope.h"
#include <cmath>
#include <algorithm>

namespace drift {

// Exponential envelope coefficients — see Nigel Redmon (earlevel.com).
// The envelope approximates analog RC timing with a log-domain one-pole filter.
// coeff = exp(-log((1+targetRatio)/targetRatio) / (timeSec * sampleRate))
// base  = (1 + targetRatio) * (1 - coeff)  — applied to attack or decay polarity.

static constexpr float kAttackTargetRatio  = 0.3f;   // reaches ~100% before end
static constexpr float kDecayTargetRatio   = 0.0001f; // exponential tail

float Envelope::computeCoeff(float timeSec, float targetRatio) const noexcept {
    if (timeSec <= 0.0f) return 0.0f;
    const double n = static_cast<double>(timeSec) * mSampleRate;
    return static_cast<float>(std::exp(-std::log((1.0 + targetRatio) / targetRatio) / n));
}

void Envelope::prepare(double sampleRate) noexcept {
    mSampleRate = sampleRate;
    reset();
    // Force coefficient recomputation
    setParams(mParams);
}

void Envelope::setParams(const Params& p) noexcept {
    mParams = p;
    const float atr = kAttackTargetRatio;
    const float dtr = kDecayTargetRatio;

    mAttackCoeff  = computeCoeff(p.attackTime, atr);
    mAttackBase   = (1.0f + atr) * (1.0f - mAttackCoeff);

    mDecayCoeff   = computeCoeff(p.decayTime, dtr);
    mDecayBase    = (p.sustainLevel - dtr) * (1.0f - mDecayCoeff);

    mReleaseCoeff = computeCoeff(p.releaseTime, dtr);
    mReleaseBase  = -dtr * (1.0f - mReleaseCoeff);

    mDelaySamples = static_cast<int>(p.delayTime * mSampleRate);
    mHoldSamples  = static_cast<int>(p.holdTime  * mSampleRate);
}

void Envelope::reset() noexcept {
    mStage        = Stage::Idle;
    mValue        = 0.0f;
    mSampleCount  = 0;
}

void Envelope::noteOn(float velocity) noexcept {
    mVelocity = std::clamp(velocity, 0.0f, 1.0f);
    if (mParams.delayTime > 0.0f) {
        transitionTo(Stage::Delay);
    } else {
        transitionTo(Stage::Attack);
    }
}

void Envelope::noteOff() noexcept {
    if (mStage != Stage::Idle)
        transitionTo(Stage::Release);
}

void Envelope::transitionTo(Stage s) noexcept {
    mStage       = s;
    mSampleCount = 0;
}

float Envelope::process() noexcept {
    switch (mStage) {
        case Stage::Idle:
            return 0.0f;

        case Stage::Delay:
            if (++mSampleCount >= mDelaySamples)
                transitionTo(Stage::Attack);
            return 0.0f;

        case Stage::Attack:
            mValue = mAttackBase + mAttackCoeff * mValue;
            if (mValue >= 1.0f) {
                mValue = 1.0f;
                transitionTo(mHoldSamples > 0 ? Stage::Hold : Stage::Decay);
            }
            break;

        case Stage::Hold:
            mValue = 1.0f;
            if (++mSampleCount >= mHoldSamples)
                transitionTo(Stage::Decay);
            break;

        case Stage::Decay:
            mValue = mDecayBase + mDecayCoeff * mValue;
            if (mValue <= mParams.sustainLevel + 1e-5f) {
                mValue = mParams.sustainLevel;
                transitionTo(Stage::Sustain);
            }
            break;

        case Stage::Sustain:
            mValue = mParams.sustainLevel;
            break;

        case Stage::Release:
            mValue = mReleaseBase + mReleaseCoeff * mValue;
            if (mValue <= 1e-5f) {
                mValue = 0.0f;
                transitionTo(Stage::Idle);
            }
            break;
    }

    return mValue * mVelocity;
}

} // namespace drift
