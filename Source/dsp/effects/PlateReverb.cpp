#include "PlateReverb.h"
#include <cmath>
#include <algorithm>
#include <numeric>

namespace drift {

// ── All-pass diffusor ─────────────────────────────────────────────────────────
float PlateReverb::AllPass::process(float x) noexcept {
    const int readPos = (writePos - delayLen + static_cast<int>(buf.size())) % static_cast<int>(buf.size());
    const float d = buf[readPos];
    const float v = x - gain * d;
    buf[writePos]  = v;
    writePos = (writePos + 1) % static_cast<int>(buf.size());
    return d + gain * v;
}

// ── Pure delay line ───────────────────────────────────────────────────────────
float PlateReverb::DelayLine::read() const noexcept {
    const int sz = static_cast<int>(buf.size());
    const int readPos = (writePos - delayLen + sz) % sz;
    return buf[readPos];
}

void PlateReverb::DelayLine::write(float x) noexcept {
    const int sz = static_cast<int>(buf.size());
    // Sanitise: a single non-finite sample must never enter the recirculating
    // buffer, or it would poison the reverb permanently.
    if (!std::isfinite(x)) x = 0.0f;
    buf[writePos] = std::clamp(x, -8.0f, 8.0f);
    writePos = (writePos + 1) % sz;
}

// In-place fast Walsh–Hadamard transform (length 8). Combined with the 1/√8
// scaling below this is an orthonormal (energy-preserving) mixing matrix.
static inline void fwht8(float* a) noexcept {
    for (int len = 1; len < 8; len <<= 1)
        for (int i = 0; i < 8; i += (len << 1))
            for (int j = i; j < i + len; ++j) {
                const float x = a[j], y = a[j + len];
                a[j] = x + y; a[j + len] = x - y;
            }
}

// ── PlateReverb ───────────────────────────────────────────────────────────────
void PlateReverb::prepare(double sampleRate) noexcept {
    mSampleRate = static_cast<float>(sampleRate);

    // Scale delay lengths to sample rate (original tuned at 44.1 kHz)
    const float scale = mSampleRate / 44100.0f;
    const int apLengths[] = { 113, 127, 149, 167 };
    for (int i = 0; i < kNumAP; ++i) {
        mApL[i].delayLen = static_cast<int>(apLengths[i] * scale);
        mApR[i].delayLen = static_cast<int>(apLengths[i] * scale) + 7;  // slight offset L/R
        mApL[i].gain = mApR[i].gain = 0.7f;
    }
    for (int i = 0; i < kNumLP; ++i) {
        mDelayLines[i].delayLen = static_cast<int>(kDelayLengths[i] * scale);
    }

    reset();
}

void PlateReverb::reset() noexcept {
    for (auto& a : mApL) a.reset();
    for (auto& a : mApR) a.reset();
    for (auto& d : mDelayLines) d.reset();
    std::fill(std::begin(mLPState), std::end(mLPState), 0.0f);
}

void PlateReverb::setParameters(float size, float damping, float mix) noexcept {
    // Size → decay gain: 0..1 maps to 0.50..0.88. Strictly below 1, so the
    // orthonormal feedback network is unconditionally stable.
    mDecay   = 0.50f + std::clamp(size, 0.0f, 1.0f) * 0.38f;
    mLPCoeff = 1.0f - std::clamp(damping, 0.0f, 1.0f) * 0.9f;
    mMix     = std::clamp(mix, 0.0f, 1.0f);
}

void PlateReverb::processStereo(float& L, float& R) noexcept {
    if (mBypassed) return;

    const float dryL = L, dryR = R;

    // ── Diffuse input through all-pass chain ──────────────────────────────
    float diffL = L, diffR = R;
    for (auto& a : mApL) diffL = a.process(diffL);
    for (auto& a : mApR) diffR = a.process(diffR);
    const float input = (diffL + diffR) * 0.5f;

    // ── Read the 8 delay-line outputs ──────────────────────────────────────
    float s[kNumLP];
    for (int i = 0; i < kNumLP; ++i) s[i] = mDelayLines[i].read();

    // Tap stereo output from the raw delayed signals.
    const float wetL = (s[0] + s[2] - s[4] - s[6]) * 0.5f;
    const float wetR = (s[1] + s[3] - s[5] - s[7]) * 0.5f;

    // ── Feedback: per-line damping → orthonormal mix → single decay gain ───
    float h[kNumLP];
    for (int i = 0; i < kNumLP; ++i) {
        mLPState[i] += mLPCoeff * (s[i] - mLPState[i]);   // damping
        h[i] = mLPState[i];
    }
    fwht8(h);  // energy-preserving mix (orthonormal after the 1/√8 below)

    // 1/√8 normalises the Hadamard transform; mDecay (<1) sets the tail length.
    // Because the matrix is orthonormal, the total loop gain is exactly mDecay,
    // so the network can never grow without bound — no matter the input.
    const float g = mDecay * 0.35355339f;  // 0.35355339 = 1/√8
    for (int i = 0; i < kNumLP; ++i)
        mDelayLines[i].write(input + h[i] * g);

    L = dryL + mMix * (wetL - dryL);
    R = dryR + mMix * (wetR - dryR);
}

} // namespace drift
