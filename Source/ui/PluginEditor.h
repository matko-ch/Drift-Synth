#pragma once
#include <JuceHeader.h>
#include "../plugin/PluginProcessor.h"
#include "DriftLookAndFeel.h"
#include "components/OscillatorPanel.h"
#include "components/FilterPanel.h"
#include "components/EnvelopePanel.h"
#include "components/LFOPanel.h"
#include "components/ModMatrixPanel.h"
#include "components/FXPanel.h"
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

    PluginProcessor& mProcessor;

    // ── Sections ──────────────────────────────────────────────────────────────
    OscillatorPanel mOsc1Panel, mOsc2Panel;
    FilterPanel     mFilter1Panel, mFilter2Panel;

    // Filter routing toggle
    juce::ToggleButton mFilterRoutingBtn { "Parallel" };
    juce::Label        mFilterRoutingLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mRoutingAtt;

    // Unison section
    juce::GroupComponent mUnisonGroup;
    KnobComponent mUnisonVoicesKnob { "Voices" };
    KnobComponent mUnisonDetuneKnob { "Detune" };
    KnobComponent mUnisonSpreadKnob { "Spread" };

    // Osc mix
    KnobComponent mOscMixKnob { "Mix" };

    // Master section
    juce::GroupComponent mMasterGroup;
    KnobComponent mMasterVolKnob   { "Volume" };
    KnobComponent mMasterTuneKnob  { "Tune"   };
    KnobComponent mGlideKnob       { "Glide"  };
    juce::ToggleButton mGlideOnBtn { "Glide" };
    juce::ComboBox     mPolyVoicesCB;
    juce::Label        mPolyLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>   mGlideOnAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mPolyAtt;

    // Envelopes
    EnvelopePanel mAmpEnvPanel, mFilterEnvPanel, mModEnvPanel;

    // LFOs
    LFOPanel mLFO1Panel, mLFO2Panel;

    // Mod Matrix
    ModMatrixPanel mModMatrix;

    // FX
    FXPanel mFXPanel;

    // Meters
    float mMeterLDisplay = 0.0f, mMeterRDisplay = 0.0f;

    // Header
    juce::Label mTitleLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};

} // namespace drift
