#include "AdvancedPanel.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"

namespace drift {

AdvancedPanel::AdvancedPanel(juce::AudioProcessorValueTreeState& apvts)
    : mOsc1(apvts, 1), mOsc2(apvts, 2), mOscExtra(apvts)
    , mFilter1(apvts, 1), mFilter2(apvts, 2)
    , mAmpEnv(apvts, 0), mFilterEnv(apvts, 1), mModEnv(apvts, 2)
    , mLFO1(apvts, 1), mLFO2(apvts, 2)
    , mFX(apvts)
{
    for (auto* c : std::initializer_list<juce::Component*>{
            &mOsc1, &mOsc2, &mOscExtra, &mFilter1, &mFilter2, &mAmpEnv, &mFilterEnv,
            &mModEnv, &mLFO1, &mLFO2, &mFX })
        addAndMakeVisible(c);

    mMasterGroup.setText("MASTER / VOICE");
    addAndMakeVisible(mMasterGroup);
    for (auto* k : { &mMasterVol, &mMasterTune, &mGlide })
        mMasterGroup.addAndMakeVisible(k);
    mMasterVol .attach(apvts, ParamID::MASTER_VOL);
    mMasterTune.attach(apvts, ParamID::MASTER_TUNE);
    mGlide     .attach(apvts, ParamID::GLIDE_TIME);

    mMasterGroup.addAndMakeVisible(mGlideOn);
    mMasterGroup.addAndMakeVisible(mRouting);
    mGlideAtt   = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, ParamID::GLIDE_ON, mGlideOn);
    mRoutingAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, ParamID::FILT_ROUTING, mRouting);

    mPoly.addItemList({ "1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16" }, 1);
    mMasterGroup.addAndMakeVisible(mPoly);
    mPolyAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, ParamID::POLY_VOICES, mPoly);
    mPolyLabel.setText("Poly", juce::dontSendNotification);
    mPolyLabel.setJustificationType(juce::Justification::centred);
    mPolyLabel.setColour(juce::Label::textColourId, Colours::TextDim);
    mPolyLabel.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f)));
    mMasterGroup.addAndMakeVisible(mPolyLabel);
}

void AdvancedPanel::paint(juce::Graphics& g) {
    g.fillAll(Colours::Background.withAlpha(0.0f));
}

void AdvancedPanel::resized() {
    auto b = getLocalBounds().reduced(4);
    const int pad = 6;
    auto threeCol = [&](juce::Rectangle<int> row, juce::Component& a, juce::Component& c, juce::Component& d) {
        const int w = (row.getWidth() - 2 * pad) / 3;
        a.setBounds(row.removeFromLeft(w)); row.removeFromLeft(pad);
        c.setBounds(row.removeFromLeft(w)); row.removeFromLeft(pad);
        d.setBounds(row);
    };

    // Row 1: Osc1 | Osc2 | Osc Extras
    threeCol(b.removeFromTop(150), mOsc1, mOsc2, mOscExtra);
    b.removeFromTop(pad);

    // Row 2: Filter1 | Filter2 | Master
    threeCol(b.removeFromTop(150), mFilter1, mFilter2, mMasterGroup);
    b.removeFromTop(pad);

    // Row 3: Amp / Filter / Mod envelopes (six knobs each)
    threeCol(b.removeFromTop(150), mAmpEnv, mFilterEnv, mModEnv);
    b.removeFromTop(pad);

    // Row 4: LFO1 | LFO2
    auto row4 = b.removeFromTop(130);
    const int half = (row4.getWidth() - pad) / 2;
    mLFO1.setBounds(row4.removeFromLeft(half)); row4.removeFromLeft(pad);
    mLFO2.setBounds(row4);
    b.removeFromTop(pad);

    // Row 5: FX (full width, two internal rows)
    auto row5 = b.removeFromTop(220);
    mFX.setBounds(row5);

    // Master group internals
    auto mg = mMasterGroup.getLocalBounds().reduced(8, 20);
    const int kw = 46, kh = 64;
    mMasterVol .setBounds(mg.getX(),            mg.getY(), kw, kh);
    mMasterTune.setBounds(mg.getX() + kw + 4,   mg.getY(), kw, kh);
    mGlide     .setBounds(mg.getX() + 2*(kw+4), mg.getY(), kw, kh);
    auto rowB = mg.withTrimmedTop(kh + 4);
    mGlideOn.setBounds(rowB.getX(),       rowB.getY(), 70, 20);
    mRouting.setBounds(rowB.getX() + 78,  rowB.getY(), 70, 20);
    mPolyLabel.setBounds(rowB.getX(),     rowB.getY() + 24, 40, 14);
    mPoly.setBounds(rowB.getX() + 42,     rowB.getY() + 24, 50, 22);
}

} // namespace drift
