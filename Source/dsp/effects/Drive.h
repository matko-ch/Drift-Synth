#pragma once
#include "../Constants.h"

namespace drift {

// Soft-clipping drive stage — oversampled 2× to suppress aliasing.
// Uses tanh-based tube saturation character.
class Drive {
public:
    void prepare(double sampleRate) noexcept;
    void reset() noexcept;

    // amount: [0, 1] — 0 = bypass, 1 = full saturation
    // mix:    [0, 1] — wet/dry
    void setParameters(float amount, float mix) noexcept;
    void setBypassed(bool b) noexcept { mBypassed = b; }

    void processStereo(float& L, float& R) noexcept;

private:
    // 2× oversampling anti-aliasing filter (one-pole per channel, per direction)
    float mUpsampL[2] = {}, mUpsampR[2] = {};
    float mDownL[2]   = {}, mDownR[2]   = {};

    float mAmount  = 0.0f;
    float mMix     = 1.0f;
    bool  mBypassed = false;

    [[nodiscard]] float saturate(float x) const noexcept;
    // Simple half-band one-pole IIR for 2× up/downsample (good enough for 2×)
    float mHBcoeff = 0.0f;
};

} // namespace drift
