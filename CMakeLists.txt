cmake_minimum_required(VERSION 3.22)
project(Drift VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()  # needed so CTest discovers tests in subdirectories

# ── Fetch JUCE ────────────────────────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.4
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(JUCE)

# Fetch Catch2 for unit tests
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.6.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Catch2)

# ── Plugin target ──────────────────────────────────────────────────────────────
juce_add_plugin(Drift
    COMPANY_NAME            "DriftAudio"
    COMPANY_WEBSITE         ""
    PLUGIN_MANUFACTURER_CODE Drft
    PLUGIN_CODE             Dft1
    FORMATS                 VST3 AU Standalone
    PRODUCT_NAME            "Drift"
    BUNDLE_ID               "com.driftaudio.drift"
    IS_SYNTH                TRUE
    NEEDS_MIDI_INPUT        TRUE
    NEEDS_MIDI_OUTPUT       FALSE
    IS_MIDI_EFFECT          FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES         "Instrument" "Synth"
    AU_MAIN_TYPE            kAudioUnitType_MusicDevice
)

juce_generate_juce_header(Drift)

# ── Sources ────────────────────────────────────────────────────────────────────
target_sources(Drift PRIVATE
    Source/plugin/PluginProcessor.cpp
    Source/plugin/PluginProcessor.h
    Source/plugin/Presets.cpp
    Source/plugin/Presets.h
    Source/ui/PluginEditor.cpp
    Source/ui/PluginEditor.h
    Source/ui/DriftLookAndFeel.cpp
    Source/ui/DriftLookAndFeel.h
    Source/ui/components/KnobComponent.cpp
    Source/ui/components/KnobComponent.h
    Source/ui/components/PresetBar.cpp
    Source/ui/components/PresetBar.h
    Source/ui/components/DriftFieldPad.cpp
    Source/ui/components/DriftFieldPad.h
    Source/ui/components/OrbitPanel.cpp
    Source/ui/components/OrbitPanel.h
    Source/ui/components/VibeConsole.cpp
    Source/ui/components/VibeConsole.h
    Source/ui/components/SynthStrip.cpp
    Source/ui/components/SynthStrip.h
    Source/ui/components/OscExtraPanel.cpp
    Source/ui/components/OscExtraPanel.h
    Source/ui/components/PerformBar.cpp
    Source/ui/components/PerformBar.h
    Source/ui/components/AdvancedPanel.cpp
    Source/ui/components/AdvancedPanel.h
    Source/ui/components/OscillatorPanel.cpp
    Source/ui/components/OscillatorPanel.h
    Source/ui/components/FilterPanel.cpp
    Source/ui/components/FilterPanel.h
    Source/ui/components/EnvelopePanel.cpp
    Source/ui/components/EnvelopePanel.h
    Source/ui/components/LFOPanel.cpp
    Source/ui/components/LFOPanel.h
    Source/ui/components/ModMatrixPanel.cpp
    Source/ui/components/ModMatrixPanel.h
    Source/ui/components/FXPanel.cpp
    Source/ui/components/FXPanel.h
    Source/dsp/Oscillator.cpp
    Source/dsp/Oscillator.h
    Source/dsp/Filter.cpp
    Source/dsp/Filter.h
    Source/dsp/Envelope.cpp
    Source/dsp/Envelope.h
    Source/dsp/LFO.cpp
    Source/dsp/LFO.h
    Source/dsp/ModMatrix.cpp
    Source/dsp/ModMatrix.h
    Source/dsp/MacroEngine.cpp
    Source/dsp/MacroEngine.h
    Source/dsp/Arpeggiator.cpp
    Source/dsp/Arpeggiator.h
    Source/dsp/Chords.h
    Source/dsp/Voice.cpp
    Source/dsp/Voice.h
    Source/dsp/VoiceManager.cpp
    Source/dsp/VoiceManager.h
    Source/dsp/effects/Chorus.cpp
    Source/dsp/effects/Chorus.h
    Source/dsp/effects/StereoDelay.cpp
    Source/dsp/effects/StereoDelay.h
    Source/dsp/effects/PlateReverb.cpp
    Source/dsp/effects/PlateReverb.h
    Source/dsp/effects/Drive.cpp
    Source/dsp/effects/Drive.h
    Source/dsp/effects/EQ.cpp
    Source/dsp/effects/EQ.h
    Source/dsp/effects/Phaser.cpp
    Source/dsp/effects/Phaser.h
    Source/dsp/effects/Bitcrusher.cpp
    Source/dsp/effects/Bitcrusher.h
)

target_compile_definitions(Drift
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
)

target_link_libraries(Drift
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# ── Tests ──────────────────────────────────────────────────────────────────────
add_subdirectory(tests)
