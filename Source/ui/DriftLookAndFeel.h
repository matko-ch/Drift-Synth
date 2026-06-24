#pragma once
#include <JuceHeader.h>

namespace drift {

// ── Drift colour palette (cosmic / dreamy nebula) ────────────────────────────
namespace Colours {
    inline const auto Background  = juce::Colour(0xFF0E0B1Eu);  // deep space indigo
    inline const auto BgTop       = juce::Colour(0xFF16204Fu);  // deep blue
    inline const auto BgMid       = juce::Colour(0xFF2A1558u);  // indigo-purple
    inline const auto BgBottom    = juce::Colour(0xFF3C1247u);  // deep purple / magenta
    inline const auto Panel       = juce::Colour(0xFF1A1638u);
    inline const auto PanelBright = juce::Colour(0xFF262148u);
    inline const auto Accent      = juce::Colour(0xFF9D7BFFu);  // pastel violet
    inline const auto AccentDim   = juce::Colour(0xFF5B47A8u);
    inline const auto Accent2     = juce::Colour(0xFF5BE9D6u);  // aurora cyan
    inline const auto Glow        = juce::Colour(0xFFFF7BD5u);  // nebula pink
    inline const auto KnobBody    = juce::Colour(0xFF2A2550u);
    inline const auto KnobRim     = juce::Colour(0xFF4A4180u);
    inline const auto TextBright  = juce::Colour(0xFFEDEAFFu);
    inline const auto TextMid     = juce::Colour(0xFF9E98C8u);
    inline const auto TextDim     = juce::Colour(0xFF6A6498u);
    inline const auto Separator   = juce::Colour(0xFF2E2856u);
    inline const auto Green       = juce::Colour(0xFF5BE9A6u);
    inline const auto Red         = juce::Colour(0xFFFF6B9Du);
}

class DriftLookAndFeel : public juce::LookAndFeel_V4 {
public:
    DriftLookAndFeel();

    // Knobs
    void drawRotarySlider(juce::Graphics&, int x, int y, int w, int h,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider&) override;

    // Faders (vertical linear sliders)
    void drawLinearSlider(juce::Graphics&, int x, int y, int w, int h,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle, juce::Slider&) override;

    // Buttons
    void drawToggleButton(juce::Graphics&, juce::ToggleButton&,
                          bool highlight, bool down) override;

    // ComboBox
    void drawComboBox(juce::Graphics&, int w, int h, bool down,
                      int bx, int by, int bw, int bh,
                      juce::ComboBox&) override;
    void positionComboBoxText(juce::ComboBox&, juce::Label&) override;

    // Labels
    void drawLabel(juce::Graphics&, juce::Label&) override;

    // Group box (section headers)
    void drawGroupComponentOutline(juce::Graphics&, int w, int h,
                                   const juce::String& text,
                                   const juce::Justification&,
                                   juce::GroupComponent&) override;

    juce::Font getLabelFont(juce::Label&) override;
    juce::Font getComboBoxFont(juce::ComboBox&) override;

    static DriftLookAndFeel& instance() {
        static DriftLookAndFeel s_instance;
        return s_instance;
    }

private:
    juce::Font mFontRegular;
    juce::Font mFontBold;
};

} // namespace drift
