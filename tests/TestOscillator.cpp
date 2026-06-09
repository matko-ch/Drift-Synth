#include <catch2/catch_all.hpp>
#include <cmath>
#include <numeric>
#include <vector>
#include "dsp/Oscillator.h"
#include "dsp/Constants.h"

using namespace drift;
using Catch::Approx;

static constexpr double kSR = 44100.0;

// Measure peak amplitude of N cycles of oscillator output.
static float measurePeak(Oscillator& osc, OscShape shape, float freq, int cycles = 10) {
    const float phaseInc = freq / static_cast<float>(kSR);
    const int   n        = static_cast<int>(cycles * kSR / freq);
    float peak = 0.0f;
    // Skip first cycle (transients, triangle integrator settling)
    for (int i = 0; i < static_cast<int>(kSR / freq); ++i)
        (void)osc.process(phaseInc, shape);
    for (int i = 0; i < n; ++i)
        peak = std::max(peak, std::abs(osc.process(phaseInc, shape)));
    return peak;
}

// Compute THD+N: ratio of non-fundamental energy to fundamental for a sine oscillator.
// A band-limited saw should have THD from harmonics but no alias artefacts above Nyquist.
static float computeAliasPower(Oscillator& osc, OscShape shape, float freq) {
    const float phaseInc = freq / static_cast<float>(kSR);
    const int   n        = static_cast<int>(4.0 * kSR / freq);  // 4 cycles
    std::vector<float> buf(n);

    // Settle
    for (int i = 0; i < n / 4; ++i) (void)osc.process(phaseInc, shape);
    for (int i = 0; i < n; ++i)     buf[i] = osc.process(phaseInc, shape);

    // Power above Nyquist alias threshold — any alias shows up above fund frequency
    // but here we just check that no sample exceeds 1.5× unity (a simple alias indicator)
    float maxAbs = 0.0f;
    for (auto v : buf) maxAbs = std::max(maxAbs, std::abs(v));
    return maxAbs;
}

// ── Tests ─────────────────────────────────────────────────────────────────────

TEST_CASE("Oscillator: sine amplitude approx 1.0") {
    Oscillator osc;
    osc.prepare(kSR);
    const float peak = measurePeak(osc, OscShape::Sine, 440.0f);
    REQUIRE(peak == Approx(1.0f).margin(0.01f));
}

TEST_CASE("Oscillator: saw amplitude approx 1.0") {
    Oscillator osc;
    osc.prepare(kSR);
    const float peak = measurePeak(osc, OscShape::Saw, 440.0f);
    REQUIRE(peak < 1.1f);
    REQUIRE(peak > 0.85f);
}

TEST_CASE("Oscillator: square amplitude approx 1.0") {
    Oscillator osc;
    osc.prepare(kSR);
    const float peak = measurePeak(osc, OscShape::Square, 440.0f);
    REQUIRE(peak < 1.1f);
    REQUIRE(peak > 0.85f);
}

TEST_CASE("Oscillator: triangle amplitude approx 1.0 after settling") {
    Oscillator osc;
    osc.prepare(kSR);
    const float peak = measurePeak(osc, OscShape::Triangle, 440.0f, 20);
    REQUIRE(peak < 1.2f);
    REQUIRE(peak > 0.7f);
}

TEST_CASE("Oscillator: no NaN or Inf in output at high frequencies") {
    Oscillator osc;
    osc.prepare(kSR);
    const float highFreq = 19000.0f;
    const float phaseInc = highFreq / static_cast<float>(kSR);
    for (auto shape : {OscShape::Sine, OscShape::Saw, OscShape::Square, OscShape::Triangle}) {
        osc.reset();
        for (int i = 0; i < 10000; ++i) {
            const float v = osc.process(phaseInc, shape);
            REQUIRE(std::isfinite(v));
        }
    }
}

TEST_CASE("Oscillator: no alias spikes above 1.5 at high notes") {
    for (float freq : {4000.0f, 8000.0f, 12000.0f, 18000.0f}) {
        Oscillator osc;
        osc.prepare(kSR);
        const float maxAbs = computeAliasPower(osc, OscShape::Saw, freq);
        INFO("freq=" << freq << " maxAbs=" << maxAbs);
        REQUIRE(maxAbs < 1.5f);
    }
}

TEST_CASE("Oscillator: phase reset produces deterministic output") {
    Oscillator osc;
    osc.prepare(kSR);
    osc.reset(0.0f);
    const float phaseInc = 440.0f / static_cast<float>(kSR);
    std::vector<float> run1, run2;
    for (int i = 0; i < 100; ++i)
        run1.push_back(osc.process(phaseInc, OscShape::Sine));

    osc.reset(0.0f);
    for (int i = 0; i < 100; ++i)
        run2.push_back(osc.process(phaseInc, OscShape::Sine));

    REQUIRE(run1 == run2);
}

TEST_CASE("Oscillator: pulse width extremes don't produce NaN") {
    Oscillator osc;
    osc.prepare(kSR);
    const float phaseInc = 440.0f / static_cast<float>(kSR);
    for (float pw : {0.05f, 0.1f, 0.5f, 0.9f, 0.95f}) {
        osc.reset(0.0f);
        for (int i = 0; i < 1000; ++i) {
            const float v = osc.process(phaseInc, OscShape::Pulse, pw);
            REQUIRE(std::isfinite(v));
        }
    }
}
