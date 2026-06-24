#pragma once
#include <cmath>
#include <cstdint>
#include <algorithm>

// Portability: [[gnu::hot]] is GCC/Clang-only; MSVC silently ignores it with C5030.
// Use DRIFT_HOT everywhere so the attribute is present on GCC/Clang and absent on MSVC.
#if defined(__GNUC__) || defined(__clang__)
#  define DRIFT_HOT [[gnu::hot]]
#else
#  define DRIFT_HOT
#endif

namespace drift {

// ── Mathematical constants ────────────────────────────────────────────────────
inline constexpr float kPi     = 3.14159265358979f;
inline constexpr float kTwoPi  = 6.28318530717959f;
inline constexpr float kHalfPi = 1.57079632679490f;
inline constexpr float kSqrt2  = 1.41421356237310f;

// ── Tuning ────────────────────────────────────────────────────────────────────
inline constexpr float kTuningA4Hz  = 440.0f;
inline constexpr float kMidiA4Note  = 69.0f;

// ── Polyphony / voice limits ──────────────────────────────────────────────────
inline constexpr int kMaxVoices       = 16;
inline constexpr int kMaxUnisonVoices = 8;
inline constexpr int kNumEnvelopes    = 3;  // AmpEnv, FilterEnv, ModEnv
inline constexpr int kNumLFOs         = 2;
inline constexpr int kNumModSlots     = 8;

// ── Filter ────────────────────────────────────────────────────────────────────
inline constexpr float kMinCutoffHz = 20.0f;
inline constexpr float kMaxCutoffHz = 20000.0f;

// ── Effects buffer sizes (samples at 48 kHz) ─────────────────────────────────
inline constexpr int kChorusDelayMax  = 4096;   // ~85 ms at 48 kHz
inline constexpr int kDelayBufferMax  = 192000; // 4 s at 48 kHz
inline constexpr int kReverbAPFLen    = 8192;

// ── Oversampling ──────────────────────────────────────────────────────────────
inline constexpr int kOversample = 2;

// ── Smoothing time-constant ───────────────────────────────────────────────────
inline constexpr float kParamSmoothSec = 0.005f;  // 5 ms
inline constexpr float kPitchSmoothSec = 0.001f;  // 1 ms

// ── Analog "drift" character ──────────────────────────────────────────────────
// Per-oscillator pitch wander modelled as a slow, bounded random walk in [-1,1].
inline constexpr float kMaxDriftCents = 16.0f;   // peak detune at drift = 1.0
inline constexpr float kDriftStep     = 0.0016f; // random-walk step size / sample
inline constexpr float kDriftLeak     = 0.99996f;// gentle pull back toward centre

// ── Helpers ───────────────────────────────────────────────────────────────────
[[nodiscard]] inline float midiNoteToHz(float midi) noexcept {
    return kTuningA4Hz * std::exp2((midi - kMidiA4Note) * (1.0f / 12.0f));
}

// Numerically stable tanh via rational Padé approximant — accurate for |x|<4
[[nodiscard]] DRIFT_HOT inline float fastTanh(float x) noexcept {
    const float x2 = x * x;
    return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}

[[nodiscard]] inline float clamp(float v, float lo, float hi) noexcept {
    return v < lo ? lo : (v > hi ? hi : v);
}

// ── Fast per-voice RNG (xorshift32) ───────────────────────────────────────────
// Deterministic, allocation-free, thread-local-by-instance. Each Voice/Oscillator
// owns its own state so there is no shared mutable data on the audio thread.
struct XorShift32 {
    uint32_t state = 0x9E3779B9u;
    DRIFT_HOT inline uint32_t next() noexcept {
        uint32_t x = state;
        x ^= x << 13; x ^= x >> 17; x ^= x << 5;
        return state = x;
    }
    // Uniform float in [-1, 1)
    DRIFT_HOT inline float bipolar() noexcept {
        return static_cast<float>(next()) * (2.0f / 4294967296.0f) - 1.0f;
    }
    // Uniform float in [0, 1)
    DRIFT_HOT inline float unipolar() noexcept {
        return static_cast<float>(next()) * (1.0f / 4294967296.0f);
    }
    inline void seed(uint32_t s) noexcept { state = (s == 0u) ? 0x9E3779B9u : s; }
};

// Musical soft-saturator: ~unity slope near zero, smoothly limits large peaks.
// Used as a gentle master/console stage to tame digital clipping with warmth.
[[nodiscard]] DRIFT_HOT inline float softSaturate(float x) noexcept {
    return fastTanh(x);
}

// Linear to dB (quiet floor at -144 dB)
[[nodiscard]] inline float linToDb(float lin) noexcept {
    return 20.0f * std::log10(std::max(lin, 1e-7f));
}

} // namespace drift
