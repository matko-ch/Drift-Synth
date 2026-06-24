#pragma once
#include <JuceHeader.h>
#include <array>

namespace drift {

// The Vibe Console: a bank of semantic faders (Dreaminess, Warmth, Air, …).
// Each fader drives many engine parameters via the macro layer.
class VibeConsole : public juce::Component {
public:
    explicit VibeConsole(juce::AudioProcessorValueTreeState& apvts);

    void paint(juce::Graphics&) override;
    void resized() override;

    static constexpr int kNumFaders = 7;

private:
    std::array<juce::Slider, kNumFaders> mFaders;
    std::array<juce::Label,  kNumFaders> mLabels;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,
               kNumFaders> mAtts;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibeConsole)
};

} // namespace drift
