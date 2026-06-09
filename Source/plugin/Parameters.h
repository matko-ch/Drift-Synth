#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "../dsp/Constants.h"
#include "../dsp/Filter.h"
#include "../dsp/LFO.h"
#include "../dsp/ModMatrix.h"

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

// Amp Envelope
inline constexpr auto AMPENV_A = "ampenv_a";
inline constexpr auto AMPENV_D = "ampenv_d";
inline constexpr auto AMPENV_S = "ampenv_s";
inline constexpr auto AMPENV_R = "ampenv_r";

// Filter Envelope
inline constexpr auto FILTENV_A = "filtenv_a";
inline constexpr auto FILTENV_D = "filtenv_d";
inline constexpr auto FILTENV_S = "filtenv_s";
inline constexpr auto FILTENV_R = "filtenv_r";

// Mod Envelope
inline constexpr auto MODENV_A = "modenv_a";
inline constexpr auto MODENV_D = "modenv_d";
inline constexpr auto MODENV_S = "modenv_s";
inline constexpr auto MODENV_R = "modenv_r";

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

// Master
inline constexpr auto MASTER_VOL   = "master_vol";
inline constexpr auto MASTER_TUNE  = "master_tune";
inline constexpr auto POLY_VOICES  = "poly_voices";
inline constexpr auto GLIDE_TIME   = "glide_time";
inline constexpr auto GLIDE_ON     = "glide_on";
inline constexpr auto PITCH_BEND_RANGE = "pb_range";
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

    auto addBool = [&](const char* id, const char* name, bool def) {
        params.push_back(std::make_unique<AudioParameterBool>(
            ParameterID{id, 1}, name, def));
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

    // Unison
    addInt   (ParamID::UNISON_VOICES, "Unison Voices", 1, kMaxUnisonVoices, 1);
    addFloat (ParamID::UNISON_DETUNE, "Unison Detune", 0.0f, 1.0f, 0.3f);
    addFloat (ParamID::UNISON_SPREAD, "Unison Spread", 0.0f, 1.0f, 0.5f);

    // Filter 1
    {
        auto range = NormalisableRange<float>{kMinCutoffHz, kMaxCutoffHz,
            [](float lo, float hi, float v) { return lo * std::pow(hi/lo, v); },
            [](float lo, float hi, float v) { return std::log(v/lo) / std::log(hi/lo); }};
        params.push_back(std::make_unique<AudioParameterFloat>(
            ParameterID{ParamID::FILT1_CUTOFF, 1}, "Filt1 Cutoff", range, 8000.0f));
    }
    addFloat (ParamID::FILT1_RES,      "Filt1 Res",     0.0f, 1.0f, 0.0f);
    addFloat (ParamID::FILT1_DRIVE,    "Filt1 Drive",   0.0f, 1.0f, 0.0f);
    addChoice(ParamID::FILT1_TYPE,     "Filt1 Type", filtTypes, 0);
    addFloat (ParamID::FILT1_ENV_AMT,  "Filt1 Env Amt", -1.0f, 1.0f, 0.0f);
    addFloat (ParamID::FILT1_KEYTRACK, "Filt1 KeyTrk",   0.0f, 1.0f, 0.0f);

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

    auto addEnv = [&](const char* a, const char* d, const char* s, const char* r,
                       const char* na, const char* nd, const char* ns, const char* nr) {
        params.push_back(std::make_unique<AudioParameterFloat>(
            ParameterID{a,1}, na, envRange(0.001f, 5.0f), 0.01f));
        params.push_back(std::make_unique<AudioParameterFloat>(
            ParameterID{d,1}, nd, envRange(0.001f, 5.0f), 0.3f));
        params.push_back(std::make_unique<AudioParameterFloat>(
            ParameterID{s,1}, ns, NormalisableRange<float>{0.0f,1.0f}, 0.7f));
        params.push_back(std::make_unique<AudioParameterFloat>(
            ParameterID{r,1}, nr, envRange(0.001f, 10.0f), 0.5f));
    };

    addEnv(ParamID::AMPENV_A, ParamID::AMPENV_D, ParamID::AMPENV_S, ParamID::AMPENV_R,
           "Amp Atk","Amp Dec","Amp Sus","Amp Rel");
    addEnv(ParamID::FILTENV_A, ParamID::FILTENV_D, ParamID::FILTENV_S, ParamID::FILTENV_R,
           "Filt Atk","Filt Dec","Filt Sus","Filt Rel");
    addEnv(ParamID::MODENV_A, ParamID::MODENV_D, ParamID::MODENV_S, ParamID::MODENV_R,
           "Mod Atk","Mod Dec","Mod Sus","Mod Rel");

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

    addBool (ParamID::CHORUS_ON,    "Chorus On",    false);
    addFloat(ParamID::CHORUS_RATE,  "Chorus Rate",  0.1f, 5.0f, 0.5f);
    addFloat(ParamID::CHORUS_DEPTH, "Chorus Depth", 0.0f, 1.0f, 0.5f);
    addFloat(ParamID::CHORUS_MIX,   "Chorus Mix",   0.0f, 1.0f, 0.5f);

    addBool (ParamID::DELAY_ON,     "Delay On",     false);
    addFloat(ParamID::DELAY_TIME,   "Delay Time",   0.01f, 2.0f, 0.375f);
    addFloat(ParamID::DELAY_FDBK,   "Delay Fdbk",   0.0f, 0.98f, 0.4f);
    addFloat(ParamID::DELAY_MIX,    "Delay Mix",    0.0f, 1.0f, 0.3f);

    addBool (ParamID::REVERB_ON,    "Reverb On",    false);
    addFloat(ParamID::REVERB_SIZE,  "Reverb Size",  0.0f, 1.0f, 0.6f);
    addFloat(ParamID::REVERB_DAMP,  "Reverb Damp",  0.0f, 1.0f, 0.5f);
    addFloat(ParamID::REVERB_MIX,   "Reverb Mix",   0.0f, 1.0f, 0.3f);

    // Master
    addFloat(ParamID::MASTER_VOL,   "Master Vol",   0.0f, 1.0f, 0.8f);
    addFloat(ParamID::MASTER_TUNE,  "Master Tune",  -100.0f, 100.0f, 0.0f);
    addInt  (ParamID::POLY_VOICES,  "Poly Voices",  1, kMaxVoices, 8);
    addFloat(ParamID::GLIDE_TIME,   "Glide Time",   0.001f, 1.0f, 0.05f);
    addBool (ParamID::GLIDE_ON,     "Glide On",     false);
    addFloat(ParamID::PITCH_BEND_RANGE, "PB Range", 1.0f, 24.0f, 2.0f);

    return { params.begin(), params.end() };
}

} // namespace drift
