#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "../dsp/Constants.h"
#include "../dsp/Filter.h"
#include "../dsp/LFO.h"
#include "../dsp/ModMatrix.h"
#include "../dsp/Chords.h"

namespace drift {

// All parameter IDs — single source of truth.
namespace ParamID {
// Oscillator 1
inline constexpr auto OSC1_SHAPE    = "osc1_shape";
inline constexpr auto OSC1_OCTAVE   = "osc1_octave";
inline constexpr auto OSC1_SEMITONE = "osc1_semitone";
inline constexpr auto OSC1_FINE     = "osc1_fine";
inline constexpr auto OSC1_LEVEL    = "osc1_level";
inline constexpr auto OSC1_PW       = "osc1_pw";
inline constexpr auto OSC1_PAN      = "osc1_pan";

// Oscillator 2
inline constexpr auto OSC2_SHAPE    = "osc2_shape";
inline constexpr auto OSC2_OCTAVE   = "osc2_octave";
inline constexpr auto OSC2_SEMITONE = "osc2_semitone";
inline constexpr auto OSC2_FINE     = "osc2_fine";
inline constexpr auto OSC2_LEVEL    = "osc2_level";
inline constexpr auto OSC2_PW       = "osc2_pw";
inline constexpr auto OSC2_PAN      = "osc2_pan";
inline constexpr auto OSC2_SYNC     = "osc2_sync";

// Mix
inline constexpr auto OSC_MIX       = "osc_mix";

// Oscillator extras (more sonic range)
inline constexpr auto OSC_FM     = "osc_fm";     // osc2 → osc1 frequency mod
inline constexpr auto OSC_RING   = "osc_ring";   // ring modulation blend
inline constexpr auto OSC_NOISE  = "osc_noise";  // white-noise layer
inline constexpr auto SUB_LEVEL  = "sub_level";
inline constexpr auto SUB_SHAPE  = "sub_shape";
inline constexpr auto SUB_OCTAVE = "sub_octave"; // -2 or -1

// Unison
inline constexpr auto UNISON_VOICES  = "unison_voices";
inline constexpr auto UNISON_DETUNE  = "unison_detune";
inline constexpr auto UNISON_SPREAD  = "unison_spread";

// Filter 1
inline constexpr auto FILT1_CUTOFF   = "filt1_cutoff";
inline constexpr auto FILT1_RES      = "filt1_res";
inline constexpr auto FILT1_DRIVE    = "filt1_drive";
inline constexpr auto FILT1_TYPE     = "filt1_type";
inline constexpr auto FILT1_ENV_AMT  = "filt1_env_amt";
inline constexpr auto FILT1_KEYTRACK = "filt1_keytrack";

// Filter 2
inline constexpr auto FILT2_CUTOFF   = "filt2_cutoff";
inline constexpr auto FILT2_RES      = "filt2_res";
inline constexpr auto FILT2_DRIVE    = "filt2_drive";
inline constexpr auto FILT2_TYPE     = "filt2_type";
inline constexpr auto FILT2_ENV_AMT  = "filt2_env_amt";
inline constexpr auto FILT2_KEYTRACK = "filt2_keytrack";

inline constexpr auto FILT_ROUTING   = "filt_routing";  // 0=serial, 1=parallel

// Amp Envelope (DAHDSR + curve)
inline constexpr auto AMPENV_A = "ampenv_a";
inline constexpr auto AMPENV_H = "ampenv_h";
inline constexpr auto AMPENV_D = "ampenv_d";
inline constexpr auto AMPENV_S = "ampenv_s";
inline constexpr auto AMPENV_R = "ampenv_r";
inline constexpr auto AMPENV_CURVE = "ampenv_curve";

// Filter Envelope
inline constexpr auto FILTENV_A = "filtenv_a";
inline constexpr auto FILTENV_H = "filtenv_h";
inline constexpr auto FILTENV_D = "filtenv_d";
inline constexpr auto FILTENV_S = "filtenv_s";
inline constexpr auto FILTENV_R = "filtenv_r";
inline constexpr auto FILTENV_CURVE = "filtenv_curve";

// Mod Envelope
inline constexpr auto MODENV_A = "modenv_a";
inline constexpr auto MODENV_H = "modenv_h";
inline constexpr auto MODENV_D = "modenv_d";
inline constexpr auto MODENV_S = "modenv_s";
inline constexpr auto MODENV_R = "modenv_r";
inline constexpr auto MODENV_CURVE = "modenv_curve";

// LFO 1
inline constexpr auto LFO1_SHAPE   = "lfo1_shape";
inline constexpr auto LFO1_RATE    = "lfo1_rate";
inline constexpr auto LFO1_DEPTH   = "lfo1_depth";
inline constexpr auto LFO1_RETRIG  = "lfo1_retrig";

// LFO 2
inline constexpr auto LFO2_SHAPE   = "lfo2_shape";
inline constexpr auto LFO2_RATE    = "lfo2_rate";
inline constexpr auto LFO2_DEPTH   = "lfo2_depth";
inline constexpr auto LFO2_RETRIG  = "lfo2_retrig";

// Mod Matrix (8 slots × source/dest/amount)
// Generated: mod0_src, mod0_dst, mod0_amt ... mod7_src, mod7_dst, mod7_amt
inline constexpr const char* MOD_SRC[kNumModSlots] = {
    "mod0_src","mod1_src","mod2_src","mod3_src",
    "mod4_src","mod5_src","mod6_src","mod7_src"
};
inline constexpr const char* MOD_DST[kNumModSlots] = {
    "mod0_dst","mod1_dst","mod2_dst","mod3_dst",
    "mod4_dst","mod5_dst","mod6_dst","mod7_dst"
};
inline constexpr const char* MOD_AMT[kNumModSlots] = {
    "mod0_amt","mod1_amt","mod2_amt","mod3_amt",
    "mod4_amt","mod5_amt","mod6_amt","mod7_amt"
};

// Effects
inline constexpr auto DRIVE_ON     = "drive_on";
inline constexpr auto DRIVE_AMOUNT = "drive_amount";
inline constexpr auto DRIVE_MIX    = "drive_mix";

inline constexpr auto CHORUS_ON    = "chorus_on";
inline constexpr auto CHORUS_RATE  = "chorus_rate";
inline constexpr auto CHORUS_DEPTH = "chorus_depth";
inline constexpr auto CHORUS_MIX   = "chorus_mix";

inline constexpr auto DELAY_ON     = "delay_on";
inline constexpr auto DELAY_TIME   = "delay_time";
inline constexpr auto DELAY_FDBK   = "delay_fdbk";
inline constexpr auto DELAY_MIX    = "delay_mix";

inline constexpr auto REVERB_ON    = "reverb_on";
inline constexpr auto REVERB_SIZE  = "reverb_size";
inline constexpr auto REVERB_DAMP  = "reverb_damp";
inline constexpr auto REVERB_MIX   = "reverb_mix";

inline constexpr auto EQ_ON        = "eq_on";
inline constexpr auto EQ_LOW       = "eq_low";
inline constexpr auto EQ_MID       = "eq_mid";
inline constexpr auto EQ_HIGH      = "eq_high";

inline constexpr auto PHASER_ON    = "phaser_on";
inline constexpr auto PHASER_RATE  = "phaser_rate";
inline constexpr auto PHASER_DEPTH = "phaser_depth";
inline constexpr auto PHASER_MIX   = "phaser_mix";

inline constexpr auto CRUSH_ON     = "crush_on";
inline constexpr auto CRUSH_BITS   = "crush_bits";
inline constexpr auto CRUSH_AMT    = "crush_amt";
inline constexpr auto CRUSH_MIX    = "crush_mix";

// Master
inline constexpr auto MASTER_VOL   = "master_vol";
inline constexpr auto MASTER_TUNE  = "master_tune";
inline constexpr auto POLY_VOICES  = "poly_voices";
inline constexpr auto GLIDE_TIME   = "glide_time";
inline constexpr auto GLIDE_ON     = "glide_on";
inline constexpr auto PITCH_BEND_RANGE = "pb_range";

// Character
inline constexpr auto DRIFT_AMOUNT = "drift_amount";

// Arpeggiator
inline constexpr auto ARP_ON   = "arp_on";
inline constexpr auto ARP_RATE = "arp_rate";
inline constexpr auto ARP_MODE = "arp_mode";
inline constexpr auto ARP_OCT  = "arp_oct";
inline constexpr auto ARP_GATE = "arp_gate";

// Chord mode
inline constexpr auto CHORD_ON   = "chord_on";
inline constexpr auto CHORD_TYPE = "chord_type";

// ── Macro / creative-control layer ────────────────────────────────────────────
// Drift Field morph pad (bilinear blend of 4 sonic "scenes")
inline constexpr auto MORPH_X = "morph_x";
inline constexpr auto MORPH_Y = "morph_y";

// Orbit nodes — distance from centre = amount [0,1]
inline constexpr auto ORBIT_MOTION  = "orbit_motion";
inline constexpr auto ORBIT_SPACE   = "orbit_space";
inline constexpr auto ORBIT_TEXTURE = "orbit_texture";
inline constexpr auto ORBIT_DRIVE   = "orbit_drive";
inline constexpr auto ORBIT_TONE    = "orbit_tone";

// Vibe console — semantic faders [0,1]
inline constexpr auto VIBE_DREAM = "vibe_dream";
inline constexpr auto VIBE_WARM  = "vibe_warm";
inline constexpr auto VIBE_AIR   = "vibe_air";
inline constexpr auto VIBE_MOVE  = "vibe_move";
inline constexpr auto VIBE_GRIT  = "vibe_grit";
inline constexpr auto VIBE_WIDE  = "vibe_wide";
inline constexpr auto VIBE_FOCUS = "vibe_focus";
} // namespace ParamID

// ── Layout factory ────────────────────────────────────────────────────────────

inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    using namespace juce;
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    auto addFloat = [&](const char* id, const char* name,
                        float lo, float hi, float def,
                        std::function<String(float, int)> textFn = nullptr,
                        std::function<float(const String&)> fromTextFn = nullptr)
    {
        auto attr = AudioParameterFloatAttributes{};
        if (textFn)       attr = attr.withStringFromValueFunction(textFn);
        if (fromTextFn)   attr = attr.withValueFromStringFunction(fromTextFn);
        params.push_back(std::make_unique<AudioParameterFloat>(
            ParameterID{id, 1}, name, NormalisableRange<float>{lo, hi}, def, attr));
    };

    auto addInt = [&](const char* id, const char* name, int lo, int hi, int def) {
        params.push_back(std::make_unique<AudioParameterInt>(
            ParameterID{id, 1}, name, lo, hi, def));
    };

    // On/off switches are modelled as a 2-step integer so their value snaps
    // cleanly to 0/1 and survives host state round-trips exactly (verified by
    // pluginval strictness-10 "state restoration"). Button attachments treat
    // any 0..1 parameter as a toggle, so behaviour is unchanged.
    auto addBool = [&](const char* id, const char* name, bool def) {
        params.push_back(std::make_unique<AudioParameterInt>(
            ParameterID{id, 1}, name, 0, 1, def ? 1 : 0));
    };

    auto addChoice = [&](const char* id, const char* name,
                         StringArray choices, int def = 0) {
        params.push_back(std::make_unique<AudioParameterChoice>(
            ParameterID{id, 1}, name, choices, def));
    };

    const StringArray oscShapes {"Sine","Triangle","Saw","Square","Pulse"};
    const StringArray filtTypes {"LP12","HP12","BP12","Notch","LP24","HP24","MoogLP"};
    const StringArray lfoShapes {"Sine","Triangle","Saw","Square","S&H"};
    const StringArray modSources{"None","AmpEnv","FilterEnv","ModEnv","LFO1","LFO2",
                                  "Velocity","ModWheel","KeyTrack","Aftertouch"};
    const StringArray modDests  {"None","MasterPitch","Osc1Pitch","Osc2Pitch",
                                  "Osc1PW","Osc2PW","OscMix","Filt1Cut","Filt1Res",
                                  "Filt2Cut","Filt2Res","AmpVol","Pan","LFO1Rate","LFO2Rate"};

    // Osc 1
    addChoice(ParamID::OSC1_SHAPE, "Osc1 Shape", oscShapes, 2 /*Saw*/);
    addInt   (ParamID::OSC1_OCTAVE,"Osc1 Octave",  -2, 2, 0);
    addInt   (ParamID::OSC1_SEMITONE,"Osc1 Semitone", -12, 12, 0);
    addFloat (ParamID::OSC1_FINE,  "Osc1 Fine",   -100.0f, 100.0f, 0.0f);
    addFloat (ParamID::OSC1_LEVEL, "Osc1 Level",    0.0f, 1.0f, 1.0f);
    addFloat (ParamID::OSC1_PW,    "Osc1 PW",       0.05f, 0.95f, 0.5f);
    addFloat (ParamID::OSC1_PAN,   "Osc1 Pan",      -1.0f, 1.0f, 0.0f);

    // Osc 2
    addChoice(ParamID::OSC2_SHAPE, "Osc2 Shape", oscShapes, 2);
    addInt   (ParamID::OSC2_OCTAVE,"Osc2 Octave",  -2, 2, 0);
    addInt   (ParamID::OSC2_SEMITONE,"Osc2 Semitone", -12, 12, 0);
    addFloat (ParamID::OSC2_FINE,  "Osc2 Fine",   -100.0f, 100.0f, 0.0f);
    addFloat (ParamID::OSC2_LEVEL, "Osc2 Level",    0.0f, 1.0f, 1.0f);
    addFloat (ParamID::OSC2_PW,    "Osc2 PW",       0.05f, 0.95f, 0.5f);
    addFloat (ParamID::OSC2_PAN,   "Osc2 Pan",      -1.0f, 1.0f, 0.0f);
    addBool  (ParamID::OSC2_SYNC,  "Osc2 Sync",     false);
    addFloat (ParamID::OSC_MIX,    "Osc Mix",        0.0f, 1.0f, 0.5f);

    // Oscillator extras
    const StringArray subShapes { "Sine", "Triangle", "Square" };
    addFloat (ParamID::OSC_FM,    "FM Amount",  0.0f, 1.0f, 0.0f);
    addFloat (ParamID::OSC_RING,  "Ring Mod",   0.0f, 1.0f, 0.0f);
    addFloat (ParamID::OSC_NOISE, "Noise",      0.0f, 1.0f, 0.0f);
    addFloat (ParamID::SUB_LEVEL, "Sub",        0.0f, 1.0f, 0.0f);
    addChoice(ParamID::SUB_SHAPE, "Sub Shape",  subShapes, 2 /*Square*/);
    addInt   (ParamID::SUB_OCTAVE,"Sub Octave", -2, -1, -1);

    // Unison
    addInt   (ParamID::UNISON_VOICES, "Unison Voices", 1, kMaxUnisonVoices, 3);
    addFloat (ParamID::UNISON_DETUNE, "Unison Detune", 0.0f, 1.0f, 0.28f);
    addFloat (ParamID::UNISON_SPREAD, "Unison Spread", 0.0f, 1.0f, 0.6f);

    // Filter 1
    {
        auto range = NormalisableRange<float>{kMinCutoffHz, kMaxCutoffHz,
            [](float lo, float hi, float v) { return lo * std::pow(hi/lo, v); },
            [](float lo, float hi, float v) { return std::log(v/lo) / std::log(hi/lo); }};
        params.push_back(std::make_unique<AudioParameterFloat>(
            ParameterID{ParamID::FILT1_CUTOFF, 1}, "Filt1 Cutoff", range, 2800.0f));
    }
    addFloat (ParamID::FILT1_RES,      "Filt1 Res",     0.0f, 1.0f, 0.12f);
    addFloat (ParamID::FILT1_DRIVE,    "Filt1 Drive",   0.0f, 1.0f, 0.0f);
    addChoice(ParamID::FILT1_TYPE,     "Filt1 Type", filtTypes, 6 /*MoogLP*/);
    addFloat (ParamID::FILT1_ENV_AMT,  "Filt1 Env Amt", -1.0f, 1.0f, 0.35f);
    addFloat (ParamID::FILT1_KEYTRACK, "Filt1 KeyTrk",   0.0f, 1.0f, 0.35f);

    // Filter 2
    {
        auto range = NormalisableRange<float>{kMinCutoffHz, kMaxCutoffHz,
            [](float lo, float hi, float v) { return lo * std::pow(hi/lo, v); },
            [](float lo, float hi, float v) { return std::log(v/lo) / std::log(hi/lo); }};
        params.push_back(std::make_unique<AudioParameterFloat>(
            ParameterID{ParamID::FILT2_CUTOFF, 1}, "Filt2 Cutoff", range, 8000.0f));
    }
    addFloat (ParamID::FILT2_RES,      "Filt2 Res",     0.0f, 1.0f, 0.0f);
    addFloat (ParamID::FILT2_DRIVE,    "Filt2 Drive",   0.0f, 1.0f, 0.0f);
    addChoice(ParamID::FILT2_TYPE,     "Filt2 Type", filtTypes, 0);
    addFloat (ParamID::FILT2_ENV_AMT,  "Filt2 Env Amt", -1.0f, 1.0f, 0.0f);
    addFloat (ParamID::FILT2_KEYTRACK, "Filt2 KeyTrk",   0.0f, 1.0f, 0.0f);
    addBool  (ParamID::FILT_ROUTING,   "Filter Routing", false);

    // Envelopes
    auto envRange = [](float lo, float hi) {
        return NormalisableRange<float>{lo, hi,
            [](float l, float h, float v) { return l * std::pow(h/l, v); },
            [](float l, float h, float v) { return std::log(v/l) / std::log(h/l); }};
    };

    // Full DAHDSR + curve per envelope.
    auto addEnv = [&](const char* a, const char* h, const char* d, const char* s,
                      const char* r, const char* curve, const juce::String& pfx,
                      float defA, float defD, float defS, float defR) {
        params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{a,1}, pfx+" Atk",  envRange(0.001f, 5.0f), defA));
        params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{h,1}, pfx+" Hold", NormalisableRange<float>{0.0f, 2.0f}, 0.0f));
        params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{d,1}, pfx+" Dec",  envRange(0.001f, 5.0f), defD));
        params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{s,1}, pfx+" Sus",  NormalisableRange<float>{0.0f, 1.0f}, defS));
        params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{r,1}, pfx+" Rel",  envRange(0.001f, 10.0f), defR));
        params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{curve,1}, pfx+" Curve", NormalisableRange<float>{0.0f, 1.0f}, 0.7f));
    };

    addEnv(ParamID::AMPENV_A, ParamID::AMPENV_H, ParamID::AMPENV_D, ParamID::AMPENV_S,
           ParamID::AMPENV_R, ParamID::AMPENV_CURVE, "Amp",  0.012f, 0.5f, 0.85f, 0.8f);
    addEnv(ParamID::FILTENV_A, ParamID::FILTENV_H, ParamID::FILTENV_D, ParamID::FILTENV_S,
           ParamID::FILTENV_R, ParamID::FILTENV_CURVE, "Filt", 0.02f, 0.3f, 0.5f, 0.5f);
    addEnv(ParamID::MODENV_A, ParamID::MODENV_H, ParamID::MODENV_D, ParamID::MODENV_S,
           ParamID::MODENV_R, ParamID::MODENV_CURVE, "Mod",  0.01f, 0.3f, 0.7f, 0.5f);

    // LFOs
    auto lfoRange = NormalisableRange<float>{0.01f, 20.0f,
        [](float l, float h, float v) { return l * std::pow(h/l, v); },
        [](float l, float h, float v) { return std::log(v/l) / std::log(h/l); }};

    addChoice(ParamID::LFO1_SHAPE,  "LFO1 Shape",  lfoShapes, 0);
    params.push_back(std::make_unique<AudioParameterFloat>(
        ParameterID{ParamID::LFO1_RATE,1}, "LFO1 Rate", lfoRange, 1.0f));
    addFloat (ParamID::LFO1_DEPTH,  "LFO1 Depth",  0.0f, 1.0f, 0.0f);
    addBool  (ParamID::LFO1_RETRIG, "LFO1 Retrig", true);

    addChoice(ParamID::LFO2_SHAPE,  "LFO2 Shape",  lfoShapes, 0);
    params.push_back(std::make_unique<AudioParameterFloat>(
        ParameterID{ParamID::LFO2_RATE,1}, "LFO2 Rate", lfoRange, 0.5f));
    addFloat (ParamID::LFO2_DEPTH,  "LFO2 Depth",  0.0f, 1.0f, 0.0f);
    addBool  (ParamID::LFO2_RETRIG, "LFO2 Retrig", false);

    // Mod Matrix
    for (int i = 0; i < kNumModSlots; ++i) {
        addChoice(ParamID::MOD_SRC[i], (juce::String("Mod")+juce::String(i)+" Src").toRawUTF8(), modSources, 0);
        addChoice(ParamID::MOD_DST[i], (juce::String("Mod")+juce::String(i)+" Dst").toRawUTF8(), modDests,   0);
        addFloat (ParamID::MOD_AMT[i], (juce::String("Mod")+juce::String(i)+" Amt").toRawUTF8(), -1.0f, 1.0f, 0.0f);
    }

    // Effects
    addBool (ParamID::DRIVE_ON,     "Drive On",     false);
    addFloat(ParamID::DRIVE_AMOUNT, "Drive Amount",  0.0f, 1.0f, 0.3f);
    addFloat(ParamID::DRIVE_MIX,    "Drive Mix",     0.0f, 1.0f, 1.0f);

    addBool (ParamID::CHORUS_ON,    "Chorus On",    true);
    addFloat(ParamID::CHORUS_RATE,  "Chorus Rate",  0.1f, 5.0f, 0.4f);
    addFloat(ParamID::CHORUS_DEPTH, "Chorus Depth", 0.0f, 1.0f, 0.45f);
    addFloat(ParamID::CHORUS_MIX,   "Chorus Mix",   0.0f, 1.0f, 0.35f);

    addBool (ParamID::DELAY_ON,     "Delay On",     false);
    addFloat(ParamID::DELAY_TIME,   "Delay Time",   0.01f, 2.0f, 0.375f);
    addFloat(ParamID::DELAY_FDBK,   "Delay Fdbk",   0.0f, 0.98f, 0.35f);
    addFloat(ParamID::DELAY_MIX,    "Delay Mix",    0.0f, 1.0f, 0.25f);

    addBool (ParamID::REVERB_ON,    "Reverb On",    true);
    addFloat(ParamID::REVERB_SIZE,  "Reverb Size",  0.0f, 1.0f, 0.72f);
    addFloat(ParamID::REVERB_DAMP,  "Reverb Damp",  0.0f, 1.0f, 0.45f);
    addFloat(ParamID::REVERB_MIX,   "Reverb Mix",   0.0f, 1.0f, 0.28f);

    addBool (ParamID::EQ_ON,        "EQ On",        false);
    addFloat(ParamID::EQ_LOW,       "EQ Low",      -12.0f, 12.0f, 0.0f);
    addFloat(ParamID::EQ_MID,       "EQ Mid",      -12.0f, 12.0f, 0.0f);
    addFloat(ParamID::EQ_HIGH,      "EQ High",     -12.0f, 12.0f, 0.0f);

    addBool (ParamID::PHASER_ON,    "Phaser On",    false);
    addFloat(ParamID::PHASER_RATE,  "Phaser Rate",  0.05f, 8.0f, 0.3f);
    addFloat(ParamID::PHASER_DEPTH, "Phaser Depth", 0.0f, 1.0f, 0.6f);
    addFloat(ParamID::PHASER_MIX,   "Phaser Mix",   0.0f, 1.0f, 0.5f);

    addBool (ParamID::CRUSH_ON,     "Crush On",     false);
    addFloat(ParamID::CRUSH_BITS,   "Crush Bits",   1.0f, 16.0f, 12.0f);
    addFloat(ParamID::CRUSH_AMT,    "Crush Amt",    0.0f, 1.0f, 0.3f);
    addFloat(ParamID::CRUSH_MIX,    "Crush Mix",    0.0f, 1.0f, 0.5f);

    // Master
    addFloat(ParamID::MASTER_VOL,   "Master Vol",   0.0f, 1.0f, 0.8f);
    addFloat(ParamID::MASTER_TUNE,  "Master Tune",  -100.0f, 100.0f, 0.0f);
    addInt  (ParamID::POLY_VOICES,  "Poly Voices",  1, kMaxVoices, 8);
    // Glide time 0 = off (also controllable from the Vibe console's Glide fader).
    addFloat(ParamID::GLIDE_TIME,   "Glide Time",   0.0f, 1.0f, 0.0f);
    addBool (ParamID::GLIDE_ON,     "Glide On",     false);
    addFloat(ParamID::PITCH_BEND_RANGE, "PB Range", 1.0f, 24.0f, 2.0f);

    // Character
    addFloat(ParamID::DRIFT_AMOUNT, "Drift",        0.0f, 1.0f, 0.35f);

    // Arpeggiator
    addBool  (ParamID::ARP_ON,   "Arp On", false);
    addChoice(ParamID::ARP_RATE, "Arp Rate",
              StringArray{ "1/2","1/4","1/4T","1/8","1/8T","1/16","1/16T","1/32" }, 5 /*1/16*/);
    addChoice(ParamID::ARP_MODE, "Arp Mode",
              StringArray{ "Up","Down","Up/Down","Down/Up","As Played","Random" }, 0);
    addInt   (ParamID::ARP_OCT,  "Arp Octaves", 1, 4, 1);
    addFloat (ParamID::ARP_GATE, "Arp Gate", 0.05f, 1.0f, 0.5f);

    // Chord mode
    addBool  (ParamID::CHORD_ON,   "Chord On", false);
    {
        StringArray chordNames;
        for (auto* n : kChordNames) chordNames.add(n);
        addChoice(ParamID::CHORD_TYPE, "Chord Type", chordNames, 0 /*Major*/);
    }

    // ── Macro / creative-control layer ────────────────────────────────────────
    addFloat(ParamID::MORPH_X, "Morph X", 0.0f, 1.0f, 0.5f);
    addFloat(ParamID::MORPH_Y, "Morph Y", 0.0f, 1.0f, 0.5f);

    addFloat(ParamID::ORBIT_MOTION,  "Motion",  0.0f, 1.0f, 0.0f);
    addFloat(ParamID::ORBIT_SPACE,   "Space",   0.0f, 1.0f, 0.0f);
    addFloat(ParamID::ORBIT_TEXTURE, "Texture", 0.0f, 1.0f, 0.0f);
    addFloat(ParamID::ORBIT_DRIVE,   "Drive",   0.0f, 1.0f, 0.0f);
    addFloat(ParamID::ORBIT_TONE,    "Tone",    0.0f, 1.0f, 0.0f);

    addFloat(ParamID::VIBE_DREAM, "Dreaminess", 0.0f, 1.0f, 0.0f);
    addFloat(ParamID::VIBE_WARM,  "Warmth",     0.0f, 1.0f, 0.0f);
    addFloat(ParamID::VIBE_AIR,   "Air",        0.0f, 1.0f, 0.0f);
    addFloat(ParamID::VIBE_MOVE,  "Movement",   0.0f, 1.0f, 0.0f);
    addFloat(ParamID::VIBE_GRIT,  "Grit",       0.0f, 1.0f, 0.0f);
    addFloat(ParamID::VIBE_WIDE,  "Width",      0.0f, 1.0f, 0.0f);
    addFloat(ParamID::VIBE_FOCUS, "Focus",      0.0f, 1.0f, 0.0f);

    return { params.begin(), params.end() };
}

} // namespace drift
