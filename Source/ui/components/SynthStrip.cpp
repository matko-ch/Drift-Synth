#include "SynthStrip.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"
#include <array>

namespace drift {

namespace {
constexpr int kPad = 10, kGap = 12, kCapH = 16;
struct Seg { int x, w; const char* cap; };

// Fixed, non-overlapping segment layout (left-aligned).
std::array<Seg, 6> segs() {
    std::array<Seg, 6> s{};
    const int w[6] = { 90, 150, 56, 110, 176, 56 };
    const char* caps[6] = { "OSC 1", "OSC 2", "MIX", "FILTER", "AMP ENV", "CHAR" };
    int x = kPad;
    for (int i = 0; i < 6; ++i) { s[i] = { x, w[i], caps[i] }; x += w[i] + kGap; }
    return s;
}
}

SynthStrip::SynthStrip(juce::AudioProcessorValueTreeState& apvts) {
    const juce::StringArray shapes { "Sine", "Triangle", "Saw", "Square", "Pulse" };
    const juce::StringArray octs   { "-2", "-1", "0", "+1", "+2" };

    auto initCombo = [this](juce::ComboBox& cb, const juce::StringArray& items) {
        cb.addItemList(items, 1);
        cb.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(cb);
    };
    initCombo(mOsc1Shape, shapes); initCombo(mOsc1Oct, octs);
    initCombo(mOsc2Shape, shapes); initCombo(mOsc2Oct, octs);

    mCombos[0] = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, ParamID::OSC1_SHAPE,  mOsc1Shape);
    mCombos[1] = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, ParamID::OSC1_OCTAVE, mOsc1Oct);
    mCombos[2] = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, ParamID::OSC2_SHAPE,  mOsc2Shape);
    mCombos[3] = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, ParamID::OSC2_OCTAVE, mOsc2Oct);

    for (auto* k : { &mDetune, &mMix, &mCutoff, &mRes, &mA, &mD, &mS, &mR, &mDrift })
        addAndMakeVisible(k);

    mDetune.attach(apvts, ParamID::OSC2_FINE);
    mMix   .attach(apvts, ParamID::OSC_MIX);
    mCutoff.attach(apvts, ParamID::FILT1_CUTOFF);
    mRes   .attach(apvts, ParamID::FILT1_RES);
    mA     .attach(apvts, ParamID::AMPENV_A);
    mD     .attach(apvts, ParamID::AMPENV_D);
    mS     .attach(apvts, ParamID::AMPENV_S);
    mR     .attach(apvts, ParamID::AMPENV_R);
    mDrift .attach(apvts, ParamID::DRIFT_AMOUNT);
}

void SynthStrip::paint(juce::Graphics& g) {
    g.setColour(Colours::Panel);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.0f);
    g.setColour(Colours::Separator);
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 10.0f, 1.2f);

    g.setFont(juce::Font(juce::FontOptions{}.withHeight(9.5f).withStyle("Bold")));
    const auto s = segs();
    for (int i = 0; i < (int)s.size(); ++i) {
        g.setColour(Colours::TextDim);
        g.drawText(s[i].cap, s[i].x, 5, s[i].w, 12, juce::Justification::centred);
        if (i > 0) {
            g.setColour(Colours::Separator.withAlpha(0.6f));
            g.drawVerticalLine(s[i].x - kGap / 2, 22.0f, (float)getHeight() - 8.0f);
        }
    }
}

void SynthStrip::resized() {
    const auto s = segs();
    const int y = 22, h = getHeight() - 28;
    const int cbH = 22;

    // OSC 1 — two stacked combos
    mOsc1Shape.setBounds(s[0].x + 2, y + 6,             s[0].w - 4, cbH);
    mOsc1Oct  .setBounds(s[0].x + 2, y + 6 + cbH + 6,   s[0].w - 4, cbH);

    // OSC 2 — two stacked combos + detune knob
    mOsc2Shape.setBounds(s[1].x + 2, y + 6,             92, cbH);
    mOsc2Oct  .setBounds(s[1].x + 2, y + 6 + cbH + 6,   92, cbH);
    mDetune   .setBounds(s[1].x + 98, y,                s[1].w - 100, h);

    // MIX
    mMix.setBounds(s[2].x + 3, y, s[2].w - 6, h);

    // FILTER — cutoff + reso
    mCutoff.setBounds(s[3].x,            y, (s[3].w - 4) / 2, h);
    mRes   .setBounds(s[3].x + s[3].w/2, y, (s[3].w - 4) / 2, h);

    // AMP ENV — A D S R
    const int ew = s[4].w / 4;
    mA.setBounds(s[4].x + 0 * ew, y, ew, h);
    mD.setBounds(s[4].x + 1 * ew, y, ew, h);
    mS.setBounds(s[4].x + 2 * ew, y, ew, h);
    mR.setBounds(s[4].x + 3 * ew, y, ew, h);

    // CHARACTER — drift
    mDrift.setBounds(s[5].x + 3, y, s[5].w - 6, h);
}

} // namespace drift
