#pragma once
#include <JuceHeader.h>

namespace drift {

class PluginProcessor;

// A combo box that opens the full library browser on double-click while keeping
// the normal quick-pick dropdown on a single click.
class BrowsableComboBox : public juce::ComboBox {
public:
    std::function<void()> onDoubleClick;
    void mouseDown(const juce::MouseEvent& e) override {
        if (e.getNumberOfClicks() >= 2) {
            hidePopup();
            if (onDoubleClick) onDoubleClick();
            return;
        }
        juce::ComboBox::mouseDown(e);
    }
};

// Top bar: brand mark + factory-preset browser (prev / dropdown / next).
class PresetBar : public juce::Component, private juce::Timer {
public:
    explicit PresetBar(PluginProcessor& proc);
    ~PresetBar() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    // Invoked when the user double-clicks the dropdown (open full library).
    std::function<void()> onBrowseRequested;

private:
    void timerCallback() override;
    void load(int index);

    PluginProcessor& mProc;
    BrowsableComboBox mCombo;
    juce::TextButton mPrev { "<" }, mNext { ">" };
    juce::TextButton mBrowse { juce::String::fromUTF8("\xE2\x98\xB0") };  // ☰ list icon

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetBar)
};

} // namespace drift
