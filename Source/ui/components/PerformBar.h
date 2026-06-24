#pragma once
#include <JuceHeader.h>
#include "KnobComponent.h"

namespace drift {

// Performance strip: arpeggiator + chord-mode controls, sitting above the keyboard.
class PerformBar : public juce::Component {
public:
    explicit PerformBar(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using ComboAtt  = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAtt = juce::AudioProcessorValueTreeState::ButtonAttachment;

    // Arp
    juce::ToggleButton mArpOn { "ARP" };
    juce::ComboBox     mArpRate, mArpMode;
    KnobComponent      mArpOct { "OCT" }, mArpGate { "GATE" };
    std::unique_ptr<ButtonAtt> mArpOnAtt;
    std::unique_ptr<ComboAtt>  mArpRateAtt, mArpModeAtt;

    // Chord
    juce::ToggleButton mChordOn { "CHORD" };
    juce::ComboBox     mChordType;
    std::unique_ptr<ButtonAtt> mChordOnAtt;
    std::unique_ptr<ComboAtt>  mChordTypeAtt;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PerformBar)
};

} // namespace drift
