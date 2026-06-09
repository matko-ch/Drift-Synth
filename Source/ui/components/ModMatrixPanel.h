#pragma once
#include <JuceHeader.h>
#include "../DriftLookAndFeel.h"
#include "../../dsp/Constants.h"

namespace drift {

// One row in the mod matrix: Source combo, Destination combo, Amount knob.
class ModSlotRow : public juce::Component {
public:
    ModSlotRow(juce::AudioProcessorValueTreeState& apvts, int slotIndex);
    void resized() override;

private:
    juce::ComboBox mSrcCB, mDstCB;
    juce::Slider   mAmtSlider;
    juce::Label    mAmtLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mSrcAtt, mDstAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>   mAmtAtt;
};

// Scrollable panel containing all mod slots.
class ModMatrixPanel : public juce::GroupComponent {
public:
    ModMatrixPanel(juce::AudioProcessorValueTreeState& apvts);
    void resized() override;

private:
    std::array<std::unique_ptr<ModSlotRow>, kNumModSlots> mRows;
};

} // namespace drift
