#include <catch2/catch_all.hpp>
#include <cmath>
#include "dsp/Envelope.h"

using namespace drift;
using Catch::Approx;

static constexpr double kSR = 44100.0;

TEST_CASE("Envelope: idle before noteOn") {
    Envelope env;
    env.prepare(kSR);
    REQUIRE(env.isIdle());
    REQUIRE(env.process() == Approx(0.0f));
}

TEST_CASE("Envelope: attack reaches 1.0") {
    Envelope env;
    env.prepare(kSR);
    Envelope::Params p;
    p.attackTime  = 0.01f;
    p.decayTime   = 0.3f;
    p.sustainLevel= 0.7f;
    p.releaseTime = 0.5f;
    env.setParams(p);
    env.noteOn(1.0f);

    float maxVal = 0.0f;
    for (int i = 0; i < static_cast<int>(kSR * 0.1); ++i)
        maxVal = std::max(maxVal, env.process());

    REQUIRE(maxVal == Approx(1.0f).margin(0.01f));
}

TEST_CASE("Envelope: sustain level is correct") {
    Envelope env;
    env.prepare(kSR);
    Envelope::Params p;
    p.attackTime  = 0.001f;
    p.decayTime   = 0.01f;
    p.sustainLevel= 0.6f;
    p.releaseTime = 0.1f;
    env.setParams(p);
    env.noteOn(1.0f);

    float lastVal = 0.0f;
    // Run for 200 ms — should have reached sustain
    for (int i = 0; i < static_cast<int>(kSR * 0.2); ++i)
        lastVal = env.process();

    REQUIRE(lastVal == Approx(0.6f).margin(0.02f));
}

TEST_CASE("Envelope: release reaches 0.0") {
    Envelope env;
    env.prepare(kSR);
    Envelope::Params p;
    p.attackTime  = 0.001f;
    p.decayTime   = 0.001f;
    p.sustainLevel= 0.8f;
    p.releaseTime = 0.05f;
    env.setParams(p);
    env.noteOn(1.0f);

    // Run to sustain
    for (int i = 0; i < static_cast<int>(kSR * 0.1); ++i)
        (void)env.process();

    env.noteOff();

    // Run release
    float lastVal = 1.0f;
    for (int i = 0; i < static_cast<int>(kSR * 0.5); ++i)
        lastVal = env.process();

    REQUIRE(lastVal == Approx(0.0f).margin(0.001f));
    REQUIRE(env.isIdle());
}

TEST_CASE("Envelope: velocity scales output correctly") {
    Envelope env;
    env.prepare(kSR);
    Envelope::Params p;
    p.attackTime  = 0.001f;
    p.decayTime   = 0.001f;
    p.sustainLevel= 1.0f;
    p.releaseTime = 0.01f;
    env.setParams(p);

    auto runToSustain = [&](float vel) {
        env.reset();
        env.noteOn(vel);
        float lastVal = 0.0f;
        for (int i = 0; i < static_cast<int>(kSR * 0.1); ++i)
            lastVal = env.process();
        return lastVal;
    };

    const float full = runToSustain(1.0f);
    const float half = runToSustain(0.5f);
    REQUIRE(half == Approx(full * 0.5f).margin(0.02f));
}

TEST_CASE("Envelope: no NaN or Inf in output") {
    Envelope env;
    env.prepare(kSR);
    Envelope::Params p;
    p.attackTime  = 0.001f;
    p.decayTime   = 5.0f;
    p.sustainLevel= 0.0f;
    p.releaseTime = 10.0f;
    env.setParams(p);
    env.noteOn(1.0f);
    for (int i = 0; i < static_cast<int>(kSR * 6.0); ++i)
        REQUIRE(std::isfinite(env.process()));
    env.noteOff();
    for (int i = 0; i < static_cast<int>(kSR * 11.0); ++i)
        REQUIRE(std::isfinite(env.process()));
}
