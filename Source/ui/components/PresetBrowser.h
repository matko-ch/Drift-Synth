#pragma once
#include <JuceHeader.h>
#include <vector>

namespace drift {

class PluginProcessor;

// Full-screen overlay listing every factory preset, grouped and sorted into
// categories. Opened by double-clicking the preset dropdown.
class PresetBrowser : public juce::Component {
public:
    explicit PresetBrowser(PluginProcessor& proc);

    std::function<void()> onClose;

    void show();                       // refresh highlight + become visible
    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent&) override;

private:
    [[nodiscard]] juce::Rectangle<int> panelBounds() const;
    void highlightCurrent();

    PluginProcessor& mProc;
    juce::Viewport   mViewport;
    juce::Component  mContent;
    juce::Label      mTitle, mHint;
    juce::TextButton mClose { "Close" };

    struct Section { juce::Label* header; std::vector<juce::TextButton*> buttons; };
    juce::OwnedArray<juce::Label>      mHeaders;
    juce::OwnedArray<juce::TextButton> mButtons;
    std::vector<Section>               mSections;
    std::vector<std::pair<juce::TextButton*, int>> mButtonIndex;  // button → preset idx

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetBrowser)
};

} // namespace drift
