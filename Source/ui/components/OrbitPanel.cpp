#include "OrbitPanel.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"

namespace drift {

OrbitPanel::OrbitPanel(juce::AudioProcessorValueTreeState& apvts) {
    const char* ids[kNumNodes] = {
        ParamID::ORBIT_MOTION, ParamID::ORBIT_SPACE, ParamID::ORBIT_TEXTURE,
        ParamID::ORBIT_DRIVE,  ParamID::ORBIT_TONE
    };
    mNodes = {{
        { "MOTION",  Colours::Accent2, 0.0f },
        { "SPACE",   Colours::Accent,  0.0f },
        { "TEXTURE", Colours::Glow,    0.0f },
        { "DRIVE",   Colours::Red,     0.0f },
        { "TONE",    Colours::Green,   0.0f },
    }};
    // Distribute evenly, first node at the top.
    for (int i = 0; i < kNumNodes; ++i)
        mNodes[i].angle = -juce::MathConstants<float>::halfPi
                        + (float)i * juce::MathConstants<float>::twoPi / (float)kNumNodes;

    for (int i = 0; i < kNumNodes; ++i) {
        mVals[i].setRange(0.0, 1.0, 0.0);
        mVals[i].onValueChange = [this] { repaint(); };
        mAtts[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, ids[i], mVals[i]);
    }
}

juce::Point<float> OrbitPanel::core() const {
    auto b = getLocalBounds().toFloat();
    return { b.getCentreX(), b.getCentreY() + 4.0f };
}
float OrbitPanel::minR() const { return 26.0f; }
float OrbitPanel::maxR() const {
    auto b = getLocalBounds().toFloat().reduced(20.0f);
    return juce::jmin(b.getWidth(), b.getHeight()) * 0.5f - 6.0f;
}

juce::Point<float> OrbitPanel::nodePos(int i) const {
    const float r = minR() + (float)mVals[i].getValue() * (maxR() - minR());
    const auto c = core();
    return { c.x + std::cos(mNodes[i].angle) * r,
             c.y + std::sin(mNodes[i].angle) * r };
}

void OrbitPanel::mouseDown(const juce::MouseEvent& e) {
    int best = -1; float bestD = 1e9f;
    for (int i = 0; i < kNumNodes; ++i) {
        const float d = e.position.getDistanceFrom(nodePos(i));
        if (d < bestD) { bestD = d; best = i; }
    }
    mActive = best;
    mouseDrag(e);
}

void OrbitPanel::mouseDrag(const juce::MouseEvent& e) {
    if (mActive < 0) return;
    const auto c = core();
    // Project the mouse vector onto the node's fixed radial direction.
    const float dx = e.position.x - c.x, dy = e.position.y - c.y;
    const float proj = dx * std::cos(mNodes[mActive].angle) + dy * std::sin(mNodes[mActive].angle);
    const float v = juce::jlimit(0.0f, 1.0f, (proj - minR()) / (maxR() - minR()));
    mVals[mActive].setValue(v, juce::sendNotificationSync);
}

void OrbitPanel::mouseUp(const juce::MouseEvent&) { mActive = -1; }

void OrbitPanel::paint(juce::Graphics& g) {
    const auto c = core();

    // Title
    g.setColour(Colours::TextDim);
    g.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f).withStyle("Bold")));
    g.drawText("ORBIT", getLocalBounds().removeFromTop(16), juce::Justification::centred);

    // Faint orbit rings
    g.setColour(Colours::Separator.withAlpha(0.5f));
    for (float t = 0.33f; t <= 1.01f; t += 0.33f) {
        const float r = minR() + t * (maxR() - minR());
        g.drawEllipse(c.x - r, c.y - r, r * 2.0f, r * 2.0f, 1.0f);
    }

    // Core
    g.setGradientFill(juce::ColourGradient(Colours::TextBright.withAlpha(0.5f), c.x, c.y,
                                           juce::Colours::transparentBlack, c.x, c.y - 28.0f, true));
    g.fillEllipse(c.x - 22.0f, c.y - 22.0f, 44.0f, 44.0f);
    g.setColour(Colours::PanelBright);
    g.fillEllipse(c.x - 10.0f, c.y - 10.0f, 20.0f, 20.0f);
    g.setColour(Colours::Accent);
    g.drawEllipse(c.x - 10.0f, c.y - 10.0f, 20.0f, 20.0f, 1.5f);

    // Nodes
    g.setFont(juce::Font(juce::FontOptions{}.withHeight(9.0f).withStyle("Bold")));
    for (int i = 0; i < kNumNodes; ++i) {
        const auto p = nodePos(i);
        const float v = (float)mVals[i].getValue();
        const auto col = mNodes[i].colour;

        // connecting beam
        g.setColour(col.withAlpha(0.25f + 0.45f * v));
        g.drawLine(c.x, c.y, p.x, p.y, 1.5f + 2.0f * v);

        // glow
        g.setGradientFill(juce::ColourGradient(col.withAlpha(0.15f + 0.5f * v), p.x, p.y,
                                               juce::Colours::transparentBlack, p.x, p.y - 24.0f, true));
        g.fillEllipse(p.x - 18.0f, p.y - 18.0f, 36.0f, 36.0f);

        // node body
        const float nr = 8.0f + 3.0f * v;
        g.setColour(col);
        g.fillEllipse(p.x - nr, p.y - nr, nr * 2.0f, nr * 2.0f);
        g.setColour(Colours::Background.darker(0.3f));
        g.drawEllipse(p.x - nr, p.y - nr, nr * 2.0f, nr * 2.0f, 1.5f);

        // label (pushed slightly past the node, radially)
        const auto lp = c.getPointOnCircumference(0, 0); // unused placeholder
        juce::ignoreUnused(lp);
        const float lx = c.x + std::cos(mNodes[i].angle) * (maxR() + 12.0f);
        const float ly = c.y + std::sin(mNodes[i].angle) * (maxR() + 12.0f);
        g.setColour(v > 0.02f ? col : Colours::TextDim);
        g.drawText(mNodes[i].label,
                   juce::Rectangle<float>(lx - 36.0f, ly - 7.0f, 72.0f, 14.0f),
                   juce::Justification::centred);
    }
}

} // namespace drift
