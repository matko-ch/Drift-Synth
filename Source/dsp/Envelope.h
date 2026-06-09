#pragma once
#include "Constants.h"

namespace drift {

// DAHDSR envelope with exponential analog-like curves.
// All times in seconds. Uses per-sample exponential approximation — no branches
// in the hot path beyond the stage comparison.
class Envelope {
public:
    enum class Stage : uint8_t {
        Idle = 0,
        Delay,
        Attack,
        Hold,
        Decay,
        Sustain,
        Release
    };

    struct Params {
        float delayTime   = 0.0f;   // seconds
        float attackTime  = 0.01f;  // seconds
        float holdTime    = 0.0f;   // seconds
        float decayTime   = 0.3f;   // seconds
        float sustainLevel= 0.7f;   // 0..1
        float releaseTime = 0.5f;   // seconds
        // Curvature: 0=linear, 1=max-exp. Typical analog = 0.9
        float attackCurve = 0.9f;
        float decayCurve  = 0.9f;
    };

    void prepare(double sampleRate) noexcept;
    void setParams(const Params& p) noexcept;

    // Trigger note-on (velocity 0..1)
    void noteOn(float velocity = 1.0f) noexcept;
    // Begin release phase
    void noteOff() noexcept;
    // Hard reset (voice stealing)
    void reset() noexcept;

    [[nodiscard]] DRIFT_HOT float process() noexcept;
    [[nodiscard]] bool isIdle() const noexcept { return mStage == Stage::Idle; }
    [[nodiscard]] bool isReleasing() const noexcept { return mStage == Stage::Release; }
    [[nodiscard]] float getValue() const noexcept { return mValue; }
    [[nodiscard]] float getVelocity() const noexcept { return mVelocity; }

private:
    void transitionTo(Stage s) noexcept;
    [[nodiscard]] float computeCoeff(float timeSec, float targetRatio) const noexcept;

    double mSampleRate = 44100.0;
    Params mParams;
    Stage  mStage    = Stage::Idle;
    float  mValue    = 0.0f;
    float  mVelocity = 1.0f;

    // Per-stage cached coefficients (recomputed on setParams / noteOn)
    float  mAttackCoeff   = 0.0f;
    float  mAttackBase    = 0.0f;
    float  mDecayCoeff    = 0.0f;
    float  mDecayBase     = 0.0f;
    float  mReleaseCoeff  = 0.0f;
    float  mReleaseBase   = 0.0f;

    // Sample counters for Delay and Hold stages
    int    mSampleCount   = 0;
    int    mDelaySamples  = 0;
    int    mHoldSamples   = 0;
};

} // namespace drift
