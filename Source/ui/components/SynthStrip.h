#pragma once
#include <JuceHeader.h>
#include "KnobComponent.h"

namespace drift {

// The "keep it simple" strip: the essential synth controls (oscillators, one
// filter macro, amp envelope, drift) sitting under the creative surface.
// The full engine (2nd filter, LFOs, mod matrix, FX) lives in the Advanced view.
class SynthStrip : public juce::Component {
public:
    explicit SynthStrip(juce::AudioProcessorValueTreeState& apvts);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox mOsc1Shape, mOsc1Oct, mOsc2Shape, mOsc2Oct;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>, 4> mCombos;

    KnobComponent mDetune { "DETUNE" }, mMix { "MIX" },
                  mCutoff { "CUTOFF" }, mRes { "RESO" },
                  mA { "A" }, mD { "D" }, mS { "S" }, mR { "R" },
                  mDrift { "DRIFT" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthStrip)
};

} // namespace drift
