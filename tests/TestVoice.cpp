#include <catch2/catch_all.hpp>
#include <cmath>
#include "dsp/Voice.h"
#include "dsp/VoiceManager.h"

using namespace drift;
using Catch::Approx;

static constexpr double kSR = 44100.0;

static PatchParams defaultPatch() {
    PatchParams p;
    p.ampEnv.attackTime  = 0.001f;
    p.ampEnv.decayTime   = 0.01f;
    p.ampEnv.sustainLevel= 1.0f;
    p.ampEnv.releaseTime = 0.01f;
    p.filterEnv = p.ampEnv;
    p.modEnv    = p.ampEnv;
    p.filter1Cutoff = 20000.0f;
    p.filter2Cutoff = 20000.0f;
    p.filter1Res = 0.0f;
    p.filter2Res = 0.0f;
    p.unisonVoices = 1;
    p.oscMix = 0.5f;
    return p;
}

TEST_CASE("Voice: idle before noteOn") {
    Voice v;
    v.prepare(kSR);
    REQUIRE(v.isIdle());
}

TEST_CASE("Voice: active after noteOn") {
    Voice v;
    v.prepare(kSR);
    auto p = defaultPatch();
    v.noteOn(60, 1.0f, p);
    REQUIRE(!v.isIdle());
}

TEST_CASE("Voice: produces finite output") {
    Voice v;
    v.prepare(kSR);
    auto p = defaultPatch();
    v.noteOn(60, 1.0f, p);
    for (int i = 0; i < 4410; ++i) {
        auto [l, r] = v.process(p);
        REQUIRE(std::isfinite(l));
        REQUIRE(std::isfinite(r));
    }
}

TEST_CASE("Voice: idle after release") {
    Voice v;
    v.prepare(kSR);
    auto p = defaultPatch();
    p.ampEnv.releaseTime = 0.001f;
    v.noteOn(60, 1.0f, p);
    for (int i = 0; i < 4410; ++i) (void)v.process(p);
    v.noteOff();
    for (int i = 0; i < 4410; ++i) (void)v.process(p);
    REQUIRE(v.isIdle());
}

TEST_CASE("VoiceManager: note on to note off cycle") {
    VoiceManager vm;
    vm.prepare(kSR);
    vm.setMaxVoices(4);
    vm.noteOn(60, 0.8f);
    for (int i = 0; i < 1000; ++i) {
        float l, r;
        vm.processSample(l, r);
        REQUIRE(std::isfinite(l));
        REQUIRE(std::isfinite(r));
    }
    vm.noteOff(60);
    for (int i = 0; i < 22050; ++i) {
        float l, r;
        vm.processSample(l, r);
        REQUIRE(std::isfinite(l));
        REQUIRE(std::isfinite(r));
    }
}

TEST_CASE("VoiceManager: voice stealing doesn't crash at max polyphony") {
    VoiceManager vm;
    vm.prepare(kSR);
    vm.setMaxVoices(kMaxVoices);
    for (int note = 36; note < 36 + kMaxVoices + 4; ++note)
        vm.noteOn(note, 0.8f);
    for (int i = 0; i < 1000; ++i) {
        float l, r;
        vm.processSample(l, r);
        REQUIRE(std::isfinite(l));
        REQUIRE(std::isfinite(r));
    }
}

TEST_CASE("Phase 4: NaN/Inf at all standard sample rates and tiny buffers") {
    for (double sr : {44100.0, 48000.0, 96000.0, 192000.0}) {
        for (int blockSize : {1, 16}) {
            VoiceManager vm;
            vm.prepare(sr);
            vm.setMaxVoices(4);
            for (int n : {36, 48, 60, 72}) vm.noteOn(n, 0.8f);

            for (int block = 0; block < 128; ++block) {
                for (int i = 0; i < blockSize; ++i) {
                    float l = 0.0f, r = 0.0f;
                    vm.processSample(l, r);
                    REQUIRE(std::isfinite(l));
                    REQUIRE(std::isfinite(r));
                }
            }
            vm.allSoundOff();
        }
    }
}

TEST_CASE("VoiceManager: allSoundOff silences immediately") {
    VoiceManager vm;
    vm.prepare(kSR);
    vm.setMaxVoices(4);
    for (int n : {60, 64, 67, 71}) vm.noteOn(n, 0.8f);
    for (int i = 0; i < 100; ++i) {
        float l, r;
        vm.processSample(l, r);
    }
    vm.allSoundOff();
    float l, r;
    vm.processSample(l, r);
    REQUIRE(l == Approx(0.0f).margin(0.001f));
    REQUIRE(r == Approx(0.0f).margin(0.001f));
}
