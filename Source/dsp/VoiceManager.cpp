#include "VoiceManager.h"
#include <algorithm>
#include <limits>

namespace drift {

void VoiceManager::prepare(double sampleRate) noexcept {
    for (auto& v : mVoices)
        v.prepare(sampleRate);
    reset();
}

void VoiceManager::reset() noexcept {
    for (auto& v : mVoices)
        v.reset();
    mSustainPedal = false;
    mHeldByPedal.fill(false);
}

void VoiceManager::setMaxVoices(int n) noexcept {
    mMaxVoices = std::clamp(n, 1, kMaxVoices);
}

void VoiceManager::noteOn(int midiNote, float velocity) noexcept {
    // Retrigger an already-playing voice for this note
    int idx = findVoiceForNote(midiNote);
    if (idx >= 0) {
        mVoices[idx].noteOn(midiNote, velocity, mPatch);
        return;
    }

    idx = findFreeVoice();
    mVoices[idx].noteOn(midiNote, velocity, mPatch);
    mHeldByPedal[midiNote] = false;
}

void VoiceManager::noteOff(int midiNote) noexcept {
    if (mSustainPedal) {
        mHeldByPedal[midiNote] = true;
        return;
    }
    for (int i = 0; i < mMaxVoices; ++i) {
        if (!mVoices[i].isIdle() && mVoices[i].getMidiNote() == midiNote)
            mVoices[i].noteOff();
    }
}

void VoiceManager::setSustainPedal(bool on) noexcept {
    mSustainPedal = on;
    if (!on) {
        // Release any notes held by the pedal
        for (int n = 0; n < 128; ++n) {
            if (mHeldByPedal[n]) {
                mHeldByPedal[n] = false;
                for (int i = 0; i < mMaxVoices; ++i) {
                    if (!mVoices[i].isIdle() && mVoices[i].getMidiNote() == n)
                        mVoices[i].noteOff();
                }
            }
        }
    }
}

void VoiceManager::allNotesOff() noexcept {
    mSustainPedal = false;
    mHeldByPedal.fill(false);
    for (int i = 0; i < mMaxVoices; ++i)
        mVoices[i].noteOff();
}

void VoiceManager::allSoundOff() noexcept {
    mSustainPedal = false;
    mHeldByPedal.fill(false);
    for (int i = 0; i < mMaxVoices; ++i)
        mVoices[i].panic();
}

void VoiceManager::setPitchBend(float semitones) noexcept {
    for (int i = 0; i < mMaxVoices; ++i)
        mVoices[i].setPitchBend(semitones);
}

void VoiceManager::setModWheel(float value) noexcept {
    for (int i = 0; i < mMaxVoices; ++i)
        mVoices[i].setModWheel(value);
}

void VoiceManager::setAftertouch(float value) noexcept {
    for (int i = 0; i < mMaxVoices; ++i)
        mVoices[i].setAftertouch(value);
}

int VoiceManager::findVoiceForNote(int midiNote) noexcept {
    for (int i = 0; i < mMaxVoices; ++i) {
        if (!mVoices[i].isIdle() && mVoices[i].getMidiNote() == midiNote)
            return i;
    }
    return -1;
}

int VoiceManager::findFreeVoice() noexcept {
    // 1. Find a truly idle voice
    for (int i = 0; i < mMaxVoices; ++i) {
        if (mVoices[i].isIdle())
            return i;
    }

    // 2. Steal the oldest releasing voice
    float maxAge = -1.0f;
    int bestIdx = 0;
    for (int i = 0; i < mMaxVoices; ++i) {
        if (mVoices[i].isReleasing() && mVoices[i].getAge() > maxAge) {
            maxAge  = mVoices[i].getAge();
            bestIdx = i;
        }
    }
    if (maxAge >= 0.0f)
        return bestIdx;

    // 3. Steal the quietest active voice
    float minAmp = std::numeric_limits<float>::max();
    for (int i = 0; i < mMaxVoices; ++i) {
        if (mVoices[i].getAmplitude() < minAmp) {
            minAmp  = mVoices[i].getAmplitude();
            bestIdx = i;
        }
    }
    return bestIdx;
}

void VoiceManager::processSample(float& outL, float& outR) noexcept {
    outL = 0.0f;
    outR = 0.0f;

    for (int i = 0; i < mMaxVoices; ++i) {
        if (mVoices[i].isIdle())
            continue;
        auto [l, r] = mVoices[i].process(mPatch);
        outL += l;
        outR += r;
    }
}

} // namespace drift
