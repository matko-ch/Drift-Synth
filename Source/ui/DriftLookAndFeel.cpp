#include "DriftLookAndFeel.h"
#include <cmath>

namespace drift {

DriftLookAndFeel::DriftLookAndFeel() {
    // Font fallback to system default — swap in a custom typeface here if needed
    mFontRegular = juce::Font(juce::FontOptions{}.withHeight(12.0f));
    mFontBold    = juce::Font(juce::FontOptions{}.withHeight(12.0f)
                              .withStyle("Bold"));

    // Global colour overrides
    setColour(juce::ResizableWindow::backgroundColourId, Colours::Background);
    setColour(juce::Slider::rotarySliderFillColourId,    Colours::Accent);
    setColour(juce::Slider::thumbColourId,               Colours::AccentDim);
    setColour(juce::Label::textColourId,                 Colours::TextMid);
    setColour(juce::ComboBox::backgroundColourId,        Colours::PanelBright);
    setColour(juce::ComboBox::textColourId,              Colours::TextBright);
    setColour(juce::ComboBox::outlineColourId,           Colours::Separator);
    setColour(juce::ComboBox::arrowColourId,             Colours::Accent);
    setColour(juce::PopupMenu::backgroundColourId,       Colours::Panel);
    setColour(juce::PopupMenu::textColourId,             Colours::TextBright);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, Colours::AccentDim);
    setColour(juce::PopupMenu::highlightedTextColourId,  Colours::TextBright);
    setColour(juce::ToggleButton::textColourId,          Colours::TextMid);
    setColour(juce::ToggleButton::tickColourId,          Colours::Accent);
    setColour(juce::ToggleButton::tickDisabledColourId,  Colours::TextDim);
}

// ── Knob ──────────────────────────────────────────────────────────────────────
void DriftLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y, int w, int h,
    float sliderPos, float startAngle, float endAngle,
    juce::Slider& /*slider*/)
{
    const float cx = x + w * 0.5f;
    const float cy = y + h * 0.5f;
    const float radius = (std::min(w, h) * 0.5f) - 4.0f;
    if (radius <= 0.0f) return;

    const float angle = startAngle + sliderPos * (endAngle - startAngle);

    // ── Shadow ────────────────────────────────────────────────────────────────
    {
        juce::ColourGradient shadow(juce::Colours::black.withAlpha(0.45f),
                                     cx, cy + radius * 0.3f,
                                     juce::Colours::transparentBlack,
                                     cx, cy + radius * 1.4f, true);
        g.setGradientFill(shadow);
        g.fillEllipse(cx - radius, cy - radius + 4.0f, radius * 2.0f, radius * 2.0f);
    }

    // ── Body gradient ─────────────────────────────────────────────────────────
    {
        juce::ColourGradient grad(Colours::KnobRim, cx - radius * 0.3f, cy - radius * 0.4f,
                                   Colours::KnobBody, cx + radius * 0.3f, cy + radius * 0.4f, false);
        g.setGradientFill(grad);
        g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);
    }

    // ── Outer rim ─────────────────────────────────────────────────────────────
    g.setColour(juce::Colour(0xFF222020u));
    g.drawEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f, 1.5f);

    // ── Arc track ─────────────────────────────────────────────────────────────
    const float arcR = radius + 3.5f;
    juce::Path track;
    track.addArc(cx - arcR, cy - arcR, arcR * 2.0f, arcR * 2.0f,
                 startAngle, endAngle, true);
    g.setColour(Colours::Separator);
    g.strokePath(track, juce::PathStrokeType(2.5f, juce::PathStrokeType::curved,
                                              juce::PathStrokeType::rounded));

    // ── Filled arc ────────────────────────────────────────────────────────────
    if (std::abs(angle - startAngle) > 0.01f) {
        juce::Path fill;
        fill.addArc(cx - arcR, cy - arcR, arcR * 2.0f, arcR * 2.0f,
                    startAngle, angle, true);
        g.setColour(Colours::Accent);
        g.strokePath(fill, juce::PathStrokeType(2.5f, juce::PathStrokeType::curved,
                                                 juce::PathStrokeType::rounded));
    }

    // ── Indicator dot ─────────────────────────────────────────────────────────
    const float dotR  = radius * 0.12f;
    const float dotDist = radius * 0.62f;
    const float dotX  = cx + std::sin(angle) * dotDist;
    const float dotY  = cy - std::cos(angle) * dotDist;
    g.setColour(Colours::TextBright);
    g.fillEllipse(dotX - dotR, dotY - dotR, dotR * 2.0f, dotR * 2.0f);
}

// ── Toggle button ─────────────────────────────────────────────────────────────
void DriftLookAndFeel::drawToggleButton(juce::Graphics& g,
    juce::ToggleButton& btn, bool highlight, bool /*down*/)
{
    const bool on = btn.getToggleState();
    const auto bounds = btn.getLocalBounds().toFloat();
    const float corner = 3.0f;

    g.setColour(on ? Colours::AccentDim : Colours::PanelBright);
    g.fillRoundedRectangle(bounds, corner);

    g.setColour(on ? Colours::Accent : Colours::Separator);
    g.drawRoundedRectangle(bounds.reduced(0.5f), corner, 1.0f);

    if (highlight) {
        g.setColour(Colours::Accent.withAlpha(0.15f));
        g.fillRoundedRectangle(bounds, corner);
    }

    g.setColour(on ? Colours::TextBright : Colours::TextMid);
    g.setFont(mFontRegular.withHeight(11.0f));
    g.drawText(btn.getButtonText(), bounds, juce::Justification::centred);
}

// ── ComboBox ──────────────────────────────────────────────────────────────────
void DriftLookAndFeel::drawComboBox(juce::Graphics& g,
    int w, int h, bool /*down*/,
    int bx, int by, int bw, int bh,
    juce::ComboBox& /*cb*/)
{
    const juce::Rectangle<float> box(0, 0, (float)w, (float)h);
    g.setColour(Colours::PanelBright);
    g.fillRoundedRectangle(box, 3.0f);
    g.setColour(Colours::Separator);
    g.drawRoundedRectangle(box.reduced(0.5f), 3.0f, 1.0f);

    // Arrow
    juce::Path arrow;
    const float arrowX = bx + bw * 0.5f;
    const float arrowY = by + bh * 0.5f;
    arrow.addTriangle(arrowX - 4.0f, arrowY - 2.0f,
                      arrowX + 4.0f, arrowY - 2.0f,
                      arrowX,        arrowY + 3.0f);
    g.setColour(Colours::Accent);
    g.fillPath(arrow);
}

void DriftLookAndFeel::positionComboBoxText(juce::ComboBox& cb, juce::Label& label) {
    label.setBounds(4, 1, cb.getWidth() - 22, cb.getHeight() - 2);
    label.setFont(mFontRegular.withHeight(11.0f));
}

// ── Labels ────────────────────────────────────────────────────────────────────
void DriftLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label) {
    g.fillAll(label.findColour(juce::Label::backgroundColourId));
    if (!label.isBeingEdited()) {
        const auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        const auto font  = getLabelFont(label);
        g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
        g.setFont(font);
        const auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
        g.drawFittedText(label.getText(), textArea, label.getJustificationType(), 1, 1.0f);
    }
}

juce::Font DriftLookAndFeel::getLabelFont(juce::Label& label) {
    return label.getFont().withHeight(11.0f);
}

juce::Font DriftLookAndFeel::getComboBoxFont(juce::ComboBox&) {
    return mFontRegular.withHeight(11.0f);
}

// ── Group / section outline ───────────────────────────────────────────────────
void DriftLookAndFeel::drawGroupComponentOutline(juce::Graphics& g,
    int w, int h, const juce::String& text,
    const juce::Justification& just,
    juce::GroupComponent&)
{
    const float indent = 8.0f;
    const float textH  = 14.0f;
    const juce::Rectangle<float> area(0.5f, textH * 0.5f,
                                       w - 1.0f, h - textH * 0.5f - 0.5f);

    g.setColour(Colours::Separator);
    g.drawRoundedRectangle(area, 4.0f, 1.0f);

    const float textW = std::min(static_cast<float>(w) - indent * 2.0f,
                                  mFontBold.withHeight(11.0f).getStringWidthFloat(text) + 8.0f);
    const float textX = (just == juce::Justification::centred)
        ? (w - textW) * 0.5f : indent;

    g.setColour(Colours::Panel);
    g.fillRect(textX, 0.0f, textW, textH);

    g.setFont(mFontBold.withHeight(11.0f));
    g.setColour(Colours::Accent);
    g.drawText(text, juce::Rectangle<float>(textX + 4.0f, 0.0f, textW - 8.0f, textH),
               juce::Justification::centredLeft);
}

} // namespace drift
