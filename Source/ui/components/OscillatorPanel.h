#pragma once
#include <JuceHeader.h>
#include "KnobComponent.h"

namespace drift {

class OscillatorPanel : public juce::GroupComponent {
public:
    OscillatorPanel(juce::AudioProcessorValueTreeState& apvts, int oscIndex);
    void resized() override;

private:
    juce::ComboBox mShapeCB;
    juce::Label    mShapeLabel;

    juce::ComboBox mOctaveCB;
    juce::Label    mOctaveLabel;

    KnobComponent mFineKnob   { "Fine"  };
    KnobComponent mLevelKnob  { "Level" };
    KnobComponent mPWKnob     { "PW"    };
    KnobComponent mPanKnob    { "Pan"   };

    juce::ToggleButton mSyncBtn { "Sync" };  // only for Osc2

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mShapeAtt, mOctaveAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>   mSyncAtt;

    int mOscIndex = 1;
};

} // namespace drift
