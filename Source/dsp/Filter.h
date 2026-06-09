#pragma once
#include "Constants.h"
#include <array>

namespace drift {

enum class FilterType : int {
    LowPass12 = 0,
    HighPass12,
    BandPass12,
    Notch12,
    LowPass24,
    HighPass24,
    MoogLP24,
    Count
};

// ── ZDF State-Variable Filter (Simper/Zavalishin TPT topology) ────────────────
// Reference: Zavalishin (2012) "The Art of VA Filter Design" §4.5
//            Simper (2013) "Solving the Continuous SVF Equations…"
struct SVFState {
    float ic1eq = 0.0f;  // integrator 1 midpoint
    float ic2eq = 0.0f;  // integrator 2 midpoint
};

struct SVFCoeffs {
    float g  = 0.0f;
    float k  = 0.0f;
    float a1 = 0.0f;
    float a2 = 0.0f;
    float a3 = 0.0f;
};

struct SVFOut { float lp, bp, hp, notch; };

// ── Filter class ──────────────────────────────────────────────────────────────
class Filter {
public:
    void prepare(double sampleRate) noexcept;
    void reset() noexcept;

    // Stores desired parameters and marks coefficients dirty.
    // Cheap: no trig until process() is called and params actually changed.
    void setParameters(float cutoffHz, float resonance,
                       float drive, FilterType type) noexcept;

    // Process one sample — call from audio thread only.
    [[nodiscard]] DRIFT_HOT float process(float input) noexcept;

    // Copy coefficients from another filter (same params, different state).
    // Allows one setParameters/recompute to serve both L and R channels.
    void copyCoeffsFrom(const Filter& src) noexcept;

private:
    void recomputeIfDirty() noexcept;
    // SVF processes one sample and returns all outputs
    DRIFT_HOT SVFOut  processSVF(SVFState& st, float u) noexcept;

    // Moog ladder: 4-pole TPT with nonlinear feedback, one-sample-delayed.
    // Stable at all resonance settings; self-oscillates cleanly at resonance=1.
    DRIFT_HOT float processMoog(float input) noexcept;

    void computeSVFCoeffs() noexcept;
    void computeMoogCoeffs() noexcept;

    float mSampleRate = 44100.0f;
    float mCutoffHz   = -1.0f;  // invalid → forces first recompute
    float mResonance  = -1.0f;
    float mDrive      = -1.0f;
    FilterType mType  = FilterType::Count;  // invalid → forces first recompute
    bool  mDirty      = true;

    // SVF states — two for cascaded 24 dB modes
    SVFState  mSvf1, mSvf2;
    SVFCoeffs mSvfC;

    // Moog ladder — 4 TPT one-pole integrator states
    std::array<float, 4> mLadderS = {};
    float mMoogG = 0.0f;  // per-stage: tan(π*fc/fs)
    float mMoogK = 0.0f;  // resonance scale 0..4
};

} // namespace drift
