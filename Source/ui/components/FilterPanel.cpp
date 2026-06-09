#include "FilterPanel.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"

namespace drift {

FilterPanel::FilterPanel(juce::AudioProcessorValueTreeState& apvts, int filterIndex) {
    setText(filterIndex == 1 ? "FILTER 1" : "FILTER 2");

    mTypeCB.addItemList({"LP12","HP12","BP12","Notch","LP24","HP24","MoogLP"}, 1);
    mTypeLabel.setText("Type", juce::dontSendNotification);
    mTypeLabel.setJustificationType(juce::Justification::centred);
    mTypeLabel.setColour(juce::Label::textColourId, Colours::TextDim);
    mTypeLabel.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f)));
    addAndMakeVisible(mTypeCB);
    addAndMakeVisible(mTypeLabel);

    addAndMakeVisible(mCutoffKnob);
    addAndMakeVisible(mResKnob);
    addAndMakeVisible(mDriveKnob);
    addAndMakeVisible(mEnvKnob);
    addAndMakeVisible(mKeyTrkKnob);

    using namespace ParamID;
    if (filterIndex == 1) {
        mTypeAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, FILT1_TYPE, mTypeCB);
        mCutoffKnob.attach(apvts, FILT1_CUTOFF);
        mResKnob   .attach(apvts, FILT1_RES);
        mDriveKnob .attach(apvts, FILT1_DRIVE);
        mEnvKnob   .attach(apvts, FILT1_ENV_AMT);
        mKeyTrkKnob.attach(apvts, FILT1_KEYTRACK);
    } else {
        mTypeAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, FILT2_TYPE, mTypeCB);
        mCutoffKnob.attach(apvts, FILT2_CUTOFF);
        mResKnob   .attach(apvts, FILT2_RES);
        mDriveKnob .attach(apvts, FILT2_DRIVE);
        mEnvKnob   .attach(apvts, FILT2_ENV_AMT);
        mKeyTrkKnob.attach(apvts, FILT2_KEYTRACK);
    }
}

void FilterPanel::resized() {
    const auto b = getLocalBounds().reduced(8, 18);
    const int kw = 44, kh = 54;

    mTypeLabel.setBounds(b.getX(), b.getY(), 70, 14);
    mTypeCB.setBounds(b.getX(), b.getY() + 14, 80, 22);

    const int kY = b.getY() + 44;
    mCutoffKnob.setBounds(b.getX() + 0*(kw+4), kY, kw, kh);
    mResKnob   .setBounds(b.getX() + 1*(kw+4), kY, kw, kh);
    mDriveKnob .setBounds(b.getX() + 2*(kw+4), kY, kw, kh);
    mEnvKnob   .setBounds(b.getX() + 3*(kw+4), kY, kw, kh);
    mKeyTrkKnob.setBounds(b.getX() + 4*(kw+4), kY, kw, kh);
}

} // namespace drift
