#pragma once
#include <JuceHeader.h>
#include <array>

namespace drift {

// Orbit: five satellite nodes around a central core. Each node's distance from
// the core is its amount (Motion / Space / Texture / Drive / Tone). Drag a node
// in or out to dial the macro. Hidden sliders bridge to the APVTS params.
class OrbitPanel : public juce::Component {
public:
    explicit OrbitPanel(juce::AudioProcessorValueTreeState& apvts);

    void paint(juce::Graphics&) override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;
    void mouseUp(const juce::MouseEvent&) override;

    static constexpr int kNumNodes = 5;

private:
    struct Node { const char* label; juce::Colour colour; float angle; };

    [[nodiscard]] juce::Point<float> core() const;
    [[nodiscard]] float minR() const;
    [[nodiscard]] float maxR() const;
    [[nodiscard]] juce::Point<float> nodePos(int i) const;

    std::array<Node, kNumNodes>   mNodes;
    std::array<juce::Slider, kNumNodes> mVals;     // hidden, attached
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,
               kNumNodes> mAtts;
    int mActive = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OrbitPanel)
};

} // namespace drift
