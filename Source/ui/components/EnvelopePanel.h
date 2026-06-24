#pragma once
#include <JuceHeader.h>
#include "KnobComponent.h"

namespace drift {

// ADSR display + knobs for one envelope (Amp, Filter, or Mod).
class EnvelopePanel : public juce::GroupComponent {
public:
    // envType: 0=Amp, 1=Filter, 2=Mod
    EnvelopePanel(juce::AudioProcessorValueTreeState& apvts, int envType);
    void resized() override;
    void paint(juce::Graphics&) override;

private:
    KnobComponent mAttack  { "A" };
    KnobComponent mHold    { "H" };
    KnobComponent mDecay   { "D" };
    KnobComponent mSustain { "S" };
    KnobComponent mRelease { "R" };
    KnobComponent mCurve   { "CRV" };
};

} // namespace drift
