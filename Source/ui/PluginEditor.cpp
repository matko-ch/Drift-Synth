#include "PluginEditor.h"

namespace drift {

static constexpr int kEditorW = 1120;
static constexpr int kEditorH = 880;

PluginEditor::PluginEditor(PluginProcessor& proc)
    : AudioProcessorEditor(proc)
    , mProcessor(proc)
    , mPresetBar(proc)
    , mField(proc.apvts)
    , mOrbit(proc.apvts)
    , mVibe(proc.apvts)
    , mStrip(proc.apvts)
    , mPerform(proc.apvts)
    , mAdvanced(proc.apvts)
    , mBrowser(proc)
    , mKeyboard(proc.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setLookAndFeel(&DriftLookAndFeel::instance());

    addAndMakeVisible(mPresetBar);
    addAndMakeVisible(mField);
    addAndMakeVisible(mOrbit);
    addAndMakeVisible(mVibe);
    addAndMakeVisible(mStrip);
    addAndMakeVisible(mPerform);

    mMasterVol.attach(proc.apvts, ParamID::MASTER_VOL);
    addAndMakeVisible(mMasterVol);

    mAdvToggle.setClickingTogglesState(true);
    mAdvToggle.setColour(juce::TextButton::buttonColourId,   Colours::PanelBright);
    mAdvToggle.setColour(juce::TextButton::buttonOnColourId, Colours::AccentDim);
    mAdvToggle.setColour(juce::TextButton::textColourOffId,  Colours::TextMid);
    mAdvToggle.setColour(juce::TextButton::textColourOnId,   Colours::TextBright);
    mAdvToggle.onClick = [this] {
        mShowAdvanced = mAdvToggle.getToggleState();
        updateView();
    };
    addAndMakeVisible(mAdvToggle);

    mAdvViewport.setViewedComponent(&mAdvanced, false);
    mAdvViewport.setScrollBarsShown(true, false);
    addChildComponent(mAdvViewport);

    // Full-library browser (opened by double-clicking the preset dropdown).
    addChildComponent(mBrowser);
    mPresetBar.onBrowseRequested = [this] {
        mBrowser.setBounds(getLocalBounds());
        mBrowser.show();
    };
    mBrowser.onClose = [this] { mBrowser.setVisible(false); };

    // Keyboard: full 88-key range available; how many octaves show is driven by
    // the window width (MidiKeyboardComponent fills its bounds from the lowest
    // visible key). Scroll buttons let you reach the rest.
    mKeyboard.setAvailableRange(21, 108);
    mKeyboard.setLowestVisibleKey(36);
    mKeyboard.setKeyWidth(24.0f);
    mKeyboard.setScrollButtonsVisible(true);
    mKeyboard.setColour(juce::MidiKeyboardComponent::whiteNoteColourId, Colours::TextBright);
    mKeyboard.setColour(juce::MidiKeyboardComponent::blackNoteColourId, Colours::Background.brighter(0.1f));
    mKeyboard.setColour(juce::MidiKeyboardComponent::keySeparatorLineColourId, Colours::Separator);
    mKeyboard.setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId, Colours::Accent.withAlpha(0.7f));
    mKeyboard.setColour(juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, Colours::Accent2.withAlpha(0.35f));
    mKeyboard.setColour(juce::MidiKeyboardComponent::shadowColourId, Colours::Background);
    addAndMakeVisible(mKeyboard);

    setResizable(true, true);
    setResizeLimits(980, 740, 1600, 1180);
    setSize(kEditorW, kEditorH);

    updateView();
    startTimerHz(30);
}

PluginEditor::~PluginEditor() {
    stopTimer();
    setLookAndFeel(nullptr);
}

void PluginEditor::updateView() {
    const bool adv = mShowAdvanced;
    mAdvViewport.setVisible(adv);
    mField.setVisible(!adv);
    mOrbit.setVisible(!adv);
    mVibe.setVisible(!adv);
    resized();
}

void PluginEditor::resized() {
    mBrowser.setBounds(getLocalBounds());   // overlay covers everything when shown

    auto full = getLocalBounds();

    // ── Top bar ─────────────────────────────────────────────────────────────────
    auto bar = full.removeFromTop(64);
    auto barRight = bar.removeFromRight(180);
    mPresetBar.setBounds(bar);
    mMasterVol.setBounds(barRight.removeFromLeft(60).reduced(4, 4));
    mAdvToggle.setBounds(barRight.reduced(6, 18).removeFromTop(26));

    // ── Keyboard (bottom, full width) ────────────────────────────────────────────
    auto keys = full.removeFromBottom(104);
    mKeyboard.setBounds(keys.reduced(10, 8));
    // Centre the visible span on middle C; width decides how many octaves show.
    const int whiteKeysShown = juce::jmax(7, (int)(mKeyboard.getWidth() / mKeyboard.getKeyWidth()));
    const int semitonesShown = whiteKeysShown * 12 / 7;
    int lowest = 60 - semitonesShown / 2;
    lowest = (lowest / 12) * 12;  // snap to a C
    mKeyboard.setLowestVisibleKey(juce::jlimit(21, 108 - semitonesShown, lowest));

    // ── Perform bar (arp + chord), above keyboard ───────────────────────────────
    auto perform = full.removeFromBottom(52);
    mPerform.setBounds(perform.reduced(10, 4));

    // ── Synth strip (above perform bar) ─────────────────────────────────────────
    auto strip = full.removeFromBottom(120);
    mStrip.setBounds(strip.reduced(10, 4));

    // ── Creative area / Advanced drawer ─────────────────────────────────────────
    auto area = full.reduced(10, 6);
    if (mShowAdvanced) {
        mAdvViewport.setBounds(area);
        mAdvanced.setSize(area.getWidth() - 14, AdvancedPanel::kContentHeight);
    } else {
        const int pad = 10;
        auto vibe = area.removeFromRight(juce::jmax(330, area.getWidth() * 30 / 100));
        mVibe.setBounds(vibe);
        area.removeFromRight(pad);

        const int orbitW = juce::jmin(380, area.getWidth() * 45 / 100);
        auto orbit = area.removeFromRight(orbitW);
        mOrbit.setBounds(orbit);
        area.removeFromRight(pad);

        mField.setBounds(area);
    }
}

void PluginEditor::paint(juce::Graphics& g) {
    // Diagonal nebula gradient: deep blue (top-left) → indigo → purple/magenta.
    juce::ColourGradient bg(Colours::BgTop, 0.0f, 0.0f,
                            Colours::BgBottom, (float)getWidth(), (float)getHeight(), false);
    bg.addColour(0.45, Colours::BgMid);
    bg.addColour(0.80, Colours::Background);
    g.setGradientFill(bg);
    g.fillAll();

    // Soft nebula blooms (radial colour washes for depth)
    auto bloom = [&](float cx, float cy, float r, juce::Colour c, float a) {
        g.setGradientFill(juce::ColourGradient(c.withAlpha(a), cx, cy,
                                               juce::Colours::transparentBlack, cx, cy - r, true));
        g.fillEllipse(cx - r, cy - r, r * 2.0f, r * 2.0f);
    };
    bloom(getWidth() * 0.14f, getHeight() * 0.26f, 320.0f, Colours::Accent2, 0.12f); // cyan-blue, upper left
    bloom(getWidth() * 0.86f, getHeight() * 0.30f, 300.0f, Colours::Accent,  0.13f); // violet, upper right
    bloom(getWidth() * 0.62f, getHeight() * 0.85f, 360.0f, Colours::Glow,    0.14f); // pink, lower

    // Top-bar separator
    g.setColour(Colours::Separator);
    g.drawHorizontalLine(64, 0.0f, (float)getWidth());

    // Stereo output meters (top-right corner)
    const int mX = getWidth() - 16, mY = 8, mW = 5, mH = 10;
    auto drawMeter = [&](int x, float v) {
        g.setColour(Colours::Separator);
        g.fillRect(x, mY, mW, mH);
        g.setColour(v > 0.92f ? Colours::Red : Colours::Green);
        const int lit = (int)(juce::jlimit(0.0f, 1.0f, v) * mH);
        g.fillRect(x, mY + mH - lit, mW, lit);
    };
    drawMeter(mX - mW,        mMeterL);
    drawMeter(mX - mW * 2 - 2, mMeterR);
}

void PluginEditor::timerCallback() {
    const float atk = 0.7f, dcy = 0.9f;
    const float rawL = mProcessor.meterL.load(std::memory_order_relaxed);
    const float rawR = mProcessor.meterR.load(std::memory_order_relaxed);
    mMeterL = rawL > mMeterL ? mMeterL + atk * (rawL - mMeterL) : mMeterL * dcy;
    mMeterR = rawR > mMeterR ? mMeterR + atk * (rawR - mMeterR) : mMeterR * dcy;
    repaint(getWidth() - 40, 0, 40, 24);
}

} // namespace drift
