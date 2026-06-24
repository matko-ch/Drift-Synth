#pragma once
#include <JuceHeader.h>

namespace drift {

// The Drift Field: a 2-D morph pad. Dragging the puck blends the patch between
// four sonic "scenes" (the engine reads MORPH_X / MORPH_Y). Hidden sliders give
// us host automation + undo for free.
class DriftFieldPad : public juce::Component {
public:
    explicit DriftFieldPad(juce::AudioProcessorValueTreeState& apvts);

    void paint(juce::Graphics&) override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;

private:
    [[nodiscard]] juce::Rectangle<float> padArea() const;
    void setFromMouse(const juce::MouseEvent&);

    juce::Slider mX, mY;  // hidden, attached to MORPH_X / MORPH_Y
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mXAtt, mYAtt;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DriftFieldPad)
};

} // namespace drift
