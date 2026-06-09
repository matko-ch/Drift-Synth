#include "Drive.h"
#include <cmath>

namespace drift {

void Drive::prepare(double sampleRate) noexcept {
    // Half-band Butterworth one-pole at 0.45*fs (≈ 20 kHz at 44.1 kHz native → 20 kHz at 88.2 kHz oversampled)
    const float fc = 0.45f;
    mHBcoeff = (1.0f - kPi * fc) / (1.0f + kPi * fc);
    reset();
    (void)sampleRate;
}

void Drive::reset() noexcept {
    mUpsampL[0] = mUpsampL[1] = 0.0f;
    mUpsampR[0] = mUpsampR[1] = 0.0f;
    mDownL[0]   = mDownL[1]   = 0.0f;
    mDownR[0]   = mDownR[1]   = 0.0f;
}

void Drive::setParameters(float amount, float mix) noexcept {
    mAmount = clamp(amount, 0.0f, 1.0f);
    mMix    = clamp(mix, 0.0f, 1.0f);
}

float Drive::saturate(float x) const noexcept {
    // Gain stage + soft clip via tanh
    const float gain = 1.0f + mAmount * 15.0f;
    return fastTanh(x * gain) / std::sqrt(gain * 0.25f + 0.75f);
}

void Drive::processStereo(float& L, float& R) noexcept {
    if (mBypassed || mAmount < 1e-4f) return;

    const float dryL = L, dryR = R;

    // ── 2× upsample (insert zero) then one-pole AA filter ─────────────────
    auto upsample = [&](float x, float* s) -> std::pair<float,float> {
        // Insert zero between samples, then filter
        float s0 = mHBcoeff * (x - s[0]) + x; s[0] = s0;
        float s1 = mHBcoeff * (x - s[1]); s[1] = s1 + x; // crude 2-tap
        return { s0 * 2.0f, s1 * 2.0f };
    };

    auto [lUp0, lUp1] = upsample(L, mUpsampL);
    auto [rUp0, rUp1] = upsample(R, mUpsampR);

    // ── Saturate both oversampled sub-samples ──────────────────────────────
    lUp0 = saturate(lUp0);
    lUp1 = saturate(lUp1);
    rUp0 = saturate(rUp0);
    rUp1 = saturate(rUp1);

    // ── 2× downsample (decimate, AA filter first) ──────────────────────────
    auto downsample = [&](float x0, float x1, float* s) -> float {
        float y0 = mHBcoeff * (x0 - s[0]) + x0; s[0] = y0;
        float y1 = mHBcoeff * (x1 - s[1]) + x1; s[1] = y1;
        return (y0 + y1) * 0.5f;
    };

    float wetL = downsample(lUp0, lUp1, mDownL);
    float wetR = downsample(rUp0, rUp1, mDownR);

    L = dryL + mMix * (wetL - dryL);
    R = dryR + mMix * (wetR - dryR);
}

} // namespace drift
