#include "DriftFieldPad.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"

namespace drift {

DriftFieldPad::DriftFieldPad(juce::AudioProcessorValueTreeState& apvts) {
    for (auto* s : { &mX, &mY }) {
        s->setRange(0.0, 1.0, 0.0);
        s->setSliderStyle(juce::Slider::LinearHorizontal);
        s->onValueChange = [this] { repaint(); };
    }
    mXAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, ParamID::MORPH_X, mX);
    mYAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, ParamID::MORPH_Y, mY);
}

juce::Rectangle<float> DriftFieldPad::padArea() const {
    return getLocalBounds().toFloat().reduced(14.0f, 18.0f);
}

void DriftFieldPad::mouseDown(const juce::MouseEvent& e) { setFromMouse(e); }
void DriftFieldPad::mouseDrag(const juce::MouseEvent& e) { setFromMouse(e); }

void DriftFieldPad::setFromMouse(const juce::MouseEvent& e) {
    const auto a = padArea();
    const float nx = juce::jlimit(0.0f, 1.0f, (e.position.x - a.getX()) / a.getWidth());
    const float ny = juce::jlimit(0.0f, 1.0f, (e.position.y - a.getY()) / a.getHeight());
    mX.setValue(nx, juce::sendNotificationSync);
    mY.setValue(1.0f - ny, juce::sendNotificationSync);  // screen y is inverted
}

void DriftFieldPad::paint(juce::Graphics& g) {
    const auto a = padArea();

    // ── Backplate ───────────────────────────────────────────────────────────────
    g.setColour(Colours::Panel);
    g.fillRoundedRectangle(a, 10.0f);

    // ── Corner nebula glows (the four scenes) ───────────────────────────────────
    struct Corner { juce::Point<float> p; juce::Colour c; juce::String label; juce::Justification j; };
    const Corner corners[] = {
        { a.getBottomLeft(),  Colours::Glow,    "VELVET", juce::Justification::bottomLeft },
        { a.getBottomRight(), Colours::AccentDim,"SHADOW", juce::Justification::bottomRight },
        { a.getTopLeft(),     Colours::Accent2, "GLASS",  juce::Justification::topLeft },
        { a.getTopRight(),    Colours::Accent,  "HOLLOW", juce::Justification::topRight },
    };
    const float gr = juce::jmin(a.getWidth(), a.getHeight()) * 0.85f;
    for (auto& c : corners) {
        g.setGradientFill(juce::ColourGradient(c.c.withAlpha(0.38f), c.p.x, c.p.y,
                                               juce::Colours::transparentBlack,
                                               c.p.x + (c.p.x < a.getCentreX() ? gr : -gr),
                                               c.p.y + (c.p.y < a.getCentreY() ? gr : -gr), true));
        g.fillRect(a);
    }

    // ── Frame + grid crosshair ──────────────────────────────────────────────────
    g.setColour(Colours::Separator);
    g.drawRoundedRectangle(a.reduced(0.5f), 10.0f, 1.2f);
    g.setColour(Colours::Separator.withAlpha(0.5f));
    g.drawVerticalLine((int)a.getCentreX(), a.getY(), a.getBottom());
    g.drawHorizontalLine((int)a.getCentreY(), a.getX(), a.getRight());

    // ── Corner labels ───────────────────────────────────────────────────────────
    g.setFont(juce::Font(juce::FontOptions{}.withHeight(11.0f).withStyle("Bold")));
    for (auto& c : corners) {
        g.setColour(c.c.withAlpha(0.85f));
        g.drawText(c.label, a.reduced(8.0f), c.j);
    }

    // ── Puck ────────────────────────────────────────────────────────────────────
    const float px = a.getX() + (float)mX.getValue() * a.getWidth();
    const float py = a.getY() + (1.0f - (float)mY.getValue()) * a.getHeight();

    g.setGradientFill(juce::ColourGradient(Colours::TextBright.withAlpha(0.55f), px, py,
                                           juce::Colours::transparentBlack, px, py - 46.0f, true));
    g.fillEllipse(px - 38.0f, py - 38.0f, 76.0f, 76.0f);

    g.setColour(Colours::Accent);
    g.drawLine(px - 12.0f, py, px + 12.0f, py, 1.0f);
    g.drawLine(px, py - 12.0f, px, py + 12.0f, 1.0f);

    g.setColour(Colours::TextBright);
    g.fillEllipse(px - 7.0f, py - 7.0f, 14.0f, 14.0f);
    g.setColour(Colours::Accent);
    g.drawEllipse(px - 7.0f, py - 7.0f, 14.0f, 14.0f, 2.0f);

    // ── Title ───────────────────────────────────────────────────────────────────
    g.setColour(Colours::TextDim);
    g.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f).withStyle("Bold")));
    g.drawText("DRIFT FIELD", getLocalBounds().removeFromTop(16),
               juce::Justification::centred);
}

} // namespace drift
