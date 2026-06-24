#pragma once
#include <JuceHeader.h>

namespace drift {

class PluginProcessor;

// Top bar: brand mark + factory-preset browser (prev / dropdown / next).
class PresetBar : public juce::Component, private juce::Timer {
public:
    explicit PresetBar(PluginProcessor& proc);
    ~PresetBar() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void load(int index);

    PluginProcessor& mProc;
    juce::ComboBox   mCombo;
    juce::TextButton mPrev { "<" }, mNext { ">" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetBar)
};

} // namespace drift
