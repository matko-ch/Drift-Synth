#include "LFO.h"
#include <cmath>

namespace drift {

void LFO::prepare(double sampleRate) noexcept {
    mSampleRate = static_cast<float>(sampleRate);
    reset();
}

void LFO::reset() noexcept {
    mPhase     = 0.0f;
    mSHOutput  = 0.0f;
    mSHPrevPhase = 1.0f;
}

void LFO::noteOn() noexcept {
    if (mRetrigger) {
        mPhase = 0.0f;
        mSHPrevPhase = 1.0f;
    }
}

void LFO::setRate(float hz) noexcept {
    mPhaseInc = hz / mSampleRate;
}

float LFO::process(float depth) noexcept {
    float raw = 0.0f;

    switch (mShape) {
        case LFOShape::Sine:
            raw = std::sin(mPhase * kTwoPi);
            break;
        case LFOShape::Triangle:
            raw = (mPhase < 0.5f) ? (4.0f * mPhase - 1.0f)
                                   : (3.0f - 4.0f * mPhase);
            break;
        case LFOShape::Saw:
            raw = 2.0f * mPhase - 1.0f;
            break;
        case LFOShape::Square:
            raw = mPhase < 0.5f ? 1.0f : -1.0f;
            break;
        case LFOShape::SampleAndHold:
            // Update held value at each cycle reset
            if (mPhase < mSHPrevPhase)
                mSHOutput = 2.0f * (std::fmod(mSHOutput * 13.4f + 7.3f, 2.0f) - 1.0f);
            mSHPrevPhase = mPhase;
            raw = mSHOutput;
            break;
        default:
            break;
    }

    mPhase += mPhaseInc;
    if (mPhase >= 1.0f)
        mPhase -= 1.0f;

    return raw * depth;
}

} // namespace drift
