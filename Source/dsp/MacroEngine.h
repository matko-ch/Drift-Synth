#pragma once
#include "Voice.h"   // PatchParams

namespace drift {

// ── Raw creative-control values (read from APVTS each block) ───────────────────
struct MacroParams {
    // Drift Field morph pad
    float morphX = 0.5f, morphY = 0.5f;
    // Orbit nodes (distance from centre = amount, [0,1])
    float orbitMotion = 0.0f, orbitSpace = 0.0f, orbitTexture = 0.0f,
          orbitDrive  = 0.0f, orbitTone  = 0.0f;
    // Vibe console faders [0,1]
    float vibeDream = 0.0f, vibeWarm = 0.0f, vibeAir = 0.0f, vibeMove = 0.0f,
          vibeGrit  = 0.0f, vibeWide = 0.0f, vibeFocus = 0.0f;
};

// ── Adjustments the processor applies to the master/FX chain ───────────────────
// (FX parameters live outside PatchParams, so the macro layer returns deltas.)
struct MacroResult {
    float chorusDepthAdd = 0.0f;
    float chorusMixAdd   = 0.0f;
    float reverbMixAdd   = 0.0f;
    float reverbSizeAdd  = 0.0f;
    float delayMixAdd    = 0.0f;
    float driveAmountAdd = 0.0f;
    bool  forceChorusOn  = false;
    bool  forceReverbOn  = false;
    bool  forceDelayOn   = false;
    bool  forceDriveOn   = false;
    float stereoWidth    = 1.0f;  // master mid/side width (1 = neutral)
};

// Maps the creative-control surface onto the synthesis engine.
// `apply` rewrites the synth-side fields of `patch` in place and returns the
// master/FX deltas. Stateless — safe to call once per block on the audio thread.
class MacroEngine {
public:
    static MacroResult apply(PatchParams& patch, const MacroParams& m) noexcept;
};

} // namespace drift
