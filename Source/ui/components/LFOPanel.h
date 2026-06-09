#pragma once
#include <JuceHeader.h>
#include "KnobComponent.h"

namespace drift {

class LFOPanel : public juce::GroupComponent {
public:
    LFOPanel(juce::AudioProcessorValueTreeState& apvts, int lfoIndex);
    void resized() override;

private:
    juce::ComboBox    mShapeCB;
    juce::Label       mShapeLabel;
    juce::ToggleButton mRetrigBtn { "Retrig" };
    KnobComponent mRateKnob  { "Rate"  };
    KnobComponent mDepthKnob { "Depth" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mShapeAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>   mRetrigAtt;
};

} // namespace drift
