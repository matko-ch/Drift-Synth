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
    const float arcR  = radius + 3.5f;

    // ── Outer glow (nebula bloom) ───────────────────────────────────────────────
    {
        juce::ColourGradient glow(Colours::Accent.withAlpha(0.28f), cx, cy,
                                   juce::Colours::transparentBlack, cx, cy - arcR * 1.6f, true);
        g.setGradientFill(glow);
        g.fillEllipse(cx - arcR * 1.5f, cy - arcR * 1.5f, arcR * 3.0f, arcR * 3.0f);
    }

    // ── Body gradient ─────────────────────────────────────────────────────────
    {
        juce::ColourGradient grad(Colours::KnobRim, cx - radius * 0.4f, cy - radius * 0.5f,
                                   Colours::KnobBody, cx + radius * 0.4f, cy + radius * 0.5f, false);
        g.setGradientFill(grad);
        g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);
    }

    // ── Inner sheen ─────────────────────────────────────────────────────────────
    g.setColour(juce::Colours::white.withAlpha(0.05f));
    g.fillEllipse(cx - radius * 0.7f, cy - radius * 0.8f, radius * 1.4f, radius * 1.0f);

    // ── Outer rim ─────────────────────────────────────────────────────────────
    g.setColour(Colours::Background.darker(0.4f));
    g.drawEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f, 1.5f);

    // ── Arc track ─────────────────────────────────────────────────────────────
    juce::Path track;
    track.addArc(cx - arcR, cy - arcR, arcR * 2.0f, arcR * 2.0f,
                 startAngle, endAngle, true);
    g.setColour(Colours::Separator);
    g.strokePath(track, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved,
                                              juce::PathStrokeType::rounded));

    // ── Filled arc (violet→pink gradient) ───────────────────────────────────────
    if (std::abs(angle - startAngle) > 0.01f) {
        juce::Path fill;
        fill.addArc(cx - arcR, cy - arcR, arcR * 2.0f, arcR * 2.0f,
                    startAngle, angle, true);
        g.setGradientFill(juce::ColourGradient(
            Colours::Accent2, cx - arcR, cy, Colours::Glow, cx + arcR, cy, false));
        g.strokePath(fill, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved,
                                                 juce::PathStrokeType::rounded));
    }

    // ── Indicator line ──────────────────────────────────────────────────────────
    const float innerR = radius * 0.35f, outerR = radius * 0.82f;
    const float sa = std::sin(angle), ca = -std::cos(angle);
    juce::Line<float> ind(cx + sa * innerR, cy + ca * innerR,
                          cx + sa * outerR, cy + ca * outerR);
    g.setColour(Colours::TextBright);
    g.drawLine(ind, 2.4f);
}

// ── Fader (vertical linear slider) ──────────────────────────────────────────────
void DriftLookAndFeel::drawLinearSlider(juce::Graphics& g,
    int x, int y, int w, int h,
    float sliderPos, float /*minPos*/, float /*maxPos*/,
    juce::Slider::SliderStyle /*style*/, juce::Slider& /*slider*/)
{
    const float cx = x + w * 0.5f;
    const float trackW = 5.0f;
    const float top = y + 6.0f, bot = y + h - 6.0f;

    // Track
    juce::Rectangle<float> track(cx - trackW * 0.5f, top, trackW, bot - top);
    g.setColour(Colours::Separator);
    g.fillRoundedRectangle(track, trackW * 0.5f);

    // Filled portion (from bottom up to the thumb)
    const float ty = juce::jlimit(top, bot, sliderPos);
    juce::Rectangle<float> fill(cx - trackW * 0.5f, ty, trackW, bot - ty);
    g.setGradientFill(juce::ColourGradient(Colours::Glow, cx, bot,
                                           Colours::Accent2, cx, top, false));
    g.fillRoundedRectangle(fill, trackW * 0.5f);

    // Thumb (glowing cap)
    const float thumbR = 9.0f;
    {
        juce::ColourGradient glow(Colours::Accent.withAlpha(0.5f), cx, ty,
                                   juce::Colours::transparentBlack, cx, ty - thumbR * 2.2f, true);
        g.setGradientFill(glow);
        g.fillEllipse(cx - thumbR * 2.0f, ty - thumbR * 2.0f, thumbR * 4.0f, thumbR * 4.0f);
    }
    g.setColour(Colours::TextBright);
    g.fillEllipse(cx - thumbR, ty - thumbR, thumbR * 2.0f, thumbR * 2.0f);
    g.setColour(Colours::Accent);
    g.drawEllipse(cx - thumbR, ty - thumbR, thumbR * 2.0f, thumbR * 2.0f, 2.0f);
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
