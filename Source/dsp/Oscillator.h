#pragma once
#include "Constants.h"
#include "PolyBLEP.h"

namespace drift {

enum class OscShape : int {
    Sine = 0,
    Triangle,
    Saw,
    Square,
    Pulse,
    Count
};

class Oscillator {
public:
    void prepare(double sampleRate, float phase = 0.0f) noexcept;
    void reset(float phase = 0.0f) noexcept;

    // Returns one output sample, advances internal phase.
    // phaseInc : normalised freq (Hz / sampleRate); caller applies pitch, detune, etc.
    // shape    : waveform
    // pw       : pulse width [0.05, 0.95] — used only for Pulse shape
    [[nodiscard]] DRIFT_HOT
    float process(float phaseInc, OscShape shape, float pw = 0.5f) noexcept;

    // Hard-sync: call this after the master oscillator wraps its phase.
    // Resets slave phase to syncPhase (usually 0) with a blep correction
    // applied on next call.
    void syncReset(float syncPhase = 0.0f) noexcept;

    [[nodiscard]] float getPhase() const noexcept { return mPhase; }
    void setPhase(float p) noexcept { mPhase = p; }

private:
    DRIFT_HOT float processSine()                       noexcept;
    DRIFT_HOT float processTriangle(float phaseInc)     noexcept;
    DRIFT_HOT float processSaw(float phaseInc)          noexcept;
    DRIFT_HOT float processPulse(float phaseInc, float pw) noexcept;

    float mSampleRate  = 44100.0f;
    float mPhase       = 0.0f;

    // Triangle: running integration of band-limited square
    float mTriAccum    = 0.0f;
    float mTriDCx      = 0.0f; // DC-blocker state: x[n-1]
    float mTriDCy      = 0.0f; // DC-blocker state: y[n-1]
    float mDCBlockR    = 0.9997f;

    // Hard-sync discontinuity tracking
    bool  mSyncPending = false;
    float mSyncPhase   = 0.0f;
};

} // namespace drift
