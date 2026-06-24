#include "MacroEngine.h"
#include <cmath>

namespace drift {

namespace {

// Accumulated, additive adjustments gathered from every control surface before
// being committed to the patch. All fields default to "no change".
struct Acc {
    float cutoffOct  = 0.0f;  // octaves added to both filter cutoffs
    float res        = 0.0f;  // filter resonance add
    float detune     = 0.0f;  // unison detune add
    float drift      = 0.0f;  // analog drift add
    float grit       = 0.0f;  // per-filter drive add
    float chorus     = 0.0f;  // chorus depth/mix add
    float reverb     = 0.0f;  // reverb mix add
    float reverbSize = 0.0f;  // reverb size add
    float delay      = 0.0f;  // delay mix add
    float drive      = 0.0f;  // master Drive FX amount add
    float ampRel     = 0.0f;  // amp release add (seconds)
    float attack     = 0.0f;  // amp attack add (seconds)
    float motion     = 0.0f;  // LFO1 → cutoff (octaves)
    float vibrato    = 0.0f;  // LFO1 → pitch (semitones)
    float width      = 0.0f;  // master stereo width add
};

// ── Morph-pad scenes ──────────────────────────────────────────────────────────
// Four corner "flavours". The pad bilinearly blends them and subtracts the
// centroid, so the centre (0.5,0.5) is neutral and each corner pushes character.
constexpr Acc kSceneVelvet { -0.70f, 0.00f, 0.05f, 0.30f, 0.00f,   // BL: warm/dark
                              0.00f, 0.35f, 0.15f, 0.00f, 0.00f,
                              1.20f, 0.00f, 0.00f, 0.00f, 0.20f };
constexpr Acc kSceneShadow { -0.35f, 0.05f, 0.00f, 0.10f, 0.50f,   // BR: gritty/dark
                              0.00f, 0.00f, 0.00f, 0.00f, 0.40f,
                              0.00f, 0.00f, 0.10f, 0.00f, 0.00f };
constexpr Acc kSceneGlass  {  1.10f, 0.12f, 0.00f, 0.00f, 0.00f,   // TL: bright/airy
                              0.00f, 0.20f, 0.05f, 0.00f, 0.00f,
                              0.00f, 0.00f, 0.00f, 0.00f, 0.25f };
constexpr Acc kSceneHollow {  0.35f, 0.20f, 0.25f, 0.00f, 0.00f,   // TR: detuned/hollow
                              0.22f, 0.00f, 0.00f, 0.00f, 0.00f,
                              0.00f, 0.00f, 0.22f, 0.00f, 0.05f };

inline void addScaled(Acc& a, const Acc& s, float g) noexcept {
    a.cutoffOct += s.cutoffOct * g; a.res    += s.res    * g;
    a.detune    += s.detune    * g; a.drift  += s.drift  * g;
    a.grit      += s.grit      * g; a.chorus += s.chorus * g;
    a.reverb    += s.reverb    * g; a.reverbSize += s.reverbSize * g;
    a.delay     += s.delay     * g; a.drive  += s.drive  * g;
    a.ampRel    += s.ampRel    * g; a.attack += s.attack * g;
    a.motion    += s.motion    * g; a.vibrato += s.vibrato * g;
    a.width     += s.width     * g;
}

} // namespace

MacroResult MacroEngine::apply(PatchParams& patch, const MacroParams& m) noexcept {
    Acc a{};

    // ── Drift Field morph pad (bilinear blend of 4 scenes, centroid-relative) ──
    const float x = clamp(m.morphX, 0.0f, 1.0f);
    const float y = clamp(m.morphY, 0.0f, 1.0f);
    const float wBL = (1.0f - x) * (1.0f - y);
    const float wBR = x * (1.0f - y);
    const float wTL = (1.0f - x) * y;
    const float wTR = x * y;
    addScaled(a, kSceneVelvet, wBL - 0.25f);
    addScaled(a, kSceneShadow, wBR - 0.25f);
    addScaled(a, kSceneGlass,  wTL - 0.25f);
    addScaled(a, kSceneHollow, wTR - 0.25f);

    // ── Orbit nodes ────────────────────────────────────────────────────────────
    a.motion += 0.70f * m.orbitMotion; a.chorus += 0.25f * m.orbitMotion;
    a.vibrato += 0.06f * m.orbitMotion;

    a.reverb += 0.55f * m.orbitSpace; a.reverbSize += 0.30f * m.orbitSpace;
    a.delay  += 0.35f * m.orbitSpace; a.width      += 0.35f * m.orbitSpace;

    a.drift  += 0.50f * m.orbitTexture; a.detune += 0.30f * m.orbitTexture;
    a.grit   += 0.20f * m.orbitTexture;

    a.drive  += 0.70f * m.orbitDrive; a.grit += 0.40f * m.orbitDrive;
    a.cutoffOct -= 0.10f * m.orbitDrive;

    a.cutoffOct += 1.00f * m.orbitTone;

    // ── Vibe console ───────────────────────────────────────────────────────────
    a.reverb += 0.50f * m.vibeDream; a.reverbSize += 0.25f * m.vibeDream;
    a.chorus += 0.40f * m.vibeDream; a.ampRel += 2.00f * m.vibeDream;
    a.attack += 0.15f * m.vibeDream; a.cutoffOct -= 0.30f * m.vibeDream;
    a.width  += 0.30f * m.vibeDream; a.motion += 0.08f * m.vibeDream;

    a.drift  += 0.45f * m.vibeWarm; a.cutoffOct -= 0.50f * m.vibeWarm;
    a.grit   += 0.20f * m.vibeWarm; a.detune += 0.15f * m.vibeWarm;
    a.reverb += 0.10f * m.vibeWarm;

    a.cutoffOct += 1.20f * m.vibeAir; a.reverb += 0.12f * m.vibeAir;
    a.width     += 0.20f * m.vibeAir;

    a.motion += 0.60f * m.vibeMove; a.chorus += 0.30f * m.vibeMove;
    a.detune += 0.10f * m.vibeMove; a.vibrato += 0.05f * m.vibeMove;

    a.grit   += 0.55f * m.vibeGrit; a.drive += 0.50f * m.vibeGrit;
    a.cutoffOct -= 0.10f * m.vibeGrit;

    a.width  += 0.80f * m.vibeWide; a.chorus += 0.15f * m.vibeWide;

    a.res    += 0.40f * m.vibeFocus; a.cutoffOct += 0.15f * m.vibeFocus;
    a.reverb -= 0.25f * m.vibeFocus; a.width -= 0.25f * m.vibeFocus;

    // ── Commit to the synth-side patch ─────────────────────────────────────────
    const float cutMul = std::exp2(a.cutoffOct);
    patch.filter1Cutoff = clamp(patch.filter1Cutoff * cutMul, kMinCutoffHz, kMaxCutoffHz);
    patch.filter2Cutoff = clamp(patch.filter2Cutoff * cutMul, kMinCutoffHz, kMaxCutoffHz);

    patch.filter1Res = clamp(patch.filter1Res + a.res, 0.0f, 1.0f);
    patch.filter2Res = clamp(patch.filter2Res + a.res, 0.0f, 1.0f);
    patch.filter1Drive = clamp(patch.filter1Drive + a.grit, 0.0f, 1.0f);
    patch.filter2Drive = clamp(patch.filter2Drive + a.grit, 0.0f, 1.0f);

    patch.unisonDetune = clamp(patch.unisonDetune + a.detune, 0.0f, 1.0f);
    patch.driftAmount  = clamp(patch.driftAmount  + a.drift,  0.0f, 1.5f);

    patch.ampEnv.releaseTime = clamp(patch.ampEnv.releaseTime + a.ampRel, 0.001f, 10.0f);
    patch.ampEnv.attackTime  = clamp(patch.ampEnv.attackTime  + a.attack, 0.001f,  5.0f);

    patch.macroMotionCutoff = clamp(a.motion,  0.0f, 2.5f);
    patch.macroVibrato      = clamp(a.vibrato, 0.0f, 1.0f);

    // ── Master / FX deltas ─────────────────────────────────────────────────────
    MacroResult r{};
    r.chorusDepthAdd = a.chorus;
    r.chorusMixAdd   = a.chorus;
    r.reverbMixAdd   = a.reverb;
    r.reverbSizeAdd  = a.reverbSize;
    r.delayMixAdd    = a.delay;
    r.driveAmountAdd = a.drive;
    r.forceChorusOn  = a.chorus > 0.02f;
    r.forceReverbOn  = a.reverb > 0.02f;
    r.forceDelayOn   = a.delay  > 0.02f;
    r.forceDriveOn   = a.drive  > 0.02f;
    r.stereoWidth    = clamp(1.0f + a.width, 0.0f, 2.0f);
    return r;
}

} // namespace drift
