#include <catch2/catch_all.hpp>
#include <cmath>
#include <vector>
#include "dsp/Filter.h"
#include "dsp/Constants.h"

using namespace drift;
using Catch::Approx;

static constexpr double kSR = 44100.0;

// Measure steady-state amplitude of a sine at given frequency through the filter.
static float filterGainAt(Filter& filt, float testFreqHz, FilterType type,
                           float cutoffHz, float res = 0.0f) {
    filt.reset();
    filt.setParameters(cutoffHz, res, 0.0f, type);

    const float phaseInc = testFreqHz / static_cast<float>(kSR);
    float phase = 0.0f;
    float peak  = 0.0f;

    // Settle
    for (int i = 0; i < 4096; ++i) {
        const float s = std::sin(phase * kTwoPi);
        (void)filt.process(s);
        phase += phaseInc;
        if (phase >= 1.0f) phase -= 1.0f;
    }
    // Measure
    for (int i = 0; i < 2048; ++i) {
        const float s = std::sin(phase * kTwoPi);
        const float o = filt.process(s);
        peak = std::max(peak, std::abs(o));
        phase += phaseInc;
        if (phase >= 1.0f) phase -= 1.0f;
    }
    return peak;
}

TEST_CASE("Filter SVF LP12: unity gain well below cutoff") {
    Filter f;
    f.prepare(kSR);
    const float gain = filterGainAt(f, 100.0f, FilterType::LowPass12, 8000.0f);
    REQUIRE(gain == Approx(1.0f).margin(0.05f));
}

TEST_CASE("Filter SVF LP12: significant attenuation well above cutoff") {
    Filter f;
    f.prepare(kSR);
    const float gain = filterGainAt(f, 16000.0f, FilterType::LowPass12, 1000.0f);
    REQUIRE(gain < 0.2f);
}

TEST_CASE("Filter SVF HP12: attenuation below cutoff") {
    Filter f;
    f.prepare(kSR);
    const float gain = filterGainAt(f, 100.0f, FilterType::HighPass12, 8000.0f);
    REQUIRE(gain < 0.2f);
}

TEST_CASE("Filter SVF HP12: unity gain above cutoff") {
    Filter f;
    f.prepare(kSR);
    const float gain = filterGainAt(f, 16000.0f, FilterType::HighPass12, 1000.0f);
    REQUIRE(gain > 0.8f);
}

TEST_CASE("Filter LP24: 24 dB steeper rolloff than LP12") {
    Filter f12, f24;
    f12.prepare(kSR);
    f24.prepare(kSR);
    const float freq = 8000.0f, cutoff = 1000.0f;
    const float g12 = filterGainAt(f12, freq, FilterType::LowPass12, cutoff);
    const float g24 = filterGainAt(f24, freq, FilterType::LowPass24, cutoff);
    REQUIRE(g24 < g12);
    REQUIRE(g24 < 0.1f);
}

TEST_CASE("Filter Moog LP: passes low frequencies") {
    Filter f;
    f.prepare(kSR);
    const float gain = filterGainAt(f, 200.0f, FilterType::MoogLP24, 8000.0f);
    REQUIRE(gain > 0.7f);
}

TEST_CASE("Filter: no NaN or Inf with high resonance") {
    Filter f;
    f.prepare(kSR);
    f.setParameters(1000.0f, 0.98f, 0.0f, FilterType::MoogLP24);

    float phase = 0.0f;
    const float phaseInc = 440.0f / static_cast<float>(kSR);
    for (int i = 0; i < 44100; ++i) {
        const float s = std::sin(phase * kTwoPi);
        const float o = f.process(s);
        REQUIRE(std::isfinite(o));
        phase += phaseInc;
        if (phase >= 1.0f) phase -= 1.0f;
    }
}

TEST_CASE("Filter: no NaN or Inf on reset") {
    Filter f;
    f.prepare(kSR);
    f.setParameters(500.0f, 0.5f, 0.5f, FilterType::LowPass12);
    for (int i = 0; i < 1000; ++i) (void)f.process(0.5f);
    f.reset();
    const float o = f.process(0.0f);
    REQUIRE(std::isfinite(o));
    REQUIRE(o == Approx(0.0f).margin(0.01f));
}

TEST_CASE("Filter: all types produce finite output at extreme parameters") {
    Filter f;
    f.prepare(kSR);
    for (int t = 0; t < static_cast<int>(FilterType::Count); ++t) {
        f.reset();
        f.setParameters(20000.0f, 0.99f, 1.0f, static_cast<FilterType>(t));
        for (int i = 0; i < 2048; ++i) {
            const float o = f.process(1.0f);
            REQUIRE(std::isfinite(o));
        }
    }
}
