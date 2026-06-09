#pragma once
#include <JuceHeader.h>
#include "KnobComponent.h"

namespace drift {

class FilterPanel : public juce::GroupComponent {
public:
    FilterPanel(juce::AudioProcessorValueTreeState& apvts, int filterIndex);
    void resized() override;

private:
    juce::ComboBox mTypeCB;
    juce::Label    mTypeLabel;

    KnobComponent mCutoffKnob  { "Cutoff" };
    KnobComponent mResKnob     { "Res"    };
    KnobComponent mDriveKnob   { "Drive"  };
    KnobComponent mEnvKnob     { "Env"    };
    KnobComponent mKeyTrkKnob  { "KeyTrk" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mTypeAtt;
};

} // namespace drift
