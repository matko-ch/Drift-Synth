#include "LFOPanel.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"

namespace drift {

LFOPanel::LFOPanel(juce::AudioProcessorValueTreeState& apvts, int lfoIndex) {
    setText(lfoIndex == 1 ? "LFO 1" : "LFO 2");

    mShapeCB.addItemList({"Sine","Triangle","Saw","Square","S&H"}, 1);
    mShapeLabel.setText("Shape", juce::dontSendNotification);
    mShapeLabel.setJustificationType(juce::Justification::centred);
    mShapeLabel.setColour(juce::Label::textColourId, Colours::TextDim);
    mShapeLabel.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f)));
    addAndMakeVisible(mShapeCB);
    addAndMakeVisible(mShapeLabel);
    addAndMakeVisible(mRetrigBtn);
    addAndMakeVisible(mRateKnob);
    addAndMakeVisible(mDepthKnob);

    using namespace ParamID;
    if (lfoIndex == 1) {
        mShapeAtt  = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, LFO1_SHAPE, mShapeCB);
        mRetrigAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts, LFO1_RETRIG, mRetrigBtn);
        mRateKnob .attach(apvts, LFO1_RATE);
        mDepthKnob.attach(apvts, LFO1_DEPTH);
    } else {
        mShapeAtt  = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, LFO2_SHAPE, mShapeCB);
        mRetrigAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts, LFO2_RETRIG, mRetrigBtn);
        mRateKnob .attach(apvts, LFO2_RATE);
        mDepthKnob.attach(apvts, LFO2_DEPTH);
    }
}

void LFOPanel::resized() {
    const auto b = getLocalBounds().reduced(6, 18);
    mShapeLabel.setBounds(b.getX(), b.getY(), b.getWidth(), 14);
    mShapeCB.setBounds(b.getX(), b.getY() + 14, b.getWidth() - 44, 22);
    mRetrigBtn.setBounds(b.getRight() - 40, b.getY() + 16, 40, 18);

    const int kw = (b.getWidth() - 4) / 2;
    const int kY = b.getY() + 44;
    mRateKnob .setBounds(b.getX(),          kY, kw, b.getHeight() - 44);
    mDepthKnob.setBounds(b.getX() + kw + 4, kY, kw, b.getHeight() - 44);
}

} // namespace drift
