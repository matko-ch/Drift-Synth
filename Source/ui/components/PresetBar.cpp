#include "PresetBar.h"
#include "../../plugin/PluginProcessor.h"
#include "../../plugin/Presets.h"
#include "../DriftLookAndFeel.h"

namespace drift {

PresetBar::PresetBar(PluginProcessor& proc) : mProc(proc) {
    const auto& bank = getFactoryPresets();
    juce::String lastCat;
    for (int i = 0; i < (int)bank.size(); ++i) {
        if (juce::String(bank[(size_t)i].category) != lastCat) {
            lastCat = bank[(size_t)i].category;
            mCombo.addSectionHeading(lastCat);
        }
        mCombo.addItem(bank[(size_t)i].name, i + 1);
    }
    mCombo.setJustificationType(juce::Justification::centred);
    mCombo.setSelectedId(mProc.currentPreset + 1, juce::dontSendNotification);
    mCombo.onChange = [this] {
        const int id = mCombo.getSelectedId();
        if (id > 0) mProc.loadFactoryPreset(id - 1);
    };
    addAndMakeVisible(mCombo);

    mPrev.onClick = [this] { load(mProc.currentPreset - 1); };
    mNext.onClick = [this] { load(mProc.currentPreset + 1); };
    for (auto* b : { &mPrev, &mNext }) {
        b->setColour(juce::TextButton::buttonColourId, Colours::PanelBright);
        b->setColour(juce::TextButton::textColourOffId, Colours::Accent);
        addAndMakeVisible(b);
    }

    startTimerHz(8);
}

PresetBar::~PresetBar() { stopTimer(); }

void PresetBar::load(int index) {
    const int n = (int)getFactoryPresets().size();
    if (n == 0) return;
    index = (index % n + n) % n;  // wrap
    mProc.loadFactoryPreset(index);
    mCombo.setSelectedId(index + 1, juce::dontSendNotification);
}

void PresetBar::timerCallback() {
    // Keep the dropdown in sync if the preset changed elsewhere (DAW program, etc.)
    if (mCombo.getSelectedId() != mProc.currentPreset + 1)
        mCombo.setSelectedId(mProc.currentPreset + 1, juce::dontSendNotification);
}

void PresetBar::paint(juce::Graphics& g) {
    // Brand mark
    g.setColour(Colours::TextBright);
    g.setFont(juce::Font(juce::FontOptions{}.withHeight(26.0f).withStyle("Bold")));
    g.drawText("DRIFT", getLocalBounds().withTrimmedLeft(14).removeFromLeft(140),
               juce::Justification::centredLeft);
    g.setColour(Colours::Accent);
    g.setFont(juce::Font(juce::FontOptions{}.withHeight(9.0f)));
    g.drawText("DREAM SYNTHESIZER", getLocalBounds().withTrimmedLeft(15).withTrimmedTop(30).removeFromLeft(160),
               juce::Justification::topLeft);
}

void PresetBar::resized() {
    auto b = getLocalBounds().reduced(10, 12);
    b.removeFromLeft(160);                       // brand area
    auto browser = b.removeFromLeft(juce::jmin(360, b.getWidth())).withSizeKeepingCentre(
        juce::jmin(360, b.getWidth()), 28);
    // centre the browser group within the bar
    browser.setX(getWidth() / 2 - browser.getWidth() / 2);
    mPrev.setBounds(browser.removeFromLeft(30));
    browser.removeFromLeft(4);
    mNext.setBounds(browser.removeFromRight(30));
    browser.removeFromRight(4);
    mCombo.setBounds(browser);
}

} // namespace drift
