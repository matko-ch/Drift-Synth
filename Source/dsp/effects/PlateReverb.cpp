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

// ── Feedback delay line ───────────────────────────────────────────────────────
float PlateReverb::DelayLine::process(float x) noexcept {
    const int readPos = (writePos - delayLen + static_cast<int>(buf.size())) % static_cast<int>(buf.size());
    const float out   = buf[readPos];

    // One-pole LP damping
    lpState = lpState + lpCoeff * (out - lpState);

    buf[writePos] = x + lpState * gain;
    writePos = (writePos + 1) % static_cast<int>(buf.size());
    return out;
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
    std::fill(std::begin(mFDNState), std::end(mFDNState), 0.0f);
}

void PlateReverb::setParameters(float size, float damping, float mix) noexcept {
    // Size → feedback gain: 0..1 maps to 0.5..0.98
    mFeedback = 0.5f + std::clamp(size, 0.0f, 1.0f) * 0.48f;
    const float lpCutoff = 1.0f - std::clamp(damping, 0.0f, 1.0f) * 0.9f;
    for (auto& d : mDelayLines) {
        d.gain    = mFeedback;
        d.lpCoeff = lpCutoff;
    }
    mMix = std::clamp(mix, 0.0f, 1.0f);
}

void PlateReverb::processStereo(float& L, float& R) noexcept {
    if (mBypassed) return;

    const float dryL = L, dryR = R;

    // ── Diffuse input through all-pass chain ──────────────────────────────
    float diffL = L, diffR = R;
    for (auto& a : mApL) diffL = a.process(diffL);
    for (auto& a : mApR) diffR = a.process(diffR);

    // ── FDN: 8-line with Hadamard mixing ──────────────────────────────────
    // Simple mixing: feed each line from the sum of all previous states + input
    const float input = (diffL + diffR) * 0.5f;

    // Hadamard (orthogonal) 8×8 mix — simplified as a sum + sign alternation
    float mix = 0.0f;
    for (int i = 0; i < kNumLP; ++i) mix += mFDNState[i];

    for (int i = 0; i < kNumLP; ++i) {
        const float sign = (i & 1) ? -1.0f : 1.0f;
        mFDNState[i] = mDelayLines[i].process(input + sign * mix * (1.0f / kNumLP));
    }

    // ── Tap left and right from different delay lines ─────────────────────
    const float wetL = mFDNState[0] + mFDNState[2] - mFDNState[4] - mFDNState[6];
    const float wetR = mFDNState[1] + mFDNState[3] - mFDNState[5] - mFDNState[7];

    L = dryL + mMix * (wetL * 0.25f - dryL);
    R = dryR + mMix * (wetR * 0.25f - dryR);
}

} // namespace drift
