#include "PresetBrowser.h"
#include "../../plugin/PluginProcessor.h"
#include "../../plugin/Presets.h"
#include "../DriftLookAndFeel.h"
#include <map>

namespace drift {

PresetBrowser::PresetBrowser(PluginProcessor& proc) : mProc(proc) {
    mTitle.setText("PRESET LIBRARY", juce::dontSendNotification);
    mTitle.setColour(juce::Label::textColourId, Colours::TextBright);
    mTitle.setFont(juce::Font(juce::FontOptions{}.withHeight(18.0f).withStyle("Bold")));
    addAndMakeVisible(mTitle);

    mHint.setText("click a preset to load  ·  click outside to close", juce::dontSendNotification);
    mHint.setColour(juce::Label::textColourId, Colours::TextDim);
    mHint.setFont(juce::Font(juce::FontOptions{}.withHeight(11.0f)));
    addAndMakeVisible(mHint);

    mClose.onClick = [this] { if (onClose) onClose(); };
    mClose.setColour(juce::TextButton::buttonColourId, Colours::PanelBright);
    mClose.setColour(juce::TextButton::textColourOffId, Colours::Accent);
    addAndMakeVisible(mClose);

    mViewport.setViewedComponent(&mContent, false);
    mViewport.setScrollBarsShown(true, false);
    addAndMakeVisible(mViewport);

    // Group presets by category (std::map keeps categories alphabetically sorted).
    const auto& bank = getFactoryPresets();
    std::map<juce::String, std::vector<int>> byCat;
    for (int i = 0; i < (int)bank.size(); ++i)
        byCat[juce::String(bank[(size_t)i].category)].push_back(i);

    for (auto& [cat, indices] : byCat) {
        auto* h = new juce::Label();
        h->setText(cat.toUpperCase() + "   (" + juce::String((int)indices.size()) + ")",
                   juce::dontSendNotification);
        h->setColour(juce::Label::textColourId, Colours::Accent2);
        h->setFont(juce::Font(juce::FontOptions{}.withHeight(12.5f).withStyle("Bold")));
        mHeaders.add(h);
        mContent.addAndMakeVisible(h);

        Section sec; sec.header = h;
        for (int idx : indices) {
            auto* b = new juce::TextButton(bank[(size_t)idx].name);
            b->setColour(juce::TextButton::buttonColourId, Colours::PanelBright);
            b->setColour(juce::TextButton::buttonOnColourId, Colours::AccentDim);
            b->setColour(juce::TextButton::textColourOffId, Colours::TextBright);
            b->setColour(juce::TextButton::textColourOnId, Colours::TextBright);
            b->onClick = [this, idx] { mProc.loadFactoryPreset(idx); if (onClose) onClose(); };
            mContent.addAndMakeVisible(b);
            mButtons.add(b);
            sec.buttons.push_back(b);
            mButtonIndex.emplace_back(b, idx);
        }
        mSections.push_back(std::move(sec));
    }

    setVisible(false);
}

juce::Rectangle<int> PresetBrowser::panelBounds() const {
    const int w = juce::jmin(getWidth() - 60, 1040);
    const int h = juce::jmin(getHeight() - 60, 680);
    return juce::Rectangle<int>((getWidth() - w) / 2, (getHeight() - h) / 2, w, h);
}

void PresetBrowser::show() {
    highlightCurrent();
    setVisible(true);
    toFront(true);
    resized();
}

void PresetBrowser::highlightCurrent() {
    for (auto& [btn, idx] : mButtonIndex)
        btn->setToggleState(idx == mProc.currentPreset, juce::dontSendNotification);
}

void PresetBrowser::mouseDown(const juce::MouseEvent& e) {
    if (!panelBounds().contains(e.getPosition()))
        if (onClose) onClose();
}

void PresetBrowser::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black.withAlpha(0.62f));   // dim backdrop
    const auto p = panelBounds().toFloat();
    g.setColour(Colours::Panel);
    g.fillRoundedRectangle(p, 14.0f);
    g.setColour(Colours::Accent.withAlpha(0.5f));
    g.drawRoundedRectangle(p.reduced(0.5f), 14.0f, 1.5f);
}

void PresetBrowser::resized() {
    auto panel = panelBounds().reduced(16);
    auto header = panel.removeFromTop(30);
    mTitle.setBounds(header.removeFromLeft(220));
    mClose.setBounds(header.removeFromRight(80).withSizeKeepingCentre(80, 26));
    mHint.setBounds(header);
    panel.removeFromTop(6);
    mViewport.setBounds(panel);

    // Lay out content: per category a header row then a grid of preset buttons.
    const int contentW = mViewport.getWidth() - 14;  // leave room for scrollbar
    const int gap = 6, btnH = 26, headH = 26;
    const int cols = juce::jmax(1, contentW / 200);
    const int btnW = (contentW - (cols - 1) * gap) / cols;

    int y = 0;
    for (auto& sec : mSections) {
        sec.header->setBounds(0, y, contentW, headH);
        y += headH;
        for (size_t i = 0; i < sec.buttons.size(); ++i) {
            const int c = (int)i % cols;
            const int r = (int)i / cols;
            sec.buttons[i]->setBounds(c * (btnW + gap), y + r * (btnH + gap), btnW, btnH);
        }
        const int rows = ((int)sec.buttons.size() + cols - 1) / cols;
        y += rows * (btnH + gap) + 10;
    }
    mContent.setSize(contentW, y + 8);
}

} // namespace drift
