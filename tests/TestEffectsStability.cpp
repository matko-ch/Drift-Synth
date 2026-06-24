// Regression tests for the runaway-feedback / NaN-poisoning class of bugs.
// A feedback effect must NEVER grow without bound, and a single bad sample must
// never get trapped recirculating forever. These tests guarantee that.
#include <catch2/catch_test_macros.hpp>
#include "dsp/effects/PlateReverb.h"
#include "dsp/effects/StereoDelay.h"
#include <cmath>
#include <limits>
#include <memory>

using namespace drift;

namespace {
constexpr double kSR = 48000.0;
constexpr float  kBound = 50.0f;  // anything above this is a runaway

// A simple deterministic noise source.
struct Noise { uint32_t s = 0x1234567u; float next() {
    s ^= s << 13; s ^= s >> 17; s ^= s << 5;
    return (float)s * (2.0f / 4294967296.0f) - 1.0f; } };
}

TEST_CASE("PlateReverb never runs away at max settings", "[stability]") {
    auto revPtr = std::make_unique<PlateReverb>();  // heap: buffers are large
    auto& rev = *revPtr;
    rev.prepare(kSR);
    rev.setParameters(1.0f, 0.0f, 1.0f);  // max size, no damping, full wet

    Noise n;
    // 10 seconds of loud sustained input — the old FDN exploded within ms.
    for (int i = 0; i < (int)(kSR * 10); ++i) {
        float l = n.next() * 0.9f, r = n.next() * 0.9f;
        rev.processStereo(l, r);
        REQUIRE(std::isfinite(l));
        REQUIRE(std::isfinite(r));
        REQUIRE(std::abs(l) < kBound);
        REQUIRE(std::abs(r) < kBound);
    }
}

TEST_CASE("PlateReverb recovers after a NaN/Inf injection", "[stability]") {
    auto revPtr = std::make_unique<PlateReverb>();
    auto& rev = *revPtr;
    rev.prepare(kSR);
    rev.setParameters(0.8f, 0.3f, 0.5f);

    // Poison the input with NaN then Inf.
    float l = std::numeric_limits<float>::quiet_NaN(), r = l;
    rev.processStereo(l, r);
    l = std::numeric_limits<float>::infinity(); r = -l;
    rev.processStereo(l, r);

    // After feeding clean silence, output must return to finite & decay to ~0.
    float last = 1.0f;
    for (int i = 0; i < (int)(kSR * 4); ++i) {
        float a = 0.0f, b = 0.0f;
        rev.processStereo(a, b);
        REQUIRE(std::isfinite(a));
        REQUIRE(std::isfinite(b));
        last = std::abs(a) + std::abs(b);
    }
    REQUIRE(last < 1e-3f);
}

TEST_CASE("StereoDelay never runs away at max feedback", "[stability]") {
    auto dlyPtr = std::make_unique<StereoDelay>();  // heap: ~1.5 MB buffers
    auto& dly = *dlyPtr;
    dly.prepare(kSR);
    dly.setParameters(0.25f, 0.98f, 1.0f);  // max allowed feedback

    Noise n;
    for (int i = 0; i < (int)(kSR * 10); ++i) {
        float l = n.next(), r = n.next();
        dly.processStereo(l, r);
        REQUIRE(std::isfinite(l));
        REQUIRE(std::isfinite(r));
        REQUIRE(std::abs(l) < kBound);
        REQUIRE(std::abs(r) < kBound);
    }
}

TEST_CASE("StereoDelay recovers after a NaN injection", "[stability]") {
    auto dlyPtr = std::make_unique<StereoDelay>();
    auto& dly = *dlyPtr;
    dly.prepare(kSR);
    dly.setParameters(0.25f, 0.9f, 0.5f);

    float l = std::numeric_limits<float>::quiet_NaN(), r = l;
    dly.processStereo(l, r);

    float last = 1.0f;
    for (int i = 0; i < (int)(kSR * 4); ++i) {
        float a = 0.0f, b = 0.0f;
        dly.processStereo(a, b);
        REQUIRE(std::isfinite(a));
        REQUIRE(std::isfinite(b));
        last = std::abs(a) + std::abs(b);
    }
    REQUIRE(last < 1e-3f);
}
