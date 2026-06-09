#include "Oscillator.h"
#include <cmath>

namespace drift {

void Oscillator::prepare(double sampleRate, float phase) noexcept {
    mSampleRate = static_cast<float>(sampleRate);
    // One-pole DC blocker at ~1 Hz
    mDCBlockR = 1.0f - (kTwoPi * 1.0f / mSampleRate);
    reset(phase);
}

void Oscillator::reset(float phase) noexcept {
    mPhase = phase;

    // Initialise the triangle integrator to its steady-state value so there is
    // no settling transient. Triangle is -1 at phase=0, +1 at phase=0.5, etc.
    const float triSteady = (phase < 0.5f) ? (4.0f * phase - 1.0f)
                                            : (3.0f - 4.0f * phase);
    mTriAccum  = triSteady;
    mTriDCx    = triSteady;   // x[n-1] = same as first sample → no DC spike
    mTriDCy    = triSteady;   // y[n-1] = same → HPF output starts at correct value

    mSyncPending = false;
}

void Oscillator::syncReset(float syncPhase) noexcept {
    mSyncPending = true;
    mSyncPhase   = syncPhase;
}

float Oscillator::process(float phaseInc, OscShape shape, float pw) noexcept {
    float out = 0.0f;

    switch (shape) {
        case OscShape::Sine:     out = processSine();                break;
        case OscShape::Triangle: out = processTriangle(phaseInc);    break;
        case OscShape::Saw:      out = processSaw(phaseInc);         break;
        case OscShape::Square:   out = processPulse(phaseInc, 0.5f); break;
        case OscShape::Pulse:    out = processPulse(phaseInc, pw);   break;
        default: break;
    }

    // Advance phase
    mPhase += phaseInc;
    if (mPhase >= 1.0f)
        mPhase -= 1.0f;

    // Apply hard-sync after phase advance (so we process the current sample first)
    if (mSyncPending) {
        mPhase       = mSyncPhase;
        mSyncPending = false;
        // Accumulator reset prevents a DC click on sync
        mTriAccum = 0.0f;
    }

    return out;
}

// ── Waveforms ─────────────────────────────────────────────────────────────────

float Oscillator::processSine() noexcept {
    return std::sin(mPhase * kTwoPi);
}

float Oscillator::processSaw(float phaseInc) noexcept {
    return blSaw(mPhase, phaseInc);
}

float Oscillator::processPulse(float phaseInc, float pw) noexcept {
    return blPulse(mPhase, phaseInc, pw);
}

// Band-limited triangle via integration of a PolyBLEP square.
// Mathematical basis: the integral of a ±1 square wave of period 1 is a triangle.
// Integration factor (4*phaseInc) normalises amplitude to ±1 for all frequencies.
// A one-pole HPF at ~1 Hz removes any residual DC.
float Oscillator::processTriangle(float phaseInc) noexcept {
    const float sq = blPulse(mPhase, phaseInc, 0.5f);
    mTriAccum += 4.0f * phaseInc * sq;

    // DC blocker: y[n] = x[n] - x[n-1] + R*y[n-1]
    const float out = mTriAccum - mTriDCx + mDCBlockR * mTriDCy;
    mTriDCx = mTriAccum;
    mTriDCy = out;
    return out;
}

} // namespace drift
