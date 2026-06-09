#include "EnvelopePanel.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"

namespace drift {

EnvelopePanel::EnvelopePanel(juce::AudioProcessorValueTreeState& apvts, int envType) {
    const char* names[] = { "AMP ENV", "FILTER ENV", "MOD ENV" };
    setText(names[envType]);

    addAndMakeVisible(mAttack);
    addAndMakeVisible(mDecay);
    addAndMakeVisible(mSustain);
    addAndMakeVisible(mRelease);

    using namespace ParamID;
    switch (envType) {
        case 0:
            mAttack .attach(apvts, AMPENV_A);
            mDecay  .attach(apvts, AMPENV_D);
            mSustain.attach(apvts, AMPENV_S);
            mRelease.attach(apvts, AMPENV_R);
            break;
        case 1:
            mAttack .attach(apvts, FILTENV_A);
            mDecay  .attach(apvts, FILTENV_D);
            mSustain.attach(apvts, FILTENV_S);
            mRelease.attach(apvts, FILTENV_R);
            break;
        default:
            mAttack .attach(apvts, MODENV_A);
            mDecay  .attach(apvts, MODENV_D);
            mSustain.attach(apvts, MODENV_S);
            mRelease.attach(apvts, MODENV_R);
            break;
    }
}

void EnvelopePanel::resized() {
    const auto b = getLocalBounds().reduced(6, 18);
    const int kw = std::max(1, (b.getWidth() - 12) / 4);
    const int kh = b.getHeight();
    const int y  = b.getY();
    const int x  = b.getX();

    mAttack .setBounds(x + 0*(kw+4), y, kw, kh);
    mDecay  .setBounds(x + 1*(kw+4), y, kw, kh);
    mSustain.setBounds(x + 2*(kw+4), y, kw, kh);
    mRelease.setBounds(x + 3*(kw+4), y, kw, kh);
}

void EnvelopePanel::paint(juce::Graphics& g) {
    // Draw the group outline first
    juce::GroupComponent::paint(g);
}

} // namespace drift
