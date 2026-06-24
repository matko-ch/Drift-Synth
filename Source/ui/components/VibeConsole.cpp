#include "VibeConsole.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"
#include <cmath>

namespace drift {

namespace {
struct Def { const char* id; const char* name; juce::Colour colour; };
const Def kDefs[VibeConsole::kNumFaders] = {
    { ParamID::VIBE_DREAM, "DREAM",  Colours::Glow    },
    { ParamID::VIBE_WARM,  "WARM",   Colours::Red     },
    { ParamID::VIBE_AIR,   "AIR",    Colours::Accent2 },
    { ParamID::VIBE_MOVE,  "MOVE",   Colours::Accent  },
    { ParamID::VIBE_GRIT,  "GRIT",   Colours::Red     },
    { ParamID::VIBE_WIDE,  "WIDTH",  Colours::Green   },
    { ParamID::VIBE_FOCUS, "FOCUS",  Colours::Accent  },
};
}

VibeConsole::VibeConsole(juce::AudioProcessorValueTreeState& apvts) {
    for (int i = 0; i < kNumFaders; ++i) {
        mFaders[i].setSliderStyle(juce::Slider::LinearVertical);
        mFaders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, true, 44, 15);
        mFaders[i].setColour(juce::Slider::textBoxTextColourId, Colours::TextMid);
        mFaders[i].setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        mFaders[i].textFromValueFunction = [](double v) { return juce::String((int)std::round(v * 100.0)); };
        mFaders[i].valueFromTextFunction = [](const juce::String& t) { return t.getDoubleValue() / 100.0; };
        mFaders[i].setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(mFaders[i]);

        mLabels[i].setText(kDefs[i].name, juce::dontSendNotification);
        mLabels[i].setJustificationType(juce::Justification::centred);
        mLabels[i].setColour(juce::Label::textColourId, kDefs[i].colour.brighter(0.2f));
        mLabels[i].setFont(juce::Font(juce::FontOptions{}.withHeight(11.5f).withStyle("Bold")));
        addAndMakeVisible(mLabels[i]);

        mAtts[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, kDefs[i].id, mFaders[i]);
    }
}

void VibeConsole::paint(juce::Graphics& g) {
    g.setColour(Colours::Panel);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.0f);
    g.setColour(Colours::Separator);
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 10.0f, 1.2f);

    g.setColour(Colours::TextDim);
    g.setFont(juce::Font(juce::FontOptions{}.withHeight(11.0f).withStyle("Bold")));
    g.drawText("VIBE", getLocalBounds().removeFromTop(20), juce::Justification::centred);
}

void VibeConsole::resized() {
    auto b = getLocalBounds().reduced(8, 4).withTrimmedTop(20);
    const int nameH = 16;
    const int colW  = b.getWidth() / kNumFaders;
    for (int i = 0; i < kNumFaders; ++i) {
        auto col = b.removeFromLeft(colW);
        mLabels[i].setBounds(col.removeFromBottom(nameH));
        mFaders[i].setBounds(col.reduced(3, 2));   // textbox lives inside the slider's bottom
    }
}

} // namespace drift
