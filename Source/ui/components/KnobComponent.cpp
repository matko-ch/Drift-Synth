#include "KnobComponent.h"
#include "../DriftLookAndFeel.h"

namespace drift {

KnobComponent::KnobComponent(const juce::String& label, bool /*bipolar*/) {
    mSlider.setSliderStyle(juce::Slider::Rotary);
    mSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    mSlider.setPopupDisplayEnabled(true, false, nullptr);
    addAndMakeVisible(mSlider);

    mLabel.setText(label, juce::dontSendNotification);
    mLabel.setJustificationType(juce::Justification::centred);
    mLabel.setColour(juce::Label::textColourId, Colours::TextDim);
    mLabel.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f)));
    addAndMakeVisible(mLabel);
}

void KnobComponent::attach(juce::AudioProcessorValueTreeState& apvts,
                            const juce::String& paramID) {
    mAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, paramID, mSlider);
}

void KnobComponent::resized() {
    const int labelH = 14;
    mSlider.setBounds(0, 0, getWidth(), getHeight() - labelH);
    mLabel.setBounds(0, getHeight() - labelH, getWidth(), labelH);
}

} // namespace drift
