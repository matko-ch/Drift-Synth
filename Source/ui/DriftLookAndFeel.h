#pragma once
#include <JuceHeader.h>

namespace drift {

// ── Drift colour palette (warm, vintage, analog) ─────────────────────────────
namespace Colours {
    inline const auto Background  = juce::Colour(0xFF1A1816u);
    inline const auto Panel       = juce::Colour(0xFF252220u);
    inline const auto PanelBright = juce::Colour(0xFF2E2B28u);
    inline const auto Accent      = juce::Colour(0xFFD97706u);  // amber
    inline const auto AccentDim   = juce::Colour(0xFF92500Cu);
    inline const auto KnobBody    = juce::Colour(0xFF3A3633u);
    inline const auto KnobRim     = juce::Colour(0xFF5A5653u);
    inline const auto TextBright  = juce::Colour(0xFFE7E5E4u);
    inline const auto TextMid     = juce::Colour(0xFF9C9790u);
    inline const auto TextDim     = juce::Colour(0xFF6B6560u);
    inline const auto Separator   = juce::Colour(0xFF333030u);
    inline const auto Green       = juce::Colour(0xFF65A30Du);
    inline const auto Red         = juce::Colour(0xFFDC2626u);
}

class DriftLookAndFeel : public juce::LookAndFeel_V4 {
public:
    DriftLookAndFeel();

    // Knobs
    void drawRotarySlider(juce::Graphics&, int x, int y, int w, int h,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider&) override;

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
