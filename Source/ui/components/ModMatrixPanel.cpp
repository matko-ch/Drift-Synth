#include "ModMatrixPanel.h"
#include "../../plugin/Parameters.h"

namespace drift {

// ── ModSlotRow ────────────────────────────────────────────────────────────────

ModSlotRow::ModSlotRow(juce::AudioProcessorValueTreeState& apvts, int slotIndex) {
    const juce::StringArray sources{"None","AmpEnv","FiltEnv","ModEnv","LFO1","LFO2",
                                     "Velocity","ModWheel","KeyTrack","Aftertouch"};
    const juce::StringArray dests  {"None","MstrPitch","Osc1Pitch","Osc2Pitch",
                                     "Osc1PW","Osc2PW","OscMix","Flt1Cut","Flt1Res",
                                     "Flt2Cut","Flt2Res","AmpVol","Pan","LFO1Rate","LFO2Rate"};

    mSrcCB.addItemList(sources, 1);
    mDstCB.addItemList(dests,   1);
    addAndMakeVisible(mSrcCB);
    addAndMakeVisible(mDstCB);

    mAmtSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mAmtSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    mAmtSlider.setRange(-1.0, 1.0, 0.0);
    mAmtSlider.setPopupDisplayEnabled(true, false, nullptr);
    addAndMakeVisible(mAmtSlider);

    mSrcAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, ParamID::MOD_SRC[slotIndex], mSrcCB);
    mDstAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, ParamID::MOD_DST[slotIndex], mDstCB);
    mAmtAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, ParamID::MOD_AMT[slotIndex], mAmtSlider);
}

void ModSlotRow::resized() {
    const auto b = getLocalBounds();
    const int cbW = (b.getWidth() - 90) / 2;
    mSrcCB    .setBounds(0,           0, cbW,    b.getHeight());
    mDstCB    .setBounds(cbW + 4,     0, cbW,    b.getHeight());
    mAmtSlider.setBounds(cbW * 2 + 8, 0, 82,     b.getHeight());
}

// ── ModMatrixPanel ─────────────────────────────────────────────────────────────

ModMatrixPanel::ModMatrixPanel(juce::AudioProcessorValueTreeState& apvts) {
    setText("MOD MATRIX");
    for (int i = 0; i < kNumModSlots; ++i) {
        mRows[i] = std::make_unique<ModSlotRow>(apvts, i);
        addAndMakeVisible(*mRows[i]);
    }
}

void ModMatrixPanel::resized() {
    const auto b = getLocalBounds().reduced(8, 18);
    const int rowH = std::max(1, b.getHeight() / kNumModSlots);
    for (int i = 0; i < kNumModSlots; ++i)
        mRows[i]->setBounds(b.getX(), b.getY() + i * rowH, b.getWidth(), rowH - 2);
}

} // namespace drift
