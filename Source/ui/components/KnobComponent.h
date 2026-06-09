#pragma once
#include <JuceHeader.h>

namespace drift {

// Self-contained knob with label below and value tooltip.
// Attaches to APVTS via SliderAttachment.
class KnobComponent : public juce::Component {
public:
    KnobComponent(const juce::String& label, bool bipolar = false);

    // Attach to APVTS parameter
    void attach(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID);

    void resized() override;

    juce::Slider& getSlider() { return mSlider; }

private:
    juce::Slider mSlider;
    juce::Label  mLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mAttachment;
};

} // namespace drift
