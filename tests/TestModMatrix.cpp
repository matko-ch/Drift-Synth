#include <catch2/catch_all.hpp>
#include "dsp/ModMatrix.h"

using namespace drift;
using Catch::Approx;

TEST_CASE("ModMatrix: empty matrix outputs zeros") {
    ModMatrix mm;
    std::array<float, static_cast<int>(ModSource::Count)> src{};
    src[static_cast<int>(ModSource::LFO1)] = 1.0f;
    const auto out = mm.process(src);
    REQUIRE(out.filter1Cutoff == Approx(0.0f));
    REQUIRE(out.osc1Pitch     == Approx(0.0f));
}

TEST_CASE("ModMatrix: single slot routes correctly") {
    ModMatrix mm;
    ModSlot slot;
    slot.source = ModSource::LFO1;
    slot.dest   = ModDest::Filter1Cutoff;
    slot.amount = 0.5f;
    mm.setSlot(0, slot);

    std::array<float, static_cast<int>(ModSource::Count)> src{};
    src[static_cast<int>(ModSource::LFO1)] = 0.8f;

    const auto out = mm.process(src);
    REQUIRE(out.filter1Cutoff == Approx(0.4f));  // 0.8 * 0.5
}

TEST_CASE("ModMatrix: bipolar amount works") {
    ModMatrix mm;
    ModSlot slot;
    slot.source = ModSource::Velocity;
    slot.dest   = ModDest::AmpVolume;
    slot.amount = -1.0f;
    mm.setSlot(0, slot);

    std::array<float, static_cast<int>(ModSource::Count)> src{};
    src[static_cast<int>(ModSource::Velocity)] = 0.7f;

    const auto out = mm.process(src);
    REQUIRE(out.ampVolume == Approx(-0.7f));
}

TEST_CASE("ModMatrix: None source contributes nothing") {
    ModMatrix mm;
    ModSlot slot;
    slot.source = ModSource::None;
    slot.dest   = ModDest::Filter1Cutoff;
    slot.amount = 1.0f;
    mm.setSlot(0, slot);

    std::array<float, static_cast<int>(ModSource::Count)> src{};
    src[static_cast<int>(ModSource::None)] = 99.0f;
    const auto out = mm.process(src);
    REQUIRE(out.filter1Cutoff == Approx(0.0f));
}

TEST_CASE("ModMatrix: multiple slots accumulate") {
    ModMatrix mm;
    ModSlot s1; s1.source=ModSource::LFO1; s1.dest=ModDest::Osc1Pitch; s1.amount=2.0f;
    ModSlot s2; s2.source=ModSource::LFO2; s2.dest=ModDest::Osc1Pitch; s2.amount=1.0f;
    mm.setSlot(0, s1);
    mm.setSlot(1, s2);

    std::array<float, static_cast<int>(ModSource::Count)> src{};
    src[static_cast<int>(ModSource::LFO1)] = 0.5f;
    src[static_cast<int>(ModSource::LFO2)] = 0.3f;

    const auto out = mm.process(src);
    REQUIRE(out.osc1Pitch == Approx(0.5f*2.0f + 0.3f*1.0f));  // 1.3
}
