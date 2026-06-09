#include "PluginProcessor.h"
#include "../ui/PluginEditor.h"
#include <cmath>

namespace drift {

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
          .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , apvts(*this, nullptr, "DriftState", createParameterLayout())
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int maxBlockSize) {
    juce::ScopedNoDenormals noDenormals;

    mVoiceManager.prepare(sampleRate);
    mDrive.prepare(sampleRate);
    mChorus.prepare(sampleRate);
    mDelay.prepare(sampleRate);
    mReverb.prepare(sampleRate);

    mMasterVol.reset(sampleRate, kParamSmoothSec);
    mMasterVol.setCurrentAndTargetValue(
        *apvts.getRawParameterValue(ParamID::MASTER_VOL));

    (void)maxBlockSize;
}

// ── MIDI handling ─────────────────────────────────────────────────────────────

void PluginProcessor::handleMidi(const juce::MidiMessage& msg) noexcept {
    if (msg.isNoteOn() && msg.getVelocity() > 0) {
        mVoiceManager.noteOn(msg.getNoteNumber(),
                              msg.getFloatVelocity());
    } else if (msg.isNoteOff() || (msg.isNoteOn() && msg.getVelocity() == 0)) {
        mVoiceManager.noteOff(msg.getNoteNumber());
    } else if (msg.isPitchWheel()) {
        const float bend = (msg.getPitchWheelValue() - 8192) / 8192.0f;
        mVoiceManager.setPitchBend(bend * mPitchBendRange);
    } else if (msg.isController()) {
        const int cc  = msg.getControllerNumber();
        const float v = msg.getControllerValue() / 127.0f;
        if (cc == 1)  mVoiceManager.setModWheel(v);
        if (cc == 64) mVoiceManager.setSustainPedal(v > 0.5f);
        if (cc == 120 || cc == 123) mVoiceManager.allNotesOff();
        if (cc == 121) { mVoiceManager.allSoundOff(); mVoiceManager.reset(); }
    } else if (msg.isAftertouch()) {
        mVoiceManager.setAftertouch(msg.getAfterTouchValue() / 127.0f);
    } else if (msg.isAllNotesOff()) {
        mVoiceManager.allNotesOff();
    } else if (msg.isAllSoundOff()) {
        mVoiceManager.allSoundOff();
    }
}

// ── Sync PatchParams from APVTS ───────────────────────────────────────────────

void PluginProcessor::syncPatchFromParams() noexcept {
    using namespace ParamID;
    auto& p = mVoiceManager.getPatch();
    auto* vt = &apvts;

    // Helper lambdas — load from atomic so return type is plain float
    auto f = [&](const char* id) { return vt->getRawParameterValue(id)->load(std::memory_order_relaxed); };
    auto b = [&](const char* id) { return *vt->getRawParameterValue(id) > 0.5f; };
    auto i = [&](const char* id) { return static_cast<int>(*vt->getRawParameterValue(id)); };

    // Osc 1
    p.osc1Shape   = static_cast<OscShape>(i(OSC1_SHAPE));
    p.osc1Octave  = i(OSC1_OCTAVE);
    p.osc1Detune  = static_cast<float>(i(OSC1_SEMITONE));
    p.osc1Fine    = f(OSC1_FINE);
    p.osc1Level   = f(OSC1_LEVEL);
    p.osc1PW      = f(OSC1_PW);
    p.osc1Pan     = f(OSC1_PAN);

    // Osc 2
    p.osc2Shape   = static_cast<OscShape>(i(OSC2_SHAPE));
    p.osc2Octave  = i(OSC2_OCTAVE);
    p.osc2Detune  = static_cast<float>(i(OSC2_SEMITONE));
    p.osc2Fine    = f(OSC2_FINE);
    p.osc2Level   = f(OSC2_LEVEL);
    p.osc2PW      = f(OSC2_PW);
    p.osc2Pan     = f(OSC2_PAN);
    p.osc2Sync    = b(OSC2_SYNC);
    p.oscMix      = f(OSC_MIX);

    // Unison
    p.unisonVoices = i(UNISON_VOICES);
    p.unisonDetune = f(UNISON_DETUNE);
    p.unisonSpread = f(UNISON_SPREAD);

    // Filter 1
    p.filter1Cutoff   = f(FILT1_CUTOFF);
    p.filter1Res      = f(FILT1_RES);
    p.filter1Drive    = f(FILT1_DRIVE);
    p.filter1Type     = static_cast<FilterType>(i(FILT1_TYPE));
    p.filter1EnvAmt   = f(FILT1_ENV_AMT);
    p.filter1KeyTrack = f(FILT1_KEYTRACK);

    // Filter 2
    p.filter2Cutoff   = f(FILT2_CUTOFF);
    p.filter2Res      = f(FILT2_RES);
    p.filter2Drive    = f(FILT2_DRIVE);
    p.filter2Type     = static_cast<FilterType>(i(FILT2_TYPE));
    p.filter2EnvAmt   = f(FILT2_ENV_AMT);
    p.filter2KeyTrack = f(FILT2_KEYTRACK);
    p.filterParallel  = b(FILT_ROUTING);

    // Envelopes
    p.ampEnv   = { 0.0f, f(AMPENV_A), 0.0f, f(AMPENV_D), f(AMPENV_S), f(AMPENV_R), 0.9f, 0.9f };
    p.filterEnv= { 0.0f, f(FILTENV_A),0.0f, f(FILTENV_D),f(FILTENV_S),f(FILTENV_R),0.9f, 0.9f };
    p.modEnv   = { 0.0f, f(MODENV_A), 0.0f, f(MODENV_D), f(MODENV_S), f(MODENV_R), 0.9f, 0.9f };

    // LFOs
    p.lfo1Shape  = static_cast<LFOShape>(i(LFO1_SHAPE));
    p.lfo1Rate   = f(LFO1_RATE);
    p.lfo1Depth  = f(LFO1_DEPTH);
    p.lfo1Retrig = b(LFO1_RETRIG);

    p.lfo2Shape  = static_cast<LFOShape>(i(LFO2_SHAPE));
    p.lfo2Rate   = f(LFO2_RATE);
    p.lfo2Depth  = f(LFO2_DEPTH);
    p.lfo2Retrig = b(LFO2_RETRIG);

    // Mod matrix
    for (int slot = 0; slot < kNumModSlots; ++slot) {
        ModSlot ms;
        ms.source = static_cast<ModSource>(i(MOD_SRC[slot]));
        ms.dest   = static_cast<ModDest>(i(MOD_DST[slot]));
        ms.amount = f(MOD_AMT[slot]);
        p.modSlots[slot] = ms;
    }

    // Master
    p.masterPitch = f(MASTER_TUNE) * (1.0f / 100.0f); // cents → semitones
    p.glideTime   = b(GLIDE_ON) ? f(GLIDE_TIME) : 0.0f;

    mPitchBendRange = f(PITCH_BEND_RANGE);
    mVoiceManager.setMaxVoices(i(POLY_VOICES));
}

// ── processBlock ──────────────────────────────────────────────────────────────

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // Silence input channels we don't use
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear(ch, 0, buffer.getNumSamples());

    const int numSamples = buffer.getNumSamples();
    float* outL = buffer.getWritePointer(0);
    float* outR = buffer.getWritePointer(1 < buffer.getNumChannels() ? 1 : 0);

    // Sync patch once per block (cheap — just atomic reads)
    syncPatchFromParams();

    mMasterVol.setTargetValue(*apvts.getRawParameterValue(ParamID::MASTER_VOL));

    // Effects parameters (per-block, not per-sample — acceptable for FX)
    mDrive.setParameters(*apvts.getRawParameterValue(ParamID::DRIVE_AMOUNT),
                          *apvts.getRawParameterValue(ParamID::DRIVE_MIX));
    mDrive.setBypassed(!(*apvts.getRawParameterValue(ParamID::DRIVE_ON) > 0.5f));

    mChorus.setParameters(*apvts.getRawParameterValue(ParamID::CHORUS_RATE),
                           *apvts.getRawParameterValue(ParamID::CHORUS_DEPTH),
                           *apvts.getRawParameterValue(ParamID::CHORUS_MIX));
    mChorus.setBypassed(!(*apvts.getRawParameterValue(ParamID::CHORUS_ON) > 0.5f));

    mDelay.setParameters(*apvts.getRawParameterValue(ParamID::DELAY_TIME),
                          *apvts.getRawParameterValue(ParamID::DELAY_FDBK),
                          *apvts.getRawParameterValue(ParamID::DELAY_MIX));
    mDelay.setBypassed(!(*apvts.getRawParameterValue(ParamID::DELAY_ON) > 0.5f));

    mReverb.setParameters(*apvts.getRawParameterValue(ParamID::REVERB_SIZE),
                           *apvts.getRawParameterValue(ParamID::REVERB_DAMP),
                           *apvts.getRawParameterValue(ParamID::REVERB_MIX));
    mReverb.setBypassed(!(*apvts.getRawParameterValue(ParamID::REVERB_ON) > 0.5f));

    // Process MIDI events and audio sample-by-sample for event accuracy
    auto midiIt = midiMessages.findNextSamplePosition(0);
    int pos = 0;

    while (pos < numSamples) {
        // Process MIDI messages up to current sample position
        while (midiIt != midiMessages.cend() && (*midiIt).samplePosition <= pos) {
            handleMidi((*midiIt).getMessage());
            ++midiIt;
        }

        float l = 0.0f, r = 0.0f;
        mVoiceManager.processSample(l, r);

        // Master volume (smoothed)
        const float vol = mMasterVol.getNextValue();
        l *= vol;
        r *= vol;

        // Effects chain
        mDrive.processStereo(l, r);
        mChorus.processStereo(l, r);
        mDelay.processStereo(l, r);
        mReverb.processStereo(l, r);

        outL[pos] = l;
        outR[pos] = r;
        ++pos;
    }

    // Drain any remaining MIDI at the end of the block
    while (midiIt != midiMessages.cend()) {
        handleMidi((*midiIt).getMessage());
        ++midiIt;
    }

    // Update peak meters (lock-free)
    float peakL = 0.0f, peakR = 0.0f;
    for (int i = 0; i < numSamples; ++i) {
        peakL = std::max(peakL, std::abs(outL[i]));
        peakR = std::max(peakR, std::abs(outR[i]));
    }
    meterL.store(peakL, std::memory_order_relaxed);
    meterR.store(peakR, std::memory_order_relaxed);
}

// ── State persistence ─────────────────────────────────────────────────────────

void PluginProcessor::getStateInformation(juce::MemoryBlock& data) {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, data);
}

void PluginProcessor::setStateInformation(const void* data, int size) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size));
    if (xml && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

// ── Editor ────────────────────────────────────────────────────────────────────

juce::AudioProcessorEditor* PluginProcessor::createEditor() {
    return new PluginEditor(*this);
}

} // namespace drift

// Required factory function
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new drift::PluginProcessor();
}
