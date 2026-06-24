cmake_minimum_required(VERSION 3.22)

add_executable(DriftTests
    TestOscillator.cpp
    TestFilter.cpp
    TestEnvelope.cpp
    TestModMatrix.cpp
    TestVoice.cpp
    TestEffectsStability.cpp

    # DSP implementation — compiled directly into the test binary (no JUCE plugin machinery)
    ${CMAKE_SOURCE_DIR}/Source/dsp/Oscillator.cpp
    ${CMAKE_SOURCE_DIR}/Source/dsp/Filter.cpp
    ${CMAKE_SOURCE_DIR}/Source/dsp/Envelope.cpp
    ${CMAKE_SOURCE_DIR}/Source/dsp/LFO.cpp
    ${CMAKE_SOURCE_DIR}/Source/dsp/ModMatrix.cpp
    ${CMAKE_SOURCE_DIR}/Source/dsp/Voice.cpp
    ${CMAKE_SOURCE_DIR}/Source/dsp/VoiceManager.cpp
    ${CMAKE_SOURCE_DIR}/Source/dsp/effects/PlateReverb.cpp
    ${CMAKE_SOURCE_DIR}/Source/dsp/effects/StereoDelay.cpp
)

target_include_directories(DriftTests PRIVATE
    ${CMAKE_SOURCE_DIR}/Source
)

target_compile_features(DriftTests PRIVATE cxx_std_20)

target_link_libraries(DriftTests
    PRIVATE
        Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(DriftTests)
