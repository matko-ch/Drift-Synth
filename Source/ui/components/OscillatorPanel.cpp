#include "OscillatorPanel.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"

namespace drift {

OscillatorPanel::OscillatorPanel(juce::AudioProcessorValueTreeState& apvts, int oscIndex)
    : mOscIndex(oscIndex)
{
    const bool isOsc2 = (oscIndex == 2);
    setText(isOsc2 ? "OSC 2" : "OSC 1");

    // Shape
    mShapeCB.addItemList({"Sine","Triangle","Saw","Square","Pulse"}, 1);
    mShapeLabel.setText("Shape", juce::dontSendNotification);
    mShapeLabel.setJustificationType(juce::Justification::centred);
    mShapeLabel.setColour(juce::Label::textColourId, Colours::TextDim);
    mShapeLabel.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f)));
    addAndMakeVisible(mShapeCB);
    addAndMakeVisible(mShapeLabel);

    // Octave
    mOctaveCB.addItemList({"-2","-1","0","+1","+2"}, 1);
    mOctaveLabel.setText("Oct", juce::dontSendNotification);
    mOctaveLabel.setJustificationType(juce::Justification::centred);
    mOctaveLabel.setColour(juce::Label::textColourId, Colours::TextDim);
    mOctaveLabel.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f)));
    addAndMakeVisible(mOctaveCB);
    addAndMakeVisible(mOctaveLabel);

    // Knobs
    addAndMakeVisible(mFineKnob);
    addAndMakeVisible(mLevelKnob);
    addAndMakeVisible(mPWKnob);
    addAndMakeVisible(mPanKnob);

    using namespace ParamID;
    if (isOsc2) {
        mShapeAtt  = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, OSC2_SHAPE, mShapeCB);
        mOctaveAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, OSC2_OCTAVE, mOctaveCB);
        mFineKnob.attach(apvts, OSC2_FINE);
        mLevelKnob.attach(apvts, OSC2_LEVEL);
        mPWKnob.attach(apvts, OSC2_PW);
        mPanKnob.attach(apvts, OSC2_PAN);

        mSyncBtn.setButtonText("Sync");
        mSyncAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts, OSC2_SYNC, mSyncBtn);
        addAndMakeVisible(mSyncBtn);
    } else {
        mShapeAtt  = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, OSC1_SHAPE, mShapeCB);
        mOctaveAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, OSC1_OCTAVE, mOctaveCB);
        mFineKnob.attach(apvts, OSC1_FINE);
        mLevelKnob.attach(apvts, OSC1_LEVEL);
        mPWKnob.attach(apvts, OSC1_PW);
        mPanKnob.attach(apvts, OSC1_PAN);
    }
}

void OscillatorPanel::resized() {
    const auto b = getLocalBounds().reduced(8, 18);
    const int kw = 48, kh = 58;

    // Top row: Shape combo + Octave combo
    const int cbH = 22, cbW = 70;
    mShapeLabel.setBounds(b.getX(), b.getY(), cbW, 14);
    mShapeCB.setBounds(b.getX(), b.getY() + 14, cbW, cbH);
    mOctaveLabel.setBounds(b.getX() + cbW + 6, b.getY(), 36, 14);
    mOctaveCB.setBounds(b.getX() + cbW + 6, b.getY() + 14, 36, cbH);

    // Sync button (Osc2 only)
    if (mOscIndex == 2)
        mSyncBtn.setBounds(b.getRight() - 36, b.getY() + 10, 36, 20);

    // Knob row
    const int kY = b.getY() + cbH + 20;
    mFineKnob .setBounds(b.getX() + 0*(kw+4), kY, kw, kh);
    mLevelKnob.setBounds(b.getX() + 1*(kw+4), kY, kw, kh);
    mPWKnob   .setBounds(b.getX() + 2*(kw+4), kY, kw, kh);
    mPanKnob  .setBounds(b.getX() + 3*(kw+4), kY, kw, kh);
}

} // namespace drift
