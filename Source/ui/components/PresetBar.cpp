#include "PresetBar.h"
#include "../../plugin/PluginProcessor.h"
#include "../../plugin/Presets.h"
#include "../DriftLookAndFeel.h"
#include <map>
#include <vector>

namespace drift {

PresetBar::PresetBar(PluginProcessor& proc) : mProc(proc) {
    // Group the dropdown by category (alphabetical) so it matches the full
    // browser. Item IDs stay tied to the preset's bank index, so ordering of
    // the display is independent of how presets map to programs.
    const auto& bank = getFactoryPresets();
    std::map<juce::String, std::vector<int>> byCat;
    for (int i = 0; i < (int)bank.size(); ++i)
        byCat[juce::String(bank[(size_t)i].category)].push_back(i);
    for (auto& [cat, idxs] : byCat) {
        mCombo.addSectionHeading(cat);
        for (int idx : idxs)
            mCombo.addItem(bank[(size_t)idx].name, idx + 1);
    }
    mCombo.setJustificationType(juce::Justification::centred);
    mCombo.setSelectedId(mProc.currentPreset + 1, juce::dontSendNotification);
    mCombo.onChange = [this] {
        const int id = mCombo.getSelectedId();
        if (id > 0) mProc.loadFactoryPreset(id - 1);
    };
    mCombo.onDoubleClick = [this] { if (onBrowseRequested) onBrowseRequested(); };
    addAndMakeVisible(mCombo);

    mPrev.onClick = [this] { load(mProc.currentPreset - 1); };
    mNext.onClick = [this] { load(mProc.currentPreset + 1); };
    mBrowse.onClick = [this] { if (onBrowseRequested) onBrowseRequested(); };
    for (auto* b : { &mPrev, &mNext, &mBrowse }) {
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
    const int bx = 16;
    juce::Rectangle<int> driftR(bx, 2, 210, 40);

    // Marker-style brand. "Ink Free" is Windows' built-in felt-tip marker face;
    // it gives the logo that loose, hand-drawn racing-decal feel.
    juce::Font marker(juce::FontOptions{}.withName("Ink Free").withHeight(38.0f).withStyle("Bold"));
    g.setFont(marker);

    // Drop shadow for depth.
    g.setColour(juce::Colours::black.withAlpha(0.45f));
    g.drawText("Drift", driftR.translated(2, 2), juce::Justification::centredLeft);

    // Gradient fill (cyan → violet → pink) across the word.
    juce::ColourGradient grad(Colours::Accent2, (float)bx, 0.0f,
                              Colours::Glow, (float)bx + 150.0f, 0.0f, false);
    grad.addColour(0.5, Colours::Accent);
    g.setGradientFill(grad);
    g.drawText("Drift", driftR, juce::Justification::centredLeft);

    // Subtitle, clearly below the logo (letter-spaced for a refined look).
    g.setColour(Colours::TextMid);
    g.setFont(juce::Font(juce::FontOptions{}.withHeight(9.5f).withStyle("Bold")));
    g.drawText("D R E A M   S Y N T H E S I Z E R",
               bx + 3, 43, 240, 12, juce::Justification::topLeft);
}

void PresetBar::resized() {
    auto b = getLocalBounds().reduced(10, 12);
    b.removeFromLeft(160);                       // brand area
    const int groupW = juce::jmin(420, b.getWidth());
    auto group = b.withSizeKeepingCentre(groupW, 28);
    group.setX(getWidth() / 2 - groupW / 2);     // centre the group within the bar

    mPrev.setBounds(group.removeFromLeft(30));
    group.removeFromLeft(4);
    mBrowse.setBounds(group.removeFromRight(36)); // ☰ full library
    group.removeFromRight(4);
    mNext.setBounds(group.removeFromRight(30));
    group.removeFromRight(4);
    mCombo.setBounds(group);
}

} // namespace drift
