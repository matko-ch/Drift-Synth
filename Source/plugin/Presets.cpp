#include "Presets.h"
#include "Parameters.h"

namespace drift {

namespace P = ParamID;

// Shape indices : 0 Sine · 1 Triangle · 2 Saw · 3 Square · 4 Pulse
// Filter types  : 0 LP12 · 1 HP12 · 2 BP12 · 3 Notch · 4 LP24 · 5 HP24 · 6 MoogLP
// LFO shapes    : 0 Sine · 1 Triangle · 2 Saw · 3 Square · 4 S&H

const std::vector<Preset>& getFactoryPresets() {
    static const std::vector<Preset> bank = {
        // ── INIT ──────────────────────────────────────────────────────────────
        { "Init Drift", "Basics", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 3 }, { P::UNISON_DETUNE, 0.28f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 2800.0f }, { P::FILT1_ENV_AMT, 0.35f },
            { P::DRIFT_AMOUNT, 0.35f },
        }},

        // ── LOFI / CHILLHOP ─────────────────────────────────────────────────────
        { "Lofi Dreams", "Lofi", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, -1 },
            { P::OSC_MIX, 0.4f }, { P::UNISON_VOICES, 2 }, { P::UNISON_DETUNE, 0.22f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 1400.0f }, { P::FILT1_RES, 0.15f },
            { P::FILT1_ENV_AMT, 0.25f }, { P::FILTENV_D, 0.6f },
            { P::AMPENV_A, 0.03f }, { P::AMPENV_D, 0.8f }, { P::AMPENV_S, 0.7f }, { P::AMPENV_R, 0.9f },
            { P::DRIFT_AMOUNT, 0.7f },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.32f }, { P::REVERB_SIZE, 0.6f },
            { P::VIBE_WARM, 0.5f },
        }},

        // ── SYNTHWAVE ───────────────────────────────────────────────────────────
        { "Synthwave Brass", "Synthwave", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 8.0f },
            { P::OSC_MIX, 0.5f }, { P::UNISON_VOICES, 5 }, { P::UNISON_DETUNE, 0.35f }, { P::UNISON_SPREAD, 0.7f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 2200.0f }, { P::FILT1_RES, 0.2f },
            { P::FILT1_ENV_AMT, 0.45f }, { P::FILTENV_A, 0.02f }, { P::FILTENV_D, 0.5f }, { P::FILTENV_S, 0.4f },
            { P::AMPENV_A, 0.04f }, { P::AMPENV_D, 0.6f }, { P::AMPENV_S, 0.8f }, { P::AMPENV_R, 0.5f },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.25f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.37f }, { P::DELAY_MIX, 0.25f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.22f },
            { P::DRIFT_AMOUNT, 0.3f },
        }},

        // ── AMBIENT ─────────────────────────────────────────────────────────────
        { "Ambient Drift Pad", "Ambient", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 1 }, { P::OSC2_OCTAVE, 1 },
            { P::OSC_MIX, 0.45f }, { P::UNISON_VOICES, 6 }, { P::UNISON_DETUNE, 0.4f }, { P::UNISON_SPREAD, 0.9f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 1800.0f }, { P::FILT1_RES, 0.1f }, { P::FILT1_ENV_AMT, 0.2f },
            { P::AMPENV_A, 1.2f }, { P::AMPENV_D, 1.0f }, { P::AMPENV_S, 0.9f }, { P::AMPENV_R, 2.5f },
            { P::DRIFT_AMOUNT, 0.6f },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.45f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.5f }, { P::REVERB_SIZE, 0.9f }, { P::REVERB_DAMP, 0.4f },
            { P::ORBIT_SPACE, 0.5f }, { P::ORBIT_MOTION, 0.3f }, { P::VIBE_DREAM, 0.5f },
        }},

        // ── HOUSE / EDM ─────────────────────────────────────────────────────────
        { "House Pluck", "House", {
            { P::OSC1_SHAPE, 3 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 6.0f },
            { P::OSC_MIX, 0.4f }, { P::UNISON_VOICES, 3 }, { P::UNISON_DETUNE, 0.25f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 900.0f }, { P::FILT1_RES, 0.35f },
            { P::FILT1_ENV_AMT, 0.6f }, { P::FILTENV_A, 0.001f }, { P::FILTENV_D, 0.25f }, { P::FILTENV_S, 0.1f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.4f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 0.3f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.25f }, { P::DELAY_MIX, 0.25f }, { P::DELAY_FDBK, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.2f },
            { P::DRIFT_AMOUNT, 0.2f },
        }},

        // ── TRAP ────────────────────────────────────────────────────────────────
        { "Trap Bell", "Trap", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 1 }, { P::OSC2_OCTAVE, 1 }, { P::OSC2_FINE, 4.0f },
            { P::OSC_MIX, 0.4f }, { P::UNISON_VOICES, 2 }, { P::UNISON_DETUNE, 0.18f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 6000.0f }, { P::FILT1_RES, 0.05f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 1.2f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 1.0f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.3f }, { P::DELAY_MIX, 0.3f }, { P::DELAY_FDBK, 0.45f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.4f }, { P::REVERB_SIZE, 0.8f },
            { P::DRIFT_AMOUNT, 0.25f },
        }},

        // ── DRUM & BASS ─────────────────────────────────────────────────────────
        { "Reese Bass", "Drum & Bass", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, -14.0f },
            { P::OSC_MIX, 0.5f }, { P::UNISON_VOICES, 4 }, { P::UNISON_DETUNE, 0.45f }, { P::UNISON_SPREAD, 0.3f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 600.0f }, { P::FILT1_RES, 0.3f },
            { P::AMPENV_A, 0.005f }, { P::AMPENV_D, 0.5f }, { P::AMPENV_S, 0.9f }, { P::AMPENV_R, 0.2f },
            { P::LFO1_SHAPE, 0 }, { P::LFO1_RATE, 0.3f },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.4f },
            { P::DRIFT_AMOUNT, 0.3f },
            { P::ORBIT_MOTION, 0.4f }, { P::VIBE_GRIT, 0.3f },
        }},

        // ── VAPORWAVE ───────────────────────────────────────────────────────────
        { "Vapor Keys", "Vaporwave", {
            { P::OSC1_SHAPE, 4 }, { P::OSC1_PW, 0.3f }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 10.0f },
            { P::OSC_MIX, 0.5f }, { P::UNISON_VOICES, 4 }, { P::UNISON_DETUNE, 0.35f }, { P::UNISON_SPREAD, 0.8f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 2400.0f }, { P::FILT1_RES, 0.15f }, { P::FILT1_ENV_AMT, 0.2f },
            { P::AMPENV_A, 0.02f }, { P::AMPENV_D, 0.8f }, { P::AMPENV_S, 0.7f }, { P::AMPENV_R, 1.2f },
            { P::DRIFT_AMOUNT, 0.55f },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.5f }, { P::CHORUS_DEPTH, 0.6f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.4f }, { P::REVERB_SIZE, 0.75f },
            { P::VIBE_WIDE, 0.5f }, { P::VIBE_DREAM, 0.3f },
        }},

        // ── SYNTHPOP LEAD ───────────────────────────────────────────────────────
        { "Neon Lead", "Pop", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 3 }, { P::OSC2_FINE, 5.0f },
            { P::OSC_MIX, 0.4f }, { P::UNISON_VOICES, 3 }, { P::UNISON_DETUNE, 0.2f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 4000.0f }, { P::FILT1_RES, 0.18f }, { P::FILT1_KEYTRACK, 0.5f },
            { P::AMPENV_A, 0.005f }, { P::AMPENV_D, 0.4f }, { P::AMPENV_S, 0.85f }, { P::AMPENV_R, 0.4f },
            { P::GLIDE_ON, 1 }, { P::GLIDE_TIME, 0.06f }, { P::POLY_VOICES, 1 },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.33f }, { P::DELAY_MIX, 0.28f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.2f },
            { P::DRIFT_AMOUNT, 0.3f },
        }},

        // ── DEEP SUB BASS ───────────────────────────────────────────────────────
        { "Deep Sub", "Bass", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 1 }, { P::OSC2_OCTAVE, 0 },
            { P::OSC_MIX, 0.25f }, { P::UNISON_VOICES, 1 },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 700.0f }, { P::FILT1_RES, 0.05f },
            { P::AMPENV_A, 0.004f }, { P::AMPENV_D, 0.3f }, { P::AMPENV_S, 0.95f }, { P::AMPENV_R, 0.15f },
            { P::POLY_VOICES, 1 }, { P::GLIDE_ON, 1 }, { P::GLIDE_TIME, 0.04f },
            { P::DRIFT_AMOUNT, 0.12f },
            { P::CHORUS_ON, 0 }, { P::REVERB_ON, 0 },
        }},

        // ── CINEMATIC ───────────────────────────────────────────────────────────
        { "Cinematic Swell", "Cinematic", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_OCTAVE, -1 },
            { P::OSC_MIX, 0.5f }, { P::UNISON_VOICES, 7 }, { P::UNISON_DETUNE, 0.5f }, { P::UNISON_SPREAD, 1.0f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 1200.0f }, { P::FILT1_RES, 0.12f }, { P::FILT1_ENV_AMT, 0.4f },
            { P::FILTENV_A, 1.5f }, { P::FILTENV_D, 1.0f },
            { P::AMPENV_A, 1.8f }, { P::AMPENV_D, 1.5f }, { P::AMPENV_S, 0.85f }, { P::AMPENV_R, 3.0f },
            { P::DRIFT_AMOUNT, 0.5f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.55f }, { P::REVERB_SIZE, 1.0f },
            { P::ORBIT_SPACE, 0.6f }, { P::ORBIT_MOTION, 0.4f }, { P::VIBE_DREAM, 0.6f },
        }},

        // ── EXPERIMENTAL / GLASS ────────────────────────────────────────────────
        { "Glass Bells", "Experimental", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_SEMITONE, 7 }, { P::OSC2_FINE, 2.0f },
            { P::OSC_MIX, 0.5f }, { P::UNISON_VOICES, 2 }, { P::UNISON_DETUNE, 0.15f },
            { P::FILT1_TYPE, 2 }, { P::FILT1_CUTOFF, 5000.0f }, { P::FILT1_RES, 0.3f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 1.5f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 1.5f },
            { P::DRIFT_AMOUNT, 0.4f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.4f }, { P::DELAY_MIX, 0.35f }, { P::DELAY_FDBK, 0.5f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.5f }, { P::REVERB_SIZE, 0.85f },
            { P::MORPH_X, 0.15f }, { P::MORPH_Y, 0.9f }, { P::ORBIT_TONE, 0.4f },
        }},

        // ── DARK DRONE ──────────────────────────────────────────────────────────
        { "Shadow Drone", "Dark", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 3 }, { P::OSC2_OCTAVE, -1 }, { P::OSC2_FINE, -6.0f },
            { P::OSC_MIX, 0.5f }, { P::UNISON_VOICES, 5 }, { P::UNISON_DETUNE, 0.4f }, { P::UNISON_SPREAD, 0.6f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 800.0f }, { P::FILT1_RES, 0.25f },
            { P::AMPENV_A, 2.0f }, { P::AMPENV_D, 1.0f }, { P::AMPENV_S, 0.9f }, { P::AMPENV_R, 3.0f },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.45f },
            { P::DRIFT_AMOUNT, 0.6f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.45f }, { P::REVERB_SIZE, 0.95f }, { P::REVERB_DAMP, 0.7f },
            { P::MORPH_X, 0.85f }, { P::MORPH_Y, 0.1f }, { P::ORBIT_DRIVE, 0.4f }, { P::VIBE_GRIT, 0.4f },
        }},

        // ── DREAM POP ───────────────────────────────────────────────────────────
        { "Dreampop Shimmer", "Dream Pop", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 1 }, { P::OSC2_OCTAVE, 1 }, { P::OSC2_FINE, 7.0f },
            { P::OSC_MIX, 0.45f }, { P::UNISON_VOICES, 4 }, { P::UNISON_DETUNE, 0.3f }, { P::UNISON_SPREAD, 0.85f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 3200.0f }, { P::FILT1_RES, 0.1f }, { P::FILT1_ENV_AMT, 0.25f },
            { P::AMPENV_A, 0.3f }, { P::AMPENV_D, 0.8f }, { P::AMPENV_S, 0.8f }, { P::AMPENV_R, 1.8f },
            { P::DRIFT_AMOUNT, 0.45f },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.5f }, { P::CHORUS_DEPTH, 0.55f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.45f }, { P::DELAY_MIX, 0.3f }, { P::DELAY_FDBK, 0.45f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.45f }, { P::REVERB_SIZE, 0.85f },
            { P::ORBIT_SPACE, 0.4f }, { P::VIBE_AIR, 0.4f }, { P::VIBE_DREAM, 0.4f },
        }},

        // ── BASS ──────────────────────────────────────────────────────────────────
        { "808 Sub Mono", "Bass", {
            { P::OSC1_SHAPE, 0 }, { P::OSC_MIX, 0.0f }, { P::UNISON_VOICES, 1 },
            { P::SUB_LEVEL, 0.6f }, { P::SUB_SHAPE, 0 }, { P::SUB_OCTAVE, -1 },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 500.0f },
            { P::AMPENV_A, 0.002f }, { P::AMPENV_D, 0.6f }, { P::AMPENV_S, 0.7f }, { P::AMPENV_R, 0.4f },
            { P::POLY_VOICES, 1 }, { P::GLIDE_ON, 1 }, { P::GLIDE_TIME, 0.05f },
            { P::DRIFT_AMOUNT, 0.1f }, { P::CHORUS_ON, 0 }, { P::REVERB_ON, 0 },
        }},
        { "Pulse Bass", "Bass", {
            { P::OSC1_SHAPE, 4 }, { P::OSC1_PW, 0.25f }, { P::OSC2_SHAPE, 2 }, { P::OSC2_OCTAVE, -1 },
            { P::OSC_MIX, 0.4f }, { P::SUB_LEVEL, 0.3f }, { P::SUB_SHAPE, 2 },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 800.0f }, { P::FILT1_RES, 0.3f }, { P::FILT1_ENV_AMT, 0.4f },
            { P::FILTENV_D, 0.2f }, { P::FILTENV_S, 0.1f },
            { P::AMPENV_A, 0.002f }, { P::AMPENV_D, 0.3f }, { P::AMPENV_S, 0.8f }, { P::AMPENV_R, 0.15f },
            { P::POLY_VOICES, 1 }, { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.3f }, { P::DRIFT_AMOUNT, 0.15f },
        }},

        // ── ACID / TECHNO ──────────────────────────────────────────────────────────
        { "Acid 303", "Acid", {
            { P::OSC1_SHAPE, 2 }, { P::OSC_MIX, 0.0f }, { P::UNISON_VOICES, 1 },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 500.0f }, { P::FILT1_RES, 0.7f }, { P::FILT1_ENV_AMT, 0.7f },
            { P::FILTENV_A, 0.001f }, { P::FILTENV_D, 0.25f }, { P::FILTENV_S, 0.0f },
            { P::AMPENV_A, 0.002f }, { P::AMPENV_D, 0.4f }, { P::AMPENV_S, 0.6f }, { P::AMPENV_R, 0.1f },
            { P::POLY_VOICES, 1 }, { P::GLIDE_ON, 1 }, { P::GLIDE_TIME, 0.06f },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.4f }, { P::DELAY_ON, 1 }, { P::DELAY_MIX, 0.2f },
        }},
        { "Techno Stab", "Techno", {
            { P::OSC1_SHAPE, 3 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, -7.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 3 }, { P::UNISON_DETUNE, 0.3f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 1200.0f }, { P::FILT1_RES, 0.4f }, { P::FILT1_ENV_AMT, 0.3f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.25f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 0.2f },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.35f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.2f },
            { P::EQ_ON, 1 }, { P::EQ_LOW, 3.0f }, { P::EQ_HIGH, 2.0f },
        }},

        // ── TRANCE / FUTURE BASS ───────────────────────────────────────────────────
        { "Supersaw Trance", "Trance", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 12.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 7 }, { P::UNISON_DETUNE, 0.4f }, { P::UNISON_SPREAD, 0.9f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 3000.0f }, { P::FILT1_RES, 0.2f }, { P::FILT1_ENV_AMT, 0.3f },
            { P::AMPENV_A, 0.01f }, { P::AMPENV_D, 0.5f }, { P::AMPENV_S, 0.8f }, { P::AMPENV_R, 0.5f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.37f }, { P::DELAY_MIX, 0.25f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f }, { P::VIBE_WIDE, 0.5f }, { P::DRIFT_AMOUNT, 0.25f },
        }},
        { "Future Bass Chord", "Future Bass", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 7.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 6 }, { P::UNISON_DETUNE, 0.45f }, { P::UNISON_SPREAD, 1.0f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 4000.0f }, { P::FILT1_KEYTRACK, 0.4f },
            { P::AMPENV_A, 0.01f }, { P::AMPENV_D, 0.6f }, { P::AMPENV_S, 0.7f }, { P::AMPENV_R, 0.6f },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.4f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f },
            { P::ORBIT_MOTION, 0.5f }, { P::VIBE_WIDE, 0.6f }, { P::VIBE_AIR, 0.3f },
        }},

        // ── FM / RING (experimental) ───────────────────────────────────────────────
        { "FM Glass Bell", "Experimental", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_SEMITONE, 7 },
            { P::OSC_MIX, 0.2f }, { P::OSC_FM, 0.5f }, { P::UNISON_VOICES, 1 },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 6000.0f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 1.4f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 1.2f },
            { P::DELAY_ON, 1 }, { P::DELAY_MIX, 0.3f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.45f }, { P::REVERB_SIZE, 0.85f },
            { P::DRIFT_AMOUNT, 0.3f },
        }},
        { "Ring Metallic", "Experimental", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 3 }, { P::OSC2_SEMITONE, 5 }, { P::OSC2_FINE, 3.0f },
            { P::OSC_RING, 0.8f }, { P::OSC_MIX, 0.5f },
            { P::FILT1_TYPE, 2 }, { P::FILT1_CUTOFF, 3500.0f }, { P::FILT1_RES, 0.3f },
            { P::AMPENV_A, 0.002f }, { P::AMPENV_D, 0.8f }, { P::AMPENV_S, 0.2f }, { P::AMPENV_R, 0.8f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.4f }, { P::PHASER_ON, 1 }, { P::PHASER_MIX, 0.3f },
            { P::DRIFT_AMOUNT, 0.4f },
        }},
        { "FM Growl Bass", "Experimental", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, 1 }, { P::OSC_FM, 0.7f },
            { P::OSC_MIX, 0.3f }, { P::SUB_LEVEL, 0.4f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 700.0f }, { P::FILT1_RES, 0.3f },
            { P::LFO1_RATE, 6.0f }, { P::AMPENV_A, 0.003f }, { P::AMPENV_S, 0.9f },
            { P::POLY_VOICES, 1 }, { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.5f },
            { P::ORBIT_MOTION, 0.5f }, { P::VIBE_GRIT, 0.4f },
        }},

        // ── LOFI / CHIPTUNE (crusher) ──────────────────────────────────────────────
        { "8-Bit Lead", "Chiptune", {
            { P::OSC1_SHAPE, 4 }, { P::OSC1_PW, 0.5f }, { P::OSC_MIX, 0.0f }, { P::UNISON_VOICES, 1 },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 8000.0f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.2f }, { P::AMPENV_S, 0.7f }, { P::AMPENV_R, 0.1f },
            { P::CRUSH_ON, 1 }, { P::CRUSH_BITS, 6.0f }, { P::CRUSH_AMT, 0.4f }, { P::CRUSH_MIX, 0.8f },
            { P::POLY_VOICES, 1 }, { P::DRIFT_AMOUNT, 0.05f }, { P::CHORUS_ON, 0 }, { P::REVERB_ON, 0 },
        }},
        { "Vinyl Lofi Keys", "Lofi", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, -1 }, { P::OSC_MIX, 0.35f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 1600.0f }, { P::FILT1_RES, 0.1f },
            { P::AMPENV_A, 0.02f }, { P::AMPENV_D, 0.7f }, { P::AMPENV_S, 0.6f }, { P::AMPENV_R, 0.8f },
            { P::DRIFT_AMOUNT, 0.8f }, { P::CRUSH_ON, 1 }, { P::CRUSH_BITS, 11.0f }, { P::CRUSH_AMT, 0.25f }, { P::CRUSH_MIX, 0.5f },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.3f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f },
            { P::EQ_ON, 1 }, { P::EQ_HIGH, -4.0f }, { P::EQ_LOW, 2.0f }, { P::VIBE_WARM, 0.5f },
        }},
        { "Crushed Glitch Stab", "Glitch", {
            { P::OSC1_SHAPE, 3 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_SEMITONE, 7 }, { P::OSC_RING, 0.5f },
            { P::OSC_MIX, 0.5f }, { P::OSC_NOISE, 0.15f },
            { P::FILT1_TYPE, 2 }, { P::FILT1_CUTOFF, 2500.0f }, { P::FILT1_RES, 0.4f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.15f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 0.1f },
            { P::CRUSH_ON, 1 }, { P::CRUSH_BITS, 5.0f }, { P::CRUSH_AMT, 0.6f }, { P::CRUSH_MIX, 0.7f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.18f }, { P::DELAY_FDBK, 0.5f }, { P::DELAY_MIX, 0.35f },
        }},

        // ── HYPERPOP / POP ─────────────────────────────────────────────────────────
        { "Hyperpop Saw", "Hyperpop", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 15.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 5 }, { P::UNISON_DETUNE, 0.35f }, { P::UNISON_SPREAD, 0.7f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 6000.0f }, { P::FILT1_KEYTRACK, 0.5f },
            { P::AMPENV_A, 0.003f }, { P::AMPENV_D, 0.3f }, { P::AMPENV_S, 0.85f }, { P::AMPENV_R, 0.3f },
            { P::GLIDE_ON, 1 }, { P::GLIDE_TIME, 0.05f }, { P::POLY_VOICES, 1 },
            { P::CRUSH_ON, 1 }, { P::CRUSH_BITS, 12.0f }, { P::CRUSH_AMT, 0.15f }, { P::CRUSH_MIX, 0.3f },
            { P::DELAY_ON, 1 }, { P::DELAY_MIX, 0.25f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.25f },
        }},

        // ── AMBIENT / CINEMATIC (phaser, motion) ──────────────────────────────────
        { "Phaser Dream Pad", "Ambient", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_OCTAVE, 1 }, { P::OSC2_FINE, 6.0f },
            { P::OSC_MIX, 0.5f }, { P::UNISON_VOICES, 6 }, { P::UNISON_DETUNE, 0.4f }, { P::UNISON_SPREAD, 1.0f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 1800.0f }, { P::FILT1_RES, 0.15f },
            { P::AMPENV_A, 1.5f }, { P::AMPENV_D, 1.0f }, { P::AMPENV_S, 0.9f }, { P::AMPENV_R, 3.0f },
            { P::DRIFT_AMOUNT, 0.6f },
            { P::PHASER_ON, 1 }, { P::PHASER_RATE, 0.15f }, { P::PHASER_DEPTH, 0.7f }, { P::PHASER_MIX, 0.5f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.5f }, { P::REVERB_SIZE, 0.95f },
            { P::ORBIT_SPACE, 0.6f }, { P::VIBE_DREAM, 0.6f },
        }},
        { "Cosmic Noise Drone", "Drone", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 3 }, { P::OSC2_OCTAVE, -1 }, { P::OSC_MIX, 0.5f },
            { P::OSC_NOISE, 0.12f }, { P::UNISON_VOICES, 4 }, { P::UNISON_DETUNE, 0.5f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 900.0f }, { P::FILT1_RES, 0.2f },
            { P::AMPENV_A, 2.5f }, { P::AMPENV_S, 0.9f }, { P::AMPENV_R, 4.0f },
            { P::DRIFT_AMOUNT, 0.7f }, { P::PHASER_ON, 1 }, { P::PHASER_RATE, 0.08f }, { P::PHASER_MIX, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.55f }, { P::REVERB_SIZE, 1.0f }, { P::REVERB_DAMP, 0.6f },
            { P::MORPH_X, 0.8f }, { P::MORPH_Y, 0.1f }, { P::ORBIT_SPACE, 0.6f },
        }},
        { "Choir Formant Pad", "Cinematic", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 4 }, { P::OSC2_PW, 0.35f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 5 }, { P::UNISON_DETUNE, 0.35f }, { P::UNISON_SPREAD, 0.9f },
            { P::FILT1_TYPE, 2 }, { P::FILT1_CUTOFF, 1400.0f }, { P::FILT1_RES, 0.5f }, { P::FILT1_ENV_AMT, 0.25f },
            { P::AMPENV_A, 0.8f }, { P::AMPENV_D, 1.0f }, { P::AMPENV_S, 0.85f }, { P::AMPENV_R, 2.0f },
            { P::LFO1_RATE, 4.5f }, { P::DRIFT_AMOUNT, 0.5f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.5f }, { P::REVERB_SIZE, 0.9f },
            { P::ORBIT_MOTION, 0.4f }, { P::VIBE_AIR, 0.4f }, { P::VIBE_DREAM, 0.4f },
        }},

        // ── KEYS / PLUCK ───────────────────────────────────────────────────────────
        { "Warm FM Rhodes", "Keys", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, 1 }, { P::OSC_FM, 0.25f },
            { P::OSC_MIX, 0.3f }, { P::UNISON_VOICES, 1 },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 3000.0f }, { P::FILT1_ENV_AMT, 0.3f }, { P::FILT1_KEYTRACK, 0.4f },
            { P::FILTENV_D, 0.5f }, { P::FILTENV_S, 0.2f },
            { P::AMPENV_A, 0.004f }, { P::AMPENV_D, 0.8f }, { P::AMPENV_S, 0.4f }, { P::AMPENV_R, 0.5f },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.3f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.25f },
            { P::DRIFT_AMOUNT, 0.25f }, { P::VIBE_WARM, 0.3f },
        }},
        { "Glassy Pluck", "Pluck", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, 1 }, { P::OSC2_FINE, 4.0f },
            { P::OSC_MIX, 0.35f }, { P::UNISON_VOICES, 2 }, { P::UNISON_DETUNE, 0.2f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 1500.0f }, { P::FILT1_RES, 0.3f }, { P::FILT1_ENV_AMT, 0.6f },
            { P::FILTENV_A, 0.001f }, { P::FILTENV_D, 0.3f }, { P::FILTENV_S, 0.0f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.5f }, { P::AMPENV_S, 0.1f }, { P::AMPENV_R, 0.4f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.3f }, { P::DELAY_MIX, 0.3f }, { P::DELAY_FDBK, 0.45f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f }, { P::VIBE_AIR, 0.3f },
        }},
        { "Shimmer Octave Pad", "Ambient", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, 2 }, { P::OSC_MIX, 0.4f },
            { P::UNISON_VOICES, 4 }, { P::UNISON_DETUNE, 0.3f }, { P::UNISON_SPREAD, 1.0f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 2600.0f }, { P::FILT1_RES, 0.1f },
            { P::AMPENV_A, 1.0f }, { P::AMPENV_D, 1.0f }, { P::AMPENV_S, 0.85f }, { P::AMPENV_R, 3.0f },
            { P::DRIFT_AMOUNT, 0.5f }, { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.4f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.5f }, { P::DELAY_FDBK, 0.55f }, { P::DELAY_MIX, 0.3f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.55f }, { P::REVERB_SIZE, 1.0f },
            { P::ORBIT_SPACE, 0.7f }, { P::VIBE_AIR, 0.5f }, { P::VIBE_DREAM, 0.5f },
        }},

        // ── ARPEGGIATED ────────────────────────────────────────────────────────────
        { "Arp Sunrise", "Arp", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 3 }, { P::OSC2_FINE, 5.0f }, { P::OSC_MIX, 0.4f },
            { P::UNISON_VOICES, 2 }, { P::UNISON_DETUNE, 0.2f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 2200.0f }, { P::FILT1_RES, 0.25f }, { P::FILT1_ENV_AMT, 0.4f },
            { P::FILTENV_D, 0.2f }, { P::FILTENV_S, 0.1f },
            { P::AMPENV_A, 0.002f }, { P::AMPENV_D, 0.3f }, { P::AMPENV_S, 0.2f }, { P::AMPENV_R, 0.3f },
            { P::ARP_ON, 1 }, { P::ARP_RATE, 5 }, { P::ARP_MODE, 0 }, { P::ARP_OCT, 2 }, { P::ARP_GATE, 0.6f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.25f }, { P::DELAY_MIX, 0.3f }, { P::DELAY_FDBK, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f },
        }},
        { "Arp Dark Pulse", "Arp", {
            { P::OSC1_SHAPE, 3 }, { P::OSC_MIX, 0.0f }, { P::SUB_LEVEL, 0.3f }, { P::SUB_SHAPE, 2 },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 900.0f }, { P::FILT1_RES, 0.4f }, { P::FILT1_ENV_AMT, 0.4f },
            { P::FILTENV_D, 0.15f }, { P::FILTENV_S, 0.0f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.2f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 0.1f },
            { P::ARP_ON, 1 }, { P::ARP_RATE, 5 }, { P::ARP_MODE, 2 }, { P::ARP_OCT, 1 }, { P::ARP_GATE, 0.4f },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.3f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.25f },
        }},
        { "Arp Crystal Rain", "Arp", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, 1 }, { P::OSC_FM, 0.3f }, { P::OSC_MIX, 0.3f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 5000.0f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.5f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 0.6f },
            { P::ARP_ON, 1 }, { P::ARP_RATE, 6 }, { P::ARP_MODE, 5 }, { P::ARP_OCT, 3 }, { P::ARP_GATE, 0.5f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.33f }, { P::DELAY_FDBK, 0.55f }, { P::DELAY_MIX, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.45f }, { P::REVERB_SIZE, 0.9f }, { P::VIBE_AIR, 0.4f },
        }},
        { "Arp Bass Sequence", "Arp", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, -10.0f }, { P::OSC_MIX, 0.5f },
            { P::SUB_LEVEL, 0.25f }, { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 1100.0f }, { P::FILT1_RES, 0.3f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.25f }, { P::AMPENV_S, 0.3f }, { P::AMPENV_R, 0.1f },
            { P::ARP_ON, 1 }, { P::ARP_RATE, 5 }, { P::ARP_MODE, 1 }, { P::ARP_OCT, 1 }, { P::ARP_GATE, 0.7f },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.25f }, { P::DRIFT_AMOUNT, 0.2f },
        }},

        // ── CHORD STABS ─────────────────────────────────────────────────────────────
        { "Chord House Stab", "Chords", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 3 }, { P::OSC2_FINE, 6.0f }, { P::OSC_MIX, 0.45f },
            { P::UNISON_VOICES, 3 }, { P::UNISON_DETUNE, 0.25f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 1800.0f }, { P::FILT1_RES, 0.3f }, { P::FILT1_ENV_AMT, 0.4f },
            { P::FILTENV_D, 0.2f }, { P::FILTENV_S, 0.1f },
            { P::AMPENV_A, 0.002f }, { P::AMPENV_D, 0.3f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 0.25f },
            { P::CHORD_ON, 1 }, { P::CHORD_TYPE, 7 /*Min7*/ },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.25f }, { P::DELAY_ON, 1 }, { P::DELAY_MIX, 0.2f },
        }},
        { "Chord Lush Maj7", "Chords", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 4.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 4 }, { P::UNISON_DETUNE, 0.3f }, { P::UNISON_SPREAD, 0.8f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 3000.0f }, { P::FILT1_RES, 0.1f },
            { P::AMPENV_A, 0.05f }, { P::AMPENV_D, 0.7f }, { P::AMPENV_S, 0.7f }, { P::AMPENV_R, 1.2f },
            { P::CHORD_ON, 1 }, { P::CHORD_TYPE, 6 /*Maj7*/ },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.4f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.4f },
            { P::VIBE_DREAM, 0.4f }, { P::VIBE_WIDE, 0.4f }, { P::DRIFT_AMOUNT, 0.35f },
        }},
        { "Chord Power Lead", "Chords", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 3 }, { P::OSC2_OCTAVE, -1 }, { P::OSC_MIX, 0.4f },
            { P::UNISON_VOICES, 4 }, { P::UNISON_DETUNE, 0.3f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 2800.0f }, { P::FILT1_RES, 0.2f },
            { P::AMPENV_A, 0.005f }, { P::AMPENV_D, 0.4f }, { P::AMPENV_S, 0.8f }, { P::AMPENV_R, 0.4f },
            { P::CHORD_ON, 1 }, { P::CHORD_TYPE, 14 /*Power5*/ },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.4f }, { P::DELAY_ON, 1 }, { P::DELAY_MIX, 0.25f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.2f },
        }},
        { "Chord Arp Dream", "Chords", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, 1 }, { P::OSC_MIX, 0.35f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 2400.0f }, { P::FILT1_RES, 0.15f },
            { P::AMPENV_A, 0.002f }, { P::AMPENV_D, 0.4f }, { P::AMPENV_S, 0.1f }, { P::AMPENV_R, 0.4f },
            { P::CHORD_ON, 1 }, { P::CHORD_TYPE, 12 /*Min9*/ },
            { P::ARP_ON, 1 }, { P::ARP_RATE, 5 }, { P::ARP_MODE, 0 }, { P::ARP_OCT, 1 }, { P::ARP_GATE, 0.5f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.375f }, { P::DELAY_FDBK, 0.45f }, { P::DELAY_MIX, 0.35f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.4f }, { P::VIBE_DREAM, 0.5f },
        }},

        // ── LEADS ───────────────────────────────────────────────────────────────────
        { "Screaming Lead", "Lead", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 10.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 4 }, { P::UNISON_DETUNE, 0.3f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 3500.0f }, { P::FILT1_RES, 0.35f }, { P::FILT1_KEYTRACK, 0.5f },
            { P::AMPENV_A, 0.004f }, { P::AMPENV_S, 0.85f }, { P::AMPENV_R, 0.3f },
            { P::POLY_VOICES, 1 }, { P::GLIDE_ON, 1 }, { P::GLIDE_TIME, 0.04f },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.4f }, { P::DELAY_ON, 1 }, { P::DELAY_MIX, 0.3f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.2f },
        }},
        { "Soft Sine Lead", "Lead", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 1 }, { P::OSC2_FINE, 3.0f }, { P::OSC_MIX, 0.35f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 4000.0f },
            { P::AMPENV_A, 0.02f }, { P::AMPENV_S, 0.9f }, { P::AMPENV_R, 0.4f },
            { P::POLY_VOICES, 1 }, { P::GLIDE_ON, 1 }, { P::GLIDE_TIME, 0.08f }, { P::DRIFT_AMOUNT, 0.3f },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.3f }, { P::DELAY_ON, 1 }, { P::DELAY_MIX, 0.25f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f },
        }},
        { "Pluck Lead Stack", "Lead", {
            { P::OSC1_SHAPE, 3 }, { P::OSC1_PW, 0.4f }, { P::OSC2_SHAPE, 2 }, { P::OSC2_OCTAVE, 1 }, { P::OSC_MIX, 0.4f },
            { P::UNISON_VOICES, 3 }, { P::UNISON_DETUNE, 0.2f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 2000.0f }, { P::FILT1_RES, 0.3f }, { P::FILT1_ENV_AMT, 0.5f },
            { P::FILTENV_D, 0.25f }, { P::FILTENV_S, 0.1f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.4f }, { P::AMPENV_S, 0.3f }, { P::AMPENV_R, 0.3f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.3f }, { P::DELAY_MIX, 0.3f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.25f },
        }},

        // ── DUBSTEP / BASS DESIGN ───────────────────────────────────────────────────
        { "Wobble Bass", "Dubstep", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 3 }, { P::OSC2_FINE, -12.0f }, { P::OSC_MIX, 0.5f },
            { P::SUB_LEVEL, 0.4f }, { P::OSC_FM, 0.2f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 600.0f }, { P::FILT1_RES, 0.45f },
            { P::AMPENV_A, 0.003f }, { P::AMPENV_S, 0.9f },
            { P::POLY_VOICES, 1 }, { P::ORBIT_MOTION, 0.7f }, { P::VIBE_MOVE, 0.6f },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.5f }, { P::VIBE_GRIT, 0.4f },
        }},
        { "Neuro Bass", "Dubstep", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 7.0f }, { P::OSC_FM, 0.4f }, { P::OSC_RING, 0.2f },
            { P::OSC_MIX, 0.5f }, { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 800.0f }, { P::FILT1_RES, 0.4f },
            { P::AMPENV_A, 0.002f }, { P::AMPENV_S, 0.85f }, { P::POLY_VOICES, 1 },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.6f }, { P::PHASER_ON, 1 }, { P::PHASER_RATE, 0.5f }, { P::PHASER_MIX, 0.3f },
            { P::ORBIT_DRIVE, 0.5f }, { P::ORBIT_MOTION, 0.4f },
        }},

        // ── WORLD / PLUCKED ─────────────────────────────────────────────────────────
        { "Koto Pluck", "World", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, 1 }, { P::OSC_MIX, 0.3f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 3000.0f }, { P::FILT1_ENV_AMT, 0.4f },
            { P::FILTENV_A, 0.001f }, { P::FILTENV_D, 0.2f }, { P::FILTENV_S, 0.0f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.6f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 0.5f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.28f }, { P::DELAY_MIX, 0.25f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.35f }, { P::DRIFT_AMOUNT, 0.3f },
        }},
        { "Marimba Soft", "World", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, 1 }, { P::OSC_FM, 0.15f }, { P::OSC_MIX, 0.2f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 3500.0f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.5f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f }, { P::REVERB_SIZE, 0.7f },
        }},

        // ── RETRO / GAME ────────────────────────────────────────────────────────────
        { "Game Boy Arp", "Chiptune", {
            { P::OSC1_SHAPE, 3 }, { P::OSC_MIX, 0.0f }, { P::UNISON_VOICES, 1 },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 8000.0f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.1f }, { P::AMPENV_S, 0.6f }, { P::AMPENV_R, 0.05f },
            { P::ARP_ON, 1 }, { P::ARP_RATE, 7 }, { P::ARP_MODE, 0 }, { P::ARP_OCT, 2 }, { P::ARP_GATE, 0.5f },
            { P::CRUSH_ON, 1 }, { P::CRUSH_BITS, 4.0f }, { P::CRUSH_AMT, 0.3f }, { P::CRUSH_MIX, 0.9f },
            { P::POLY_VOICES, 1 },
        }},
        { "Retro PWM Pad", "Retro", {
            { P::OSC1_SHAPE, 4 }, { P::OSC1_PW, 0.3f }, { P::OSC2_SHAPE, 4 }, { P::OSC2_PW, 0.6f }, { P::OSC2_FINE, 6.0f },
            { P::OSC_MIX, 0.5f }, { P::UNISON_VOICES, 2 }, { P::UNISON_DETUNE, 0.2f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 2200.0f }, { P::FILT1_RES, 0.15f },
            { P::AMPENV_A, 0.4f }, { P::AMPENV_D, 0.8f }, { P::AMPENV_S, 0.8f }, { P::AMPENV_R, 1.0f },
            { P::LFO1_RATE, 2.0f }, { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.5f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f }, { P::DRIFT_AMOUNT, 0.4f },
        }},

        // ── EXPERIMENTAL II ─────────────────────────────────────────────────────────
        { "Broken Radio", "Experimental", {
            { P::OSC1_SHAPE, 2 }, { P::OSC_NOISE, 0.2f }, { P::OSC_MIX, 0.0f },
            { P::FILT1_TYPE, 2 }, { P::FILT1_CUTOFF, 1500.0f }, { P::FILT1_RES, 0.6f },
            { P::AMPENV_A, 0.05f }, { P::AMPENV_S, 0.7f }, { P::AMPENV_R, 0.5f },
            { P::CRUSH_ON, 1 }, { P::CRUSH_BITS, 7.0f }, { P::CRUSH_AMT, 0.5f }, { P::CRUSH_MIX, 0.6f },
            { P::PHASER_ON, 1 }, { P::PHASER_RATE, 0.3f }, { P::PHASER_MIX, 0.5f },
            { P::EQ_ON, 1 }, { P::EQ_LOW, -6.0f }, { P::EQ_MID, 4.0f }, { P::EQ_HIGH, -3.0f },
        }},
        { "Detuned Nightmare", "Experimental", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 33.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 6 }, { P::UNISON_DETUNE, 0.7f }, { P::UNISON_SPREAD, 1.0f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 1200.0f }, { P::FILT1_RES, 0.3f },
            { P::AMPENV_A, 1.0f }, { P::AMPENV_S, 0.9f }, { P::AMPENV_R, 2.5f },
            { P::DRIFT_AMOUNT, 1.0f }, { P::PHASER_ON, 1 }, { P::PHASER_MIX, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.5f }, { P::REVERB_SIZE, 1.0f },
            { P::MORPH_X, 0.9f }, { P::MORPH_Y, 0.9f },
        }},
        { "Ring Wash Texture", "Experimental", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_SEMITONE, 11 }, { P::OSC_RING, 0.6f }, { P::OSC_MIX, 0.5f },
            { P::OSC_NOISE, 0.08f }, { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 2500.0f },
            { P::AMPENV_A, 1.5f }, { P::AMPENV_S, 0.8f }, { P::AMPENV_R, 3.0f },
            { P::ORBIT_SPACE, 0.6f }, { P::ORBIT_TEXTURE, 0.5f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.55f }, { P::REVERB_SIZE, 1.0f }, { P::DELAY_ON, 1 }, { P::DELAY_MIX, 0.3f },
        }},

        // ── MORE GENRES ──────────────────────────────────────────────────────────────
        { "Tropical Pluck", "Pop", {
            { P::OSC1_SHAPE, 1 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, 1 }, { P::OSC_MIX, 0.3f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 3200.0f }, { P::FILT1_ENV_AMT, 0.3f },
            { P::FILTENV_D, 0.2f }, { P::FILTENV_S, 0.1f },
            { P::AMPENV_A, 0.002f }, { P::AMPENV_D, 0.45f }, { P::AMPENV_S, 0.1f }, { P::AMPENV_R, 0.3f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.27f }, { P::DELAY_MIX, 0.3f }, { P::DELAY_FDBK, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.25f }, { P::VIBE_AIR, 0.3f },
        }},
        { "RnB Keys", "Keys", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_OCTAVE, -1 }, { P::OSC_FM, 0.2f }, { P::OSC_MIX, 0.3f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 2600.0f }, { P::FILT1_ENV_AMT, 0.2f },
            { P::AMPENV_A, 0.005f }, { P::AMPENV_D, 0.7f }, { P::AMPENV_S, 0.5f }, { P::AMPENV_R, 0.6f },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.35f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f },
            { P::EQ_ON, 1 }, { P::EQ_LOW, 2.0f }, { P::DRIFT_AMOUNT, 0.25f }, { P::VIBE_WARM, 0.3f },
        }},
        { "Gospel Organ", "Keys", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, 1 }, { P::OSC_MIX, 0.5f },
            { P::SUB_LEVEL, 0.3f }, { P::SUB_SHAPE, 0 },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 4000.0f },
            { P::AMPENV_A, 0.005f }, { P::AMPENV_S, 0.95f }, { P::AMPENV_R, 0.2f },
            { P::LFO1_RATE, 6.0f }, { P::PHASER_ON, 1 }, { P::PHASER_RATE, 5.0f }, { P::PHASER_DEPTH, 0.5f }, { P::PHASER_MIX, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f },
        }},
        { "Trance Pluck Gate", "Trance", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 8.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 5 }, { P::UNISON_DETUNE, 0.3f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 2500.0f }, { P::FILT1_RES, 0.25f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.2f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 0.15f },
            { P::ARP_ON, 1 }, { P::ARP_RATE, 5 }, { P::ARP_MODE, 0 }, { P::ARP_OCT, 1 }, { P::ARP_GATE, 0.45f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.375f }, { P::DELAY_MIX, 0.3f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f },
        }},
        { "Ambient Guitar-ish", "Ambient", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_FINE, 4.0f }, { P::OSC_MIX, 0.3f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 2200.0f }, { P::FILT1_ENV_AMT, 0.2f },
            { P::AMPENV_A, 0.4f }, { P::AMPENV_D, 1.0f }, { P::AMPENV_S, 0.4f }, { P::AMPENV_R, 2.5f },
            { P::DRIFT_AMOUNT, 0.4f }, { P::PHASER_ON, 1 }, { P::PHASER_RATE, 0.2f }, { P::PHASER_MIX, 0.4f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.5f }, { P::DELAY_FDBK, 0.6f }, { P::DELAY_MIX, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.6f }, { P::REVERB_SIZE, 1.0f }, { P::ORBIT_SPACE, 0.7f },
        }},
        { "Future Garage Pad", "Garage", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 4 }, { P::OSC2_PW, 0.4f }, { P::OSC_MIX, 0.45f },
            { P::UNISON_VOICES, 3 }, { P::UNISON_DETUNE, 0.25f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 1600.0f }, { P::FILT1_RES, 0.2f },
            { P::AMPENV_A, 0.3f }, { P::AMPENV_D, 0.8f }, { P::AMPENV_S, 0.7f }, { P::AMPENV_R, 1.5f },
            { P::DRIFT_AMOUNT, 0.5f }, { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.45f }, { P::REVERB_SIZE, 0.9f }, { P::VIBE_WARM, 0.4f },
        }},
        { "Hard Techno Hoover", "Techno", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 3 }, { P::OSC2_FINE, -5.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 4 }, { P::UNISON_DETUNE, 0.4f }, { P::OSC_FM, 0.15f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 1400.0f }, { P::FILT1_RES, 0.35f }, { P::FILT1_ENV_AMT, 0.3f },
            { P::AMPENV_A, 0.005f }, { P::AMPENV_S, 0.85f }, { P::AMPENV_R, 0.3f },
            { P::GLIDE_ON, 1 }, { P::GLIDE_TIME, 0.08f }, { P::POLY_VOICES, 1 },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.45f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.2f },
        }},
        { "Sad Lofi Chord", "Lofi", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, -1 }, { P::OSC_MIX, 0.35f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 1300.0f }, { P::FILT1_RES, 0.1f },
            { P::AMPENV_A, 0.03f }, { P::AMPENV_D, 0.8f }, { P::AMPENV_S, 0.6f }, { P::AMPENV_R, 0.9f },
            { P::CHORD_ON, 1 }, { P::CHORD_TYPE, 7 /*Min7*/ }, { P::DRIFT_AMOUNT, 0.7f },
            { P::CRUSH_ON, 1 }, { P::CRUSH_BITS, 10.0f }, { P::CRUSH_AMT, 0.3f }, { P::CRUSH_MIX, 0.5f },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.3f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.35f },
            { P::EQ_ON, 1 }, { P::EQ_HIGH, -5.0f }, { P::VIBE_WARM, 0.5f },
        }},
        { "Big Room Supersaw", "EDM", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 14.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 8 }, { P::UNISON_DETUNE, 0.45f }, { P::UNISON_SPREAD, 1.0f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 5000.0f }, { P::FILT1_KEYTRACK, 0.4f },
            { P::AMPENV_A, 0.01f }, { P::AMPENV_S, 0.9f }, { P::AMPENV_R, 0.5f },
            { P::EQ_ON, 1 }, { P::EQ_LOW, 2.0f }, { P::EQ_HIGH, 3.0f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.3f }, { P::VIBE_WIDE, 0.6f }, { P::VIBE_AIR, 0.4f },
        }},
        { "Plucky Square Bass", "Bass", {
            { P::OSC1_SHAPE, 3 }, { P::OSC_MIX, 0.0f }, { P::SUB_LEVEL, 0.4f }, { P::SUB_SHAPE, 0 },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 900.0f }, { P::FILT1_RES, 0.25f }, { P::FILT1_ENV_AMT, 0.4f },
            { P::FILTENV_D, 0.15f }, { P::FILTENV_S, 0.0f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.3f }, { P::AMPENV_S, 0.4f }, { P::AMPENV_R, 0.15f },
            { P::POLY_VOICES, 1 }, { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.3f }, { P::DRIFT_AMOUNT, 0.15f },
        }},
        { "Celestial Choir", "Cinematic", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 4 }, { P::OSC2_PW, 0.3f }, { P::OSC2_OCTAVE, 1 }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 6 }, { P::UNISON_DETUNE, 0.4f }, { P::UNISON_SPREAD, 1.0f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 2400.0f }, { P::FILT1_RES, 0.1f },
            { P::AMPENV_A, 1.2f }, { P::AMPENV_D, 1.0f }, { P::AMPENV_S, 0.9f }, { P::AMPENV_R, 3.5f },
            { P::DRIFT_AMOUNT, 0.5f }, { P::ORBIT_SPACE, 0.7f }, { P::ORBIT_MOTION, 0.3f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.6f }, { P::REVERB_SIZE, 1.0f }, { P::VIBE_DREAM, 0.6f }, { P::VIBE_AIR, 0.5f },
        }},
        { "Mono Acid Stab", "Acid", {
            { P::OSC1_SHAPE, 4 }, { P::OSC1_PW, 0.3f }, { P::OSC_MIX, 0.0f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 700.0f }, { P::FILT1_RES, 0.6f }, { P::FILT1_ENV_AMT, 0.6f },
            { P::FILTENV_D, 0.2f }, { P::FILTENV_S, 0.0f },
            { P::AMPENV_A, 0.002f }, { P::AMPENV_D, 0.3f }, { P::AMPENV_S, 0.3f }, { P::AMPENV_R, 0.1f },
            { P::POLY_VOICES, 1 }, { P::GLIDE_ON, 1 }, { P::GLIDE_TIME, 0.05f },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.5f }, { P::DELAY_ON, 1 }, { P::DELAY_MIX, 0.25f }, { P::DELAY_FDBK, 0.5f },
        }},
        { "Warm Analog Strings", "Strings", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 6.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 5 }, { P::UNISON_DETUNE, 0.3f }, { P::UNISON_SPREAD, 0.8f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 2000.0f }, { P::FILT1_RES, 0.1f }, { P::FILT1_ENV_AMT, 0.2f },
            { P::AMPENV_A, 0.5f }, { P::AMPENV_D, 0.6f }, { P::AMPENV_S, 0.85f }, { P::AMPENV_R, 1.0f },
            { P::DRIFT_AMOUNT, 0.4f }, { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.45f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.35f }, { P::VIBE_WARM, 0.4f },
        }},
        { "Icy Bell Arp", "Arp", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_SEMITONE, 12 }, { P::OSC_FM, 0.2f }, { P::OSC_MIX, 0.3f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 6000.0f },
            { P::AMPENV_A, 0.001f }, { P::AMPENV_D, 0.4f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 0.5f },
            { P::ARP_ON, 1 }, { P::ARP_RATE, 6 }, { P::ARP_MODE, 2 }, { P::ARP_OCT, 2 }, { P::ARP_GATE, 0.4f },
            { P::DELAY_ON, 1 }, { P::DELAY_TIME, 0.2f }, { P::DELAY_FDBK, 0.5f }, { P::DELAY_MIX, 0.35f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.5f }, { P::REVERB_SIZE, 0.9f }, { P::VIBE_AIR, 0.5f },
        }},
        { "Dusty Tape Pad", "Lofi", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 1 }, { P::OSC2_OCTAVE, -1 }, { P::OSC_MIX, 0.4f }, { P::OSC_NOISE, 0.05f },
            { P::FILT1_TYPE, 6 }, { P::FILT1_CUTOFF, 1100.0f }, { P::FILT1_RES, 0.1f },
            { P::AMPENV_A, 0.4f }, { P::AMPENV_D, 0.9f }, { P::AMPENV_S, 0.7f }, { P::AMPENV_R, 1.4f },
            { P::DRIFT_AMOUNT, 0.85f }, { P::CRUSH_ON, 1 }, { P::CRUSH_BITS, 9.0f }, { P::CRUSH_AMT, 0.2f }, { P::CRUSH_MIX, 0.4f },
            { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.4f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.4f },
            { P::EQ_ON, 1 }, { P::EQ_HIGH, -6.0f }, { P::EQ_LOW, 2.0f },
        }},
        { "Sci-Fi Drone Bed", "Drone", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 2 }, { P::OSC2_FINE, 2.0f }, { P::OSC_RING, 0.3f }, { P::OSC_MIX, 0.5f },
            { P::OSC_NOISE, 0.1f }, { P::FILT1_TYPE, 2 }, { P::FILT1_CUTOFF, 1000.0f }, { P::FILT1_RES, 0.4f },
            { P::AMPENV_A, 3.0f }, { P::AMPENV_S, 0.9f }, { P::AMPENV_R, 4.0f },
            { P::LFO1_RATE, 0.2f }, { P::ORBIT_MOTION, 0.5f }, { P::ORBIT_SPACE, 0.6f },
            { P::PHASER_ON, 1 }, { P::PHASER_RATE, 0.1f }, { P::PHASER_MIX, 0.5f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.55f }, { P::REVERB_SIZE, 1.0f }, { P::REVERB_DAMP, 0.5f },
        }},
        { "Punchy Synth Brass", "Pop", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 3 }, { P::OSC2_FINE, 5.0f }, { P::OSC_MIX, 0.45f },
            { P::UNISON_VOICES, 3 }, { P::UNISON_DETUNE, 0.25f },
            { P::FILT1_TYPE, 4 }, { P::FILT1_CUTOFF, 1800.0f }, { P::FILT1_RES, 0.2f }, { P::FILT1_ENV_AMT, 0.5f },
            { P::FILTENV_A, 0.02f }, { P::FILTENV_D, 0.3f }, { P::FILTENV_S, 0.5f },
            { P::AMPENV_A, 0.01f }, { P::AMPENV_D, 0.4f }, { P::AMPENV_S, 0.8f }, { P::AMPENV_R, 0.3f },
            { P::DRIVE_ON, 1 }, { P::DRIVE_AMOUNT, 0.25f }, { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.2f },
            { P::EQ_ON, 1 }, { P::EQ_MID, 3.0f },
        }},
        { "Whale Song Pad", "Ambient", {
            { P::OSC1_SHAPE, 0 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_FINE, 8.0f }, { P::OSC_MIX, 0.5f },
            { P::UNISON_VOICES, 3 }, { P::UNISON_DETUNE, 0.3f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 1500.0f },
            { P::AMPENV_A, 2.0f }, { P::AMPENV_S, 0.9f }, { P::AMPENV_R, 4.0f },
            { P::MODENV_A, 1.0f }, { P::LFO1_RATE, 0.15f },
            { P::ORBIT_MOTION, 0.5f }, { P::ORBIT_SPACE, 0.7f }, { P::VIBE_MOVE, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.6f }, { P::REVERB_SIZE, 1.0f },
        }},
        { "Stab Orchestra Hit", "Cinematic", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 3 }, { P::OSC2_OCTAVE, -1 }, { P::OSC_MIX, 0.5f }, { P::OSC_NOISE, 0.1f },
            { P::UNISON_VOICES, 4 }, { P::UNISON_DETUNE, 0.4f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 2200.0f }, { P::FILT1_ENV_AMT, 0.3f },
            { P::FILTENV_A, 0.001f }, { P::FILTENV_D, 0.3f }, { P::FILTENV_S, 0.0f },
            { P::AMPENV_A, 0.002f }, { P::AMPENV_D, 0.6f }, { P::AMPENV_S, 0.0f }, { P::AMPENV_R, 0.5f },
            { P::CHORD_ON, 1 }, { P::CHORD_TYPE, 1 /*Minor*/ },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.4f }, { P::REVERB_SIZE, 0.9f },
        }},
        { "Liquid DnB Pad", "Drum & Bass", {
            { P::OSC1_SHAPE, 2 }, { P::OSC2_SHAPE, 0 }, { P::OSC2_OCTAVE, 1 }, { P::OSC2_FINE, 6.0f }, { P::OSC_MIX, 0.4f },
            { P::UNISON_VOICES, 4 }, { P::UNISON_DETUNE, 0.3f }, { P::UNISON_SPREAD, 0.9f },
            { P::FILT1_TYPE, 0 }, { P::FILT1_CUTOFF, 2800.0f }, { P::FILT1_RES, 0.12f },
            { P::AMPENV_A, 0.2f }, { P::AMPENV_D, 0.6f }, { P::AMPENV_S, 0.8f }, { P::AMPENV_R, 1.2f },
            { P::DRIFT_AMOUNT, 0.4f }, { P::CHORUS_ON, 1 }, { P::CHORUS_MIX, 0.4f },
            { P::REVERB_ON, 1 }, { P::REVERB_MIX, 0.4f }, { P::DELAY_ON, 1 }, { P::DELAY_MIX, 0.25f }, { P::VIBE_AIR, 0.35f },
        }},
    };
    return bank;
}

void applyPreset(juce::AudioProcessor& processor,
                 juce::AudioProcessorValueTreeState& apvts,
                 const Preset& preset)
{
    // 1) Reset everything to default so presets are fully reproducible.
    for (auto* ap : processor.getParameters())
        ap->setValueNotifyingHost(ap->getDefaultValue());

    // 2) Apply the preset's overrides (real-world values → normalised).
    for (const auto& pp : preset.params)
        if (auto* rp = apvts.getParameter(pp.id))
            rp->setValueNotifyingHost(rp->convertTo0to1(pp.value));
}

} // namespace drift
