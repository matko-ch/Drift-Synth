#include "OscExtraPanel.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"

namespace drift {

OscExtraPanel::OscExtraPanel(juce::AudioProcessorValueTreeState& apvts) {
    setText("OSC EXTRAS");

    for (auto* k : { &mFM, &mRing, &mNoise, &mSub })
        addAndMakeVisible(k);
    mFM   .attach(apvts, ParamID::OSC_FM);
    mRing .attach(apvts, ParamID::OSC_RING);
    mNoise.attach(apvts, ParamID::OSC_NOISE);
    mSub  .attach(apvts, ParamID::SUB_LEVEL);

    mSubShape.addItemList({ "Sine", "Triangle", "Square" }, 1);
    mSubOct.addItemList({ "-2", "-1" }, 1);
    addAndMakeVisible(mSubShape);
    addAndMakeVisible(mSubOct);
    mShapeAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, ParamID::SUB_SHAPE, mSubShape);
    mOctAtt   = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, ParamID::SUB_OCTAVE, mSubOct);

    auto lbl = [this](juce::Label& l, const char* t) {
        l.setText(t, juce::dontSendNotification);
        l.setJustificationType(juce::Justification::centredRight);
        l.setColour(juce::Label::textColourId, Colours::TextDim);
        l.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f)));
        addAndMakeVisible(l);
    };
    lbl(mSubShapeLbl, "Sub wave");
    lbl(mSubOctLbl,   "Sub oct");
}

void OscExtraPanel::resized() {
    auto b = getLocalBounds().reduced(8, 18);
    const int kw = 48, kh = 58;
    auto knobs = b.removeFromTop(kh);
    mFM   .setBounds(knobs.removeFromLeft(kw)); knobs.removeFromLeft(2);
    mRing .setBounds(knobs.removeFromLeft(kw)); knobs.removeFromLeft(2);
    mNoise.setBounds(knobs.removeFromLeft(kw)); knobs.removeFromLeft(2);
    mSub  .setBounds(knobs.removeFromLeft(kw));

    b.removeFromTop(6);
    auto r1 = b.removeFromTop(22);
    mSubShapeLbl.setBounds(r1.removeFromLeft(56));
    mSubShape.setBounds(r1.removeFromLeft(80));
    auto r2 = b.removeFromTop(22);
    mSubOctLbl.setBounds(r2.removeFromLeft(56));
    mSubOct.setBounds(r2.removeFromLeft(50));
}

} // namespace drift
