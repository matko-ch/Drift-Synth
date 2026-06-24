#include "FXPanel.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"

namespace drift {

// ── EffectSection ─────────────────────────────────────────────────────────────

EffectSection::EffectSection(juce::AudioProcessorValueTreeState& apvts,
                              const char* title,
                              const char* onID,
                              const char* p1, const char* l1,
                              const char* p2, const char* l2,
                              const char* p3, const char* l3)
    : mTitle(title)
    , mKnob1(l1), mKnob2(l2), mKnob3(l3)
{
    mOnBtn.setButtonText("ON");
    addAndMakeVisible(mOnBtn);
    addAndMakeVisible(mKnob1);
    addAndMakeVisible(mKnob2);
    addAndMakeVisible(mKnob3);

    mOnAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, onID, mOnBtn);
    mKnob1.attach(apvts, p1);
    mKnob2.attach(apvts, p2);
    mKnob3.attach(apvts, p3);
}

void EffectSection::paint(juce::Graphics& g) {
    g.setColour(Colours::Separator);
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 4.0f, 1.0f);

    g.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f).withStyle("Bold")));
    g.setColour(Colours::Accent);
    g.drawText(mTitle, getLocalBounds().removeFromTop(16), juce::Justification::centred);
}

void EffectSection::resized() {
    const auto b = getLocalBounds().reduced(4).withTrimmedTop(16);
    mOnBtn.setBounds(b.getX() + (b.getWidth() - 32) / 2, b.getY(), 32, 18);

    const int kw = (b.getWidth() - 8) / 3;
    const int kY = b.getY() + 24;
    const int kh = b.getHeight() - 24;
    mKnob1.setBounds(b.getX() + 0*(kw+4), kY, kw, kh);
    mKnob2.setBounds(b.getX() + 1*(kw+4), kY, kw, kh);
    mKnob3.setBounds(b.getX() + 2*(kw+4), kY, kw, kh);
}

// ── FXPanel ───────────────────────────────────────────────────────────────────

FXPanel::FXPanel(juce::AudioProcessorValueTreeState& apvts)
    : mDrive (apvts, "DRIVE",  ParamID::DRIVE_ON,
              ParamID::DRIVE_AMOUNT, "Drive",
              ParamID::DRIVE_MIX,    "Mix",
              ParamID::DRIVE_AMOUNT, "Tone")
    , mCrush (apvts, "LOFI",   ParamID::CRUSH_ON,
              ParamID::CRUSH_BITS,   "Bits",
              ParamID::CRUSH_AMT,    "Crush",
              ParamID::CRUSH_MIX,    "Mix")
    , mPhaser(apvts, "PHASER", ParamID::PHASER_ON,
              ParamID::PHASER_RATE,  "Rate",
              ParamID::PHASER_DEPTH, "Depth",
              ParamID::PHASER_MIX,   "Mix")
    , mChorus(apvts, "CHORUS", ParamID::CHORUS_ON,
              ParamID::CHORUS_RATE,  "Rate",
              ParamID::CHORUS_DEPTH, "Depth",
              ParamID::CHORUS_MIX,   "Mix")
    , mDelay (apvts, "DELAY",  ParamID::DELAY_ON,
              ParamID::DELAY_TIME,   "Time",
              ParamID::DELAY_FDBK,   "Fdbk",
              ParamID::DELAY_MIX,    "Mix")
    , mReverb(apvts, "REVERB", ParamID::REVERB_ON,
              ParamID::REVERB_SIZE,  "Size",
              ParamID::REVERB_DAMP,  "Damp",
              ParamID::REVERB_MIX,   "Mix")
    , mEQ    (apvts, "EQ",     ParamID::EQ_ON,
              ParamID::EQ_LOW,       "Low",
              ParamID::EQ_MID,       "Mid",
              ParamID::EQ_HIGH,      "High")
{
    setText("EFFECTS");
    for (auto* s : { &mDrive, &mCrush, &mPhaser, &mChorus, &mDelay, &mReverb, &mEQ })
        addAndMakeVisible(s);
}

void FXPanel::resized() {
    auto b = getLocalBounds().reduced(8, 18);
    EffectSection* order[7] = { &mDrive, &mCrush, &mPhaser, &mChorus, &mDelay, &mReverb, &mEQ };

    // Two rows: 4 on top, 3 below.
    const int gap = 4;
    const int rowH = (b.getHeight() - gap) / 2;
    const int topN = 4, botN = 3;
    const int topW = (b.getWidth() - (topN - 1) * gap) / topN;
    const int botW = (b.getWidth() - (botN - 1) * gap) / botN;

    for (int i = 0; i < topN; ++i)
        order[i]->setBounds(b.getX() + i * (topW + gap), b.getY(), topW, rowH);
    for (int i = 0; i < botN; ++i)
        order[topN + i]->setBounds(b.getX() + i * (botW + gap), b.getY() + rowH + gap, botW, rowH);
}

} // namespace drift
