#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "../dsp/VoiceManager.h"
#include "../dsp/effects/Drive.h"
#include "../dsp/effects/Chorus.h"
#include "../dsp/effects/StereoDelay.h"
#include "../dsp/effects/PlateReverb.h"
#include <atomic>

namespace drift {

class PluginProcessor final : public juce::AudioProcessor {
public:
    PluginProcessor();
    ~PluginProcessor() override = default;

    // ── AudioProcessor interface ──────────────────────────────────────────────
    void prepareToPlay(double sampleRate, int maxBlockSize) override;
    void releaseResources() override {}
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Drift"; }
    bool   acceptsMidi()  const override { return true;  }
    bool   producesMidi() const override { return false; }
    bool   isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 4.0; }

    int  getNumPrograms()    override { return 1; }
    int  getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return "Default"; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& data) override;
    void setStateInformation(const void* data, int size) override;

    // ── Public state for the editor ───────────────────────────────────────────
    juce::AudioProcessorValueTreeState apvts;

    // Lock-free meter output (peak, updated each block)
    std::atomic<float> meterL { 0.0f }, meterR { 0.0f };

private:
    void syncPatchFromParams() noexcept;
    void handleMidi(const juce::MidiMessage& msg) noexcept;

    VoiceManager  mVoiceManager;
    Drive         mDrive;
    Chorus        mChorus;
    StereoDelay   mDelay;
    PlateReverb   mReverb;

    // Master volume smoothed to avoid clicks on value changes
    juce::SmoothedValue<float> mMasterVol;

    float mPitchBendRange = 2.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};

} // namespace drift
