#pragma once
#include "Constants.h"
// Band-limiting via PolyBLEP — header-only, real-time-safe, no allocations.
// Reference: Valimaki & Pakarinen (2006); Finke (2012)

namespace drift {

// First-order PolyBLEP correction for a unit step at normalised phase t=0.
// t   : current normalised phase [0, 1)
// dt  : normalised phase increment (freq / sampleRate)
// Returns a correction that must be SUBTRACTED from a rising saw
// (naive saw = 2*t - 1 with step from +1 → −1 at t=1, wrapped to t=0).
[[nodiscard]] DRIFT_HOT inline float polyBlep(float t, float dt) noexcept {
    if (t < dt) {
        // Just past the transition (0 ≤ t < dt): apply leading correction
        t /= dt;
        return t + t - t * t - 1.0f;
    } else if (t > 1.0f - dt) {
        // Just before the transition (1-dt < t < 1): trailing correction
        t = (t - 1.0f) / dt;
        return t * t + t + t + 1.0f;
    }
    return 0.0f;
}

// Band-limited sawtooth [−1, +1] rising from −1 at t=0 to +1 at t→1.
[[nodiscard]] DRIFT_HOT inline float blSaw(float t, float dt) noexcept {
    return (2.0f * t - 1.0f) - polyBlep(t, dt);
}

// Band-limited pulse/square with variable duty cycle.
// duty in (0, 1) — 0.5 gives a symmetric square wave.
[[nodiscard]] DRIFT_HOT inline float blPulse(float t, float dt, float duty) noexcept {
    float s = (t < duty) ? 1.0f : -1.0f;
    // Rising edge at t=0
    s += polyBlep(t, dt);
    // Falling edge at t=duty
    float t2 = t - duty;
    if (t2 < 0.0f) t2 += 1.0f;
    s -= polyBlep(t2, dt);
    return s;
}

} // namespace drift
