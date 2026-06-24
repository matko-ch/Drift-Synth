#pragma once
#include <JuceHeader.h>
#include "KnobComponent.h"

namespace drift {

// Advanced oscillator extras: FM, ring mod, noise, and the sub-oscillator.
class OscExtraPanel : public juce::GroupComponent {
public:
    explicit OscExtraPanel(juce::AudioProcessorValueTreeState& apvts);
    void resized() override;

private:
    KnobComponent mFM { "FM" }, mRing { "RING" }, mNoise { "NOISE" }, mSub { "SUB" };
    juce::ComboBox mSubShape, mSubOct;
    juce::Label    mSubShapeLbl, mSubOctLbl;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mShapeAtt, mOctAtt;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscExtraPanel)
};

} // namespace drift
