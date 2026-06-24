#pragma once
#include "../Constants.h"

namespace drift {

// Three-band tone EQ: low shelf (200 Hz), mid bell (1 kHz), high shelf (4 kHz).
// RBJ biquads; flat at 0 dB so it is transparent when unused.
class EQ {
public:
    void prepare(double sampleRate) noexcept;
    void reset() noexcept;

    // Gains in dB, [-12, +12].
    void setParameters(float lowDb, float midDb, float highDb) noexcept;
    void setBypassed(bool b) noexcept { mBypassed = b; }

    void processStereo(float& L, float& R) noexcept;

private:
    struct Biquad {
        float b0 = 1, b1 = 0, b2 = 0, a1 = 0, a2 = 0;
        float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
        void reset() noexcept { x1 = x2 = y1 = y2 = 0; }
        [[nodiscard]] DRIFT_HOT float process(float x) noexcept {
            const float y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
            x2 = x1; x1 = x; y2 = y1; y1 = y;
            return y;
        }
    };

    void recompute() noexcept;

    float mSampleRate = 44100.0f;
    float mLowDb = 0, mMidDb = 0, mHighDb = 0;
    bool  mBypassed = false;
    bool  mDirty = true;

    Biquad mLowL, mLowR, mMidL, mMidR, mHighL, mHighR;
};

} // namespace drift
