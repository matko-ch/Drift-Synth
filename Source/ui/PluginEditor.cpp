#include "PluginEditor.h"

namespace drift {

static constexpr int kEditorW = 960;
static constexpr int kEditorH = 680;

PluginEditor::PluginEditor(PluginProcessor& proc)
    : AudioProcessorEditor(proc)
    , mProcessor(proc)
    , mOsc1Panel   (proc.apvts, 1)
    , mOsc2Panel   (proc.apvts, 2)
    , mFilter1Panel(proc.apvts, 1)
    , mFilter2Panel(proc.apvts, 2)
    , mAmpEnvPanel   (proc.apvts, 0)
    , mFilterEnvPanel(proc.apvts, 1)
    , mModEnvPanel   (proc.apvts, 2)
    , mLFO1Panel(proc.apvts, 1)
    , mLFO2Panel(proc.apvts, 2)
    , mModMatrix(proc.apvts)
    , mFXPanel  (proc.apvts)
{
    setLookAndFeel(&DriftLookAndFeel::instance());

    // Title
    mTitleLabel.setText("DRIFT", juce::dontSendNotification);
    mTitleLabel.setFont(juce::Font(juce::FontOptions{}.withHeight(22.0f).withStyle("Bold")));
    mTitleLabel.setColour(juce::Label::textColourId, Colours::Accent);
    mTitleLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(mTitleLabel);

    // Oscillator panels
    addAndMakeVisible(mOsc1Panel);
    addAndMakeVisible(mOsc2Panel);

    // Osc mix knob
    mOscMixKnob.attach(proc.apvts, ParamID::OSC_MIX);
    addAndMakeVisible(mOscMixKnob);

    // Unison
    mUnisonGroup.setText("UNISON");
    mUnisonVoicesKnob.attach(proc.apvts, ParamID::UNISON_VOICES);
    mUnisonDetuneKnob.attach(proc.apvts, ParamID::UNISON_DETUNE);
    mUnisonSpreadKnob.attach(proc.apvts, ParamID::UNISON_SPREAD);
    mUnisonGroup.addAndMakeVisible(mUnisonVoicesKnob);
    mUnisonGroup.addAndMakeVisible(mUnisonDetuneKnob);
    mUnisonGroup.addAndMakeVisible(mUnisonSpreadKnob);
    addAndMakeVisible(mUnisonGroup);

    // Filters
    addAndMakeVisible(mFilter1Panel);
    addAndMakeVisible(mFilter2Panel);

    // Filter routing
    mFilterRoutingBtn.setButtonText("Parallel");
    mRoutingAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        proc.apvts, ParamID::FILT_ROUTING, mFilterRoutingBtn);
    addAndMakeVisible(mFilterRoutingBtn);
    mFilterRoutingLabel.setText("Routing", juce::dontSendNotification);
    mFilterRoutingLabel.setColour(juce::Label::textColourId, Colours::TextDim);
    mFilterRoutingLabel.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f)));
    mFilterRoutingLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterRoutingLabel);

    // Envelopes
    addAndMakeVisible(mAmpEnvPanel);
    addAndMakeVisible(mFilterEnvPanel);
    addAndMakeVisible(mModEnvPanel);

    // LFOs
    addAndMakeVisible(mLFO1Panel);
    addAndMakeVisible(mLFO2Panel);

    // Mod Matrix
    addAndMakeVisible(mModMatrix);

    // Master
    mMasterGroup.setText("MASTER");
    mMasterVolKnob.attach(proc.apvts, ParamID::MASTER_VOL);
    mMasterTuneKnob.attach(proc.apvts, ParamID::MASTER_TUNE);
    mGlideKnob.attach(proc.apvts, ParamID::GLIDE_TIME);
    mGlideOnAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        proc.apvts, ParamID::GLIDE_ON, mGlideOnBtn);
    mPolyVoicesCB.addItemList({"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"}, 1);
    mPolyLabel.setText("Poly", juce::dontSendNotification);
    mPolyLabel.setColour(juce::Label::textColourId, Colours::TextDim);
    mPolyLabel.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f)));
    mPolyLabel.setJustificationType(juce::Justification::centred);
    mPolyAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        proc.apvts, ParamID::POLY_VOICES, mPolyVoicesCB);
    mMasterGroup.addAndMakeVisible(mMasterVolKnob);
    mMasterGroup.addAndMakeVisible(mMasterTuneKnob);
    mMasterGroup.addAndMakeVisible(mGlideKnob);
    mMasterGroup.addAndMakeVisible(mGlideOnBtn);
    mMasterGroup.addAndMakeVisible(mPolyVoicesCB);
    mMasterGroup.addAndMakeVisible(mPolyLabel);
    addAndMakeVisible(mMasterGroup);

    // FX
    addAndMakeVisible(mFXPanel);

    // Window size
    setResizable(true, true);
    setResizeLimits(768, 520, 1280, 900);
    setSize(kEditorW, kEditorH);

    startTimerHz(30);
}

PluginEditor::~PluginEditor() {
    stopTimer();
    setLookAndFeel(nullptr);
}

// ── Layout ────────────────────────────────────────────────────────────────────

void PluginEditor::resized() {
    auto full = getLocalBounds();

    // ── Header ────────────────────────────────────────────────────────────────
    const int headerH = 36;
    const auto header = full.removeFromTop(headerH);
    mTitleLabel.setBounds(header.reduced(10, 4));

    const int padH = 6, padV = 4;
    auto body = full.reduced(padH, padV);

    // ── Row 1: OSC + Unison + Mix ─────────────────────────────────────────────
    const int row1H = 130;
    auto row1 = body.removeFromTop(row1H);
    body.removeFromTop(padV);

    const int oscW  = 260;
    const int mixW  = 50;
    const int uniW  = 170;
    mOsc1Panel.setBounds(row1.removeFromLeft(oscW));
    row1.removeFromLeft(padH);
    mOsc2Panel.setBounds(row1.removeFromLeft(oscW));
    row1.removeFromLeft(padH);

    // Osc mix knob
    const auto mixArea = row1.removeFromLeft(mixW);
    mOscMixKnob.setBounds(mixArea.reduced(0, 30).withTrimmedBottom(10));
    row1.removeFromLeft(padH);

    mUnisonGroup.setBounds(row1.removeFromLeft(uniW));
    row1.removeFromLeft(padH);

    // ── Row 2: Filters ────────────────────────────────────────────────────────
    const int row2H = 140;
    auto row2 = body.removeFromTop(row2H);
    body.removeFromTop(padV);

    const int filtW = 280;
    mFilter1Panel.setBounds(row2.removeFromLeft(filtW));
    row2.removeFromLeft(padH);
    mFilter2Panel.setBounds(row2.removeFromLeft(filtW));
    row2.removeFromLeft(padH);

    // Routing button
    auto routArea = row2.removeFromLeft(80);
    mFilterRoutingLabel.setBounds(routArea.removeFromTop(16));
    mFilterRoutingBtn.setBounds(routArea.reduced(0, 20));

    // Master group fills the rest
    mMasterGroup.setBounds(row2);
    // Layout within master group:
    {
        auto mg = mMasterGroup.getLocalBounds().reduced(6, 18);
        const int kw = 44;
        mMasterVolKnob.setBounds(mg.getX() + 0*(kw+4), mg.getY(), kw, 58);
        mMasterTuneKnob.setBounds(mg.getX() + 1*(kw+4), mg.getY(), kw, 58);
        mGlideKnob.setBounds(mg.getX() + 2*(kw+4), mg.getY(), kw, 58);
        mGlideOnBtn.setBounds(mg.getX() + 3*(kw+4) + 2, mg.getY() + 18, 44, 20);
        mPolyLabel.setBounds(mg.getX() + 4*(kw+4), mg.getY(), 44, 14);
        mPolyVoicesCB.setBounds(mg.getX() + 4*(kw+4), mg.getY() + 14, 44, 22);
    }

    // ── Row 3: Envelopes + LFOs ───────────────────────────────────────────────
    const int row3H = 120;
    auto row3 = body.removeFromTop(row3H);
    body.removeFromTop(padV);

    const int envW = (row3.getWidth() - 2*(row3.getWidth()/5) - 4*padH) / 3;
    const int lfoW = row3.getWidth() / 5;
    mAmpEnvPanel   .setBounds(row3.removeFromLeft(envW));
    row3.removeFromLeft(padH);
    mFilterEnvPanel.setBounds(row3.removeFromLeft(envW));
    row3.removeFromLeft(padH);
    mModEnvPanel   .setBounds(row3.removeFromLeft(envW));
    row3.removeFromLeft(padH);
    mLFO1Panel.setBounds(row3.removeFromLeft(lfoW));
    row3.removeFromLeft(padH);
    mLFO2Panel.setBounds(row3);

    // ── Row 4: Mod Matrix + FX ────────────────────────────────────────────────
    const int matrixW = 430;
    auto row4 = body;
    mModMatrix.setBounds(row4.removeFromLeft(matrixW));
    row4.removeFromLeft(padH);
    mFXPanel.setBounds(row4);
}

// ── Paint ─────────────────────────────────────────────────────────────────────

void PluginEditor::paint(juce::Graphics& g) {
    // Background
    g.fillAll(Colours::Background);

    // Subtle grid lines
    g.setColour(Colours::Separator.withAlpha(0.4f));
    for (int y = 36; y < getHeight(); y += 80)
        g.drawHorizontalLine(y, 0.0f, static_cast<float>(getWidth()));

    // Header gradient
    juce::ColourGradient headerGrad(
        Colours::Panel, 0.0f, 0.0f,
        Colours::Background, 0.0f, 36.0f, false);
    g.setGradientFill(headerGrad);
    g.fillRect(0, 0, getWidth(), 36);

    // Version label
    g.setFont(juce::Font(juce::FontOptions{}.withHeight(10.0f)));
    g.setColour(Colours::TextDim);
    g.drawText("v1.0  virtual analog", getWidth() - 200, 12, 190, 14,
               juce::Justification::centredRight);

    // Output meters (right of header)
    const float mL = mMeterLDisplay;
    const float mR = mMeterRDisplay;
    const int mX = getWidth() - 200 - 50;
    const int mY = 10;
    const int mW = 6, mH = 16;

    g.setColour(Colours::Separator);
    g.fillRect(mX, mY, mW, mH);
    g.fillRect(mX + mW + 2, mY, mW, mH);

    g.setColour(mL > 0.9f ? Colours::Red : Colours::Green);
    g.fillRect(mX, mY + mH - static_cast<int>(mL * mH), mW, static_cast<int>(mL * mH));
    g.setColour(mR > 0.9f ? Colours::Red : Colours::Green);
    g.fillRect(mX + mW + 2, mY + mH - static_cast<int>(mR * mH), mW, static_cast<int>(mR * mH));
}

// ── Timer ─────────────────────────────────────────────────────────────────────

void PluginEditor::timerCallback() {
    // Meter ballistics: fast attack, slow decay
    const float atk = 0.8f, dcy = 0.93f;
    const float rawL = mProcessor.meterL.load(std::memory_order_relaxed);
    const float rawR = mProcessor.meterR.load(std::memory_order_relaxed);
    mMeterLDisplay = rawL > mMeterLDisplay ? mMeterLDisplay + atk * (rawL - mMeterLDisplay)
                                            : mMeterLDisplay * dcy;
    mMeterRDisplay = rawR > mMeterRDisplay ? mMeterRDisplay + atk * (rawR - mMeterRDisplay)
                                            : mMeterRDisplay * dcy;
    repaint(getWidth() - 280, 0, 280, 36);  // repaint only header meter area
}

} // namespace drift
