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

// Linear to dB (quiet floor at -144 dB)
[[nodiscard]] inline float linToDb(float lin) noexcept {
    return 20.0f * std::log10(std::max(lin, 1e-7f));
}

} // namespace drift
