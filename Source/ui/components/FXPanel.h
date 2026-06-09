#pragma once
#include <JuceHeader.h>
#include "KnobComponent.h"

namespace drift {

// Single effect sub-panel (Drive, Chorus, Delay, or Reverb).
class EffectSection : public juce::Component {
public:
    EffectSection(juce::AudioProcessorValueTreeState& apvts,
                  const char* sectionTitle,
                  const char* onParamID,
                  const char* param1ID, const char* label1,
                  const char* param2ID, const char* label2,
                  const char* param3ID, const char* label3);
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    juce::String      mTitle;
    juce::ToggleButton mOnBtn { "ON" };
    KnobComponent mKnob1, mKnob2, mKnob3;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mOnAtt;
};

// Full FX panel containing all four effects in a row.
class FXPanel : public juce::GroupComponent {
public:
    FXPanel(juce::AudioProcessorValueTreeState& apvts);
    void resized() override;

private:
    EffectSection mDrive, mChorus, mDelay, mReverb;
};

} // namespace drift
