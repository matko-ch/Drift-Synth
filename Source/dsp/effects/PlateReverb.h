#pragma once
#include "../Constants.h"
#include <array>

namespace drift {

// Plate reverb using a Feedback Delay Network (FDN) with 8 delay lines.
// Reference: Jot & Chaigne (1991); Smith (CCRMA) JOS reverb designs.
// All buffer sizes are prime-number lengths for diffuse density.
class PlateReverb {
public:
    void prepare(double sampleRate) noexcept;
    void reset() noexcept;

    // size:    [0, 1]  – room size / feedback gain
    // damping: [0, 1]  – high-frequency damping
    // mix:     [0, 1]  – wet/dry
    void setParameters(float size, float damping, float mix) noexcept;
    void setBypassed(bool b) noexcept { mBypassed = b; }

    void processStereo(float& L, float& R) noexcept;

private:
    static constexpr int kNumAP = 4;   // All-pass diffusors per channel
    static constexpr int kNumLP = 8;   // Delay lines in FDN

    // All-pass diffusor (Schroeder)
    struct AllPass {
        std::array<float, 1024> buf{};
        int writePos = 0;
        int delayLen = 113;
        float gain   = 0.7f;

        void reset() { buf.fill(0.0f); writePos = 0; }
        float process(float x) noexcept;
    };

    // Feedback delay line with one-pole LP damping
    struct DelayLine {
        std::array<float, kReverbAPFLen> buf{};
        int writePos = 0;
        int delayLen = 557;
        float lpState = 0.0f;
        float lpCoeff = 0.5f;
        float gain    = 0.7f;

        void reset() { buf.fill(0.0f); writePos = 0; lpState = 0.0f; }
        float process(float x) noexcept;
    };

    std::array<AllPass,   kNumAP> mApL, mApR;
    std::array<DelayLine, kNumLP> mDelayLines;

    float mFeedback = 0.7f;
    float mMix      = 0.5f;
    bool  mBypassed = false;

    float mSampleRate = 44100.0f;

    // Hadamard mixing matrix for FDN (stateless — just indices)
    float mFDNState[kNumLP] = {};
    static constexpr int kDelayLengths[kNumLP] = {
        557, 617, 697, 769, 883, 937, 1049, 1153
    };
};

} // namespace drift
