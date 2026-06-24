#pragma once
#include "../Constants.h"

namespace drift {

// LoFi crusher: sample-rate reduction (sample & hold) + bit-depth quantisation.
// Great for grit, vinyl/8-bit and experimental textures. Inherently bounded.
class Bitcrusher {
public:
    void prepare(double sampleRate) noexcept;
    void reset() noexcept;

    // bits:   [1, 16]  – quantisation depth (16 ≈ clean)
    // crush:  [0, 1]   – sample-rate reduction amount (0 = none)
    // mix:    [0, 1]
    void setParameters(float bits, float crush, float mix) noexcept;
    void setBypassed(bool b) noexcept { mBypassed = b; }

    void processStereo(float& L, float& R) noexcept;

private:
    float mLevels = 65535.0f;   // 2^bits - 1
    float mStep   = 1.0f;       // phase increment for the S&H (1 = no reduction)
    float mMix    = 0.0f;
    bool  mBypassed = false;

    float mPhase = 0.0f;
    float mHoldL = 0.0f, mHoldR = 0.0f;
};

} // namespace drift
