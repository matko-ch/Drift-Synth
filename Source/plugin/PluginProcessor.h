#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "../dsp/VoiceManager.h"
#include "../dsp/MacroEngine.h"
#include "../dsp/Arpeggiator.h"
#include "../dsp/Chords.h"
#include "../dsp/effects/Drive.h"
#include "../dsp/effects/Chorus.h"
#include "../dsp/effects/StereoDelay.h"
#include "../dsp/effects/PlateReverb.h"
#include "../dsp/effects/EQ.h"
#include "../dsp/effects/Phaser.h"
#include "../dsp/effects/Bitcrusher.h"
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

    int  getNumPrograms()    override;
    int  getCurrentProgram() override;
    void setCurrentProgram(int) override;
    const juce::String getProgramName(int) override;
    void changeProgramName(int, const juce::String&) override {}

    // Load a factory preset by index (also used by the editor's browser).
    void loadFactoryPreset(int index);

    void getStateInformation(juce::MemoryBlock& data) override;
    void setStateInformation(const void* data, int size) override;

    // ── Public state for the editor ───────────────────────────────────────────
    juce::AudioProcessorValueTreeState apvts;

    // Lock-free meter output (peak, updated each block)
    std::atomic<float> meterL { 0.0f }, meterR { 0.0f };

    // Currently selected factory preset (-1 = none / user-edited)
    int currentPreset = 0;

    // On-screen keyboard → merged into the MIDI stream each block.
    juce::MidiKeyboardState keyboardState;

private:
    void syncPatchFromParams() noexcept;
    MacroParams readMacroParams() const noexcept;
    void handleMidi(const juce::MidiMessage& msg) noexcept;
    void routeNoteOn(int note, float vel) noexcept;
    void routeNoteOff(int note) noexcept;

    VoiceManager  mVoiceManager;
    Arpeggiator   mArp;

    // Performance-mode state (read once per block)
    bool      mArpOn   = false;
    bool      mChordOn = false;
    ChordType mChordType = ChordType::Major;
    Drive         mDrive;
    Bitcrusher    mCrush;
    Phaser        mPhaser;
    Chorus        mChorus;
    StereoDelay   mDelay;
    PlateReverb   mReverb;
    EQ            mEQ;

    // Master volume smoothed to avoid clicks on value changes
    juce::SmoothedValue<float> mMasterVol;

    float mPitchBendRange = 2.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};

} // namespace drift
