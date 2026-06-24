#pragma once
#include <JuceHeader.h>
#include "../plugin/PluginProcessor.h"
#include "DriftLookAndFeel.h"
#include "components/PresetBar.h"
#include "components/DriftFieldPad.h"
#include "components/OrbitPanel.h"
#include "components/VibeConsole.h"
#include "components/SynthStrip.h"
#include "components/PerformBar.h"
#include "components/AdvancedPanel.h"
#include "components/KnobComponent.h"

namespace drift {

class PluginEditor final : public juce::AudioProcessorEditor,
                           private juce::Timer
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void updateView();

    PluginProcessor& mProcessor;

    PresetBar      mPresetBar;
    DriftFieldPad  mField;
    OrbitPanel     mOrbit;
    VibeConsole    mVibe;
    SynthStrip     mStrip;
    PerformBar     mPerform;

    juce::Viewport mAdvViewport;
    AdvancedPanel  mAdvanced;
    juce::TextButton mAdvToggle { "ADVANCED" };

    KnobComponent  mMasterVol { "VOL" };
    juce::MidiKeyboardComponent mKeyboard;

    bool  mShowAdvanced = false;
    float mMeterL = 0.0f, mMeterR = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};

} // namespace drift
