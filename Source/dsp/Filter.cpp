#include "Filter.h"
#include <cmath>
#include <algorithm>

namespace drift {

void Filter::prepare(double sampleRate) noexcept {
    mSampleRate = static_cast<float>(sampleRate);
    mDirty = true;
    reset();
}

void Filter::reset() noexcept {
    mSvf1 = {};
    mSvf2 = {};
    mLadderS.fill(0.0f);
}

void Filter::setParameters(float cutoffHz, float resonance,
                            float drive, FilterType type) noexcept {
    const float c = clamp(cutoffHz, kMinCutoffHz, mSampleRate * 0.495f);
    const float r = clamp(resonance, 0.0f, 1.0f);
    const float d = clamp(drive, 0.0f, 1.0f);
    // Only recompute expensive trig if parameters actually changed
    if (c != mCutoffHz || r != mResonance || type != mType) {
        mCutoffHz  = c;
        mResonance = r;
        mType      = type;
        mDirty     = true;
    }
    mDrive = d;  // drive is applied inline — no coefficient needed
}

void Filter::copyCoeffsFrom(const Filter& src) noexcept {
    mCutoffHz  = src.mCutoffHz;
    mResonance = src.mResonance;
    mDrive     = src.mDrive;
    mType      = src.mType;
    mMoogG     = src.mMoogG;
    mMoogK     = src.mMoogK;
    mSvfC      = src.mSvfC;
    mDirty     = false;  // coefficients are already current
}

void Filter::recomputeIfDirty() noexcept {
    if (!mDirty) return;
    computeSVFCoeffs();
    computeMoogCoeffs();
    mDirty = false;
}

// ── Coefficient computation ───────────────────────────────────────────────────

void Filter::computeSVFCoeffs() noexcept {
    // Zavalishin §4.5, Simper (2013)
    // g = tan(π*fc/fs)  — pre-warped analogue frequency
    // k = 1/Q; maps resonance [0,1] → k [2, 0.01]
    const float g = std::tan(kPi * mCutoffHz / mSampleRate);
    const float k = 2.0f - 1.99f * mResonance;  // k→0.01 at resonance=1

    mSvfC.g  = g;
    mSvfC.k  = k;
    mSvfC.a1 = 1.0f / (1.0f + g * (g + k));
    mSvfC.a2 = g * mSvfC.a1;
    mSvfC.a3 = g * mSvfC.a2;
}

void Filter::computeMoogCoeffs() noexcept {
    // TPT one-pole gain per stage: G = g/(1+g), g = tan(π*fc/fs)
    const float g = std::tan(kPi * mCutoffHz / mSampleRate);
    mMoogG = g / (1.0f + g);
    // K=0 → no resonance; K=4 → self-oscillation threshold
    mMoogK = mResonance * 3.98f;
}

// ── SVF core (one sample, single stage) ──────────────────────────────────────

SVFOut Filter::processSVF(SVFState& st, float u) noexcept {
    const float& a1 = mSvfC.a1;
    const float& a2 = mSvfC.a2;
    const float& a3 = mSvfC.a3;
    const float& k  = mSvfC.k;

    const float v3 = u - st.ic2eq;
    const float v1 = a1 * st.ic1eq + a2 * v3;
    const float v2 = st.ic2eq + a2 * st.ic1eq + a3 * v3;

    st.ic1eq = 2.0f * v1 - st.ic1eq;
    st.ic2eq = 2.0f * v2 - st.ic2eq;

    SVFOut o;
    o.lp    = v2;
    o.bp    = v1;
    o.hp    = u - k * v1 - v2;
    o.notch = o.lp + o.hp;
    return o;
}

// ── Moog 4-pole ladder ────────────────────────────────────────────────────────
// TPT one-pole cascade with tanh saturation in feedback path.
// One-sample feedback delay — standard practice; sounds authentic.
float Filter::processMoog(float input) noexcept {
    const float G = mMoogG;
    const float K = mMoogK;
    const float driveGain = 1.0f + mDrive * 3.0f;

    // Feedback with saturation
    const float fb = fastTanh(K * mLadderS[3]);
    float x = fastTanh((input * driveGain - fb) * (1.0f / driveGain));

    // 4 cascaded TPT one-pole LP filters
    // y = G*(x - s) + s = G*x + (1-G)*s
    // s_new = 2*y - s  (trapezoidal state update)
    for (int i = 0; i < 4; ++i) {
        const float y = G * x + (1.0f - G) * mLadderS[i];
        mLadderS[i]   = 2.0f * y - mLadderS[i];
        x             = y;
    }
    return x;
}

// ── Public process ────────────────────────────────────────────────────────────

float Filter::process(float input) noexcept {
    recomputeIfDirty();

    // Drive blends linearly from bypass (mDrive=0) to tanh saturation (mDrive=1).
    // At drive=0 the output is identical to the undriven input — no gain change.
    const float driveGain  = 1.0f + mDrive * 3.0f;
    const float saturated  = fastTanh(input * driveGain) / driveGain;
    const float driven     = input + mDrive * (saturated - input);

    switch (mType) {
        case FilterType::LowPass12:
            return processSVF(mSvf1, driven).lp;
        case FilterType::HighPass12:
            return processSVF(mSvf1, driven).hp;
        case FilterType::BandPass12:
            return processSVF(mSvf1, driven).bp;
        case FilterType::Notch12:
            return processSVF(mSvf1, driven).notch;
        case FilterType::LowPass24:
            return processSVF(mSvf2, processSVF(mSvf1, driven).lp).lp;
        case FilterType::HighPass24:
            return processSVF(mSvf2, processSVF(mSvf1, driven).hp).hp;
        case FilterType::MoogLP24:
            return processMoog(input);
        default:
            return input;
    }
}

} // namespace drift
