#include "EnvelopePanel.h"
#include "../../plugin/Parameters.h"
#include "../DriftLookAndFeel.h"

namespace drift {

EnvelopePanel::EnvelopePanel(juce::AudioProcessorValueTreeState& apvts, int envType) {
    const char* names[] = { "AMP ENV", "FILTER ENV", "MOD ENV" };
    setText(names[envType]);

    for (auto* k : { &mAttack, &mHold, &mDecay, &mSustain, &mRelease, &mCurve })
        addAndMakeVisible(k);

    using namespace ParamID;
    const char* a[3]  = { AMPENV_A, FILTENV_A, MODENV_A };
    const char* h[3]  = { AMPENV_H, FILTENV_H, MODENV_H };
    const char* d[3]  = { AMPENV_D, FILTENV_D, MODENV_D };
    const char* s[3]  = { AMPENV_S, FILTENV_S, MODENV_S };
    const char* r[3]  = { AMPENV_R, FILTENV_R, MODENV_R };
    const char* c[3]  = { AMPENV_CURVE, FILTENV_CURVE, MODENV_CURVE };
    const int t = juce::jlimit(0, 2, envType);
    mAttack .attach(apvts, a[t]);
    mHold   .attach(apvts, h[t]);
    mDecay  .attach(apvts, d[t]);
    mSustain.attach(apvts, s[t]);
    mRelease.attach(apvts, r[t]);
    mCurve  .attach(apvts, c[t]);
}

void EnvelopePanel::resized() {
    const auto b = getLocalBounds().reduced(6, 18);
    KnobComponent* knobs[6] = { &mAttack, &mHold, &mDecay, &mSustain, &mRelease, &mCurve };
    const int n = 6;
    const int kw = std::max(1, (b.getWidth() - (n - 1) * 3) / n);
    const int kh = b.getHeight();
    for (int i = 0; i < n; ++i)
        knobs[i]->setBounds(b.getX() + i * (kw + 3), b.getY(), kw, kh);
}

void EnvelopePanel::paint(juce::Graphics& g) {
    // Draw the group outline first
    juce::GroupComponent::paint(g);
}

} // namespace drift
