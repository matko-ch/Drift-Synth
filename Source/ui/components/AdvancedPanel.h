#pragma once
#include <JuceHeader.h>
#include "OscillatorPanel.h"
#include "OscExtraPanel.h"
#include "FilterPanel.h"
#include "EnvelopePanel.h"
#include "LFOPanel.h"
#include "FXPanel.h"
#include "KnobComponent.h"

namespace drift {

// The deep-edit view: every raw engine module, reachable on demand. Lives inside
// a Viewport so the creative front panel stays uncluttered. This is what makes
// "everything achievable manually, no preset required" true.
class AdvancedPanel : public juce::Component {
public:
    explicit AdvancedPanel(juce::AudioProcessorValueTreeState& apvts);

    void paint(juce::Graphics&) override;
    void resized() override;

    static constexpr int kContentHeight = 840;

private:
    OscillatorPanel mOsc1, mOsc2;
    OscExtraPanel   mOscExtra;
    FilterPanel     mFilter1, mFilter2;
    EnvelopePanel   mAmpEnv, mFilterEnv, mModEnv;
    LFOPanel        mLFO1, mLFO2;
    FXPanel         mFX;

    // Master / voice section
    juce::GroupComponent mMasterGroup;
    KnobComponent mMasterVol  { "VOL" }, mMasterTune { "TUNE" }, mGlide { "GLIDE" };
    juce::ToggleButton mGlideOn { "Glide" }, mRouting { "Parallel" };
    juce::ComboBox mPoly;
    juce::Label    mPolyLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>   mGlideAtt, mRoutingAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mPolyAtt;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdvancedPanel)
};

} // namespace drift
