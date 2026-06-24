#include "PerformBar.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"
#include "../../dsp/Chords.h"

namespace drift {

PerformBar::PerformBar(juce::AudioProcessorValueTreeState& apvts) {
    auto setupCombo = [this](juce::ComboBox& cb, const juce::StringArray& items) {
        cb.addItemList(items, 1);
        cb.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(cb);
    };

    // ── Arp ──────────────────────────────────────────────────────────────────────
    addAndMakeVisible(mArpOn);
    setupCombo(mArpRate, { "1/2","1/4","1/4T","1/8","1/8T","1/16","1/16T","1/32" });
    setupCombo(mArpMode, { "Up","Down","Up/Down","Down/Up","As Played","Random" });
    addAndMakeVisible(mArpOct);
    addAndMakeVisible(mArpGate);
    mArpOnAtt   = std::make_unique<ButtonAtt>(apvts, ParamID::ARP_ON,   mArpOn);
    mArpRateAtt = std::make_unique<ComboAtt>(apvts, ParamID::ARP_RATE, mArpRate);
    mArpModeAtt = std::make_unique<ComboAtt>(apvts, ParamID::ARP_MODE, mArpMode);
    mArpOct.attach(apvts, ParamID::ARP_OCT);
    mArpGate.attach(apvts, ParamID::ARP_GATE);

    // ── Chord ────────────────────────────────────────────────────────────────────
    addAndMakeVisible(mChordOn);
    juce::StringArray chordNames;
    for (auto* n : kChordNames) chordNames.add(n);
    setupCombo(mChordType, chordNames);
    mChordOnAtt   = std::make_unique<ButtonAtt>(apvts, ParamID::CHORD_ON, mChordOn);
    mChordTypeAtt = std::make_unique<ComboAtt>(apvts, ParamID::CHORD_TYPE, mChordType);
}

void PerformBar::paint(juce::Graphics& g) {
    g.setColour(Colours::Panel);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.0f);
    g.setColour(Colours::Separator);
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 8.0f, 1.0f);

    // Divider between the arp group and the chord group.
    g.setColour(Colours::Separator.withAlpha(0.7f));
    const float dx = 470.0f;
    if (getWidth() > dx + 220.0f)
        g.drawVerticalLine((int)dx, 8.0f, (float)getHeight() - 8.0f);
}

void PerformBar::resized() {
    auto b = getLocalBounds().reduced(8, 6);
    const int h = b.getHeight();

    auto place = [&](juce::Component& c, int w, int gap = 6) {
        c.setBounds(b.removeFromLeft(w).withSizeKeepingCentre(w, juce::jmin(h, 30)));
        b.removeFromLeft(gap);
    };
    auto placeKnob = [&](juce::Component& c, int w, int gap = 6) {
        c.setBounds(b.removeFromLeft(w)); b.removeFromLeft(gap);
    };

    place(mArpOn, 56);
    place(mArpRate, 64);
    place(mArpMode, 92);
    placeKnob(mArpOct, 40);
    placeKnob(mArpGate, 40);

    b.removeFromLeft(34);  // gap to the divider

    place(mChordOn, 64);
    place(mChordType, 100);
}

} // namespace drift
