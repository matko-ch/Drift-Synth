#include "Arpeggiator.h"
#include <algorithm>

namespace drift {

void Arpeggiator::prepare(double sampleRate) noexcept {
    mSampleRate = sampleRate;
    reset();
}

void Arpeggiator::reset() noexcept {
    if (mPlayingNote >= 0 && onNoteOff) onNoteOff(mPlayingNote);
    mPlayingNote = -1;
    mGateOpen = false;
    mGateSamples = 0.0;
    mHeldCount = 0;
    mSeqLen = 0;
    mSeqPos = 0;
    mPhase = 0.0;
}

void Arpeggiator::setOctaves(int o) noexcept {
    o = std::clamp(o, 1, 4);
    if (o != mOctaves) { mOctaves = o; rebuildSequence(); }
}

void Arpeggiator::noteOn(int note, float vel) noexcept {
    // Already held? update velocity.
    for (int i = 0; i < mHeldCount; ++i)
        if (mHeld[i] == note) { mHeldVel[i] = vel; return; }
    if (mHeldCount >= kMaxHeld) return;
    mHeld[mHeldCount]    = note;
    mHeldVel[mHeldCount] = vel;
    ++mHeldCount;

    const bool wasEmpty = (mSeqLen == 0);
    rebuildSequence();
    // Start immediately when the first key arrives so it feels responsive.
    if (wasEmpty) { mPhase = mStepSamples; }
}

void Arpeggiator::noteOff(int note) noexcept {
    for (int i = 0; i < mHeldCount; ++i) {
        if (mHeld[i] == note) {
            for (int j = i; j < mHeldCount - 1; ++j) {
                mHeld[j] = mHeld[j + 1];
                mHeldVel[j] = mHeldVel[j + 1];
            }
            --mHeldCount;
            break;
        }
    }
    rebuildSequence();
    if (mHeldCount == 0) {
        if (mPlayingNote >= 0 && onNoteOff) onNoteOff(mPlayingNote);
        mPlayingNote = -1;
        mGateOpen = false;
    }
}

void Arpeggiator::clearHeld() noexcept {
    mHeldCount = 0;
    rebuildSequence();
    if (mPlayingNote >= 0 && onNoteOff) onNoteOff(mPlayingNote);
    mPlayingNote = -1;
    mGateOpen = false;
}

void Arpeggiator::rebuildSequence() noexcept {
    mSeqLen = 0;
    if (mHeldCount == 0) { mSeqPos = 0; return; }

    // Sorted ascending copy for pitch-ordered modes.
    std::array<int, kMaxHeld> sorted{};
    std::array<float, kMaxHeld> sortedVel{};
    for (int i = 0; i < mHeldCount; ++i) { sorted[i] = mHeld[i]; sortedVel[i] = mHeldVel[i]; }
    for (int i = 0; i < mHeldCount; ++i)
        for (int j = i + 1; j < mHeldCount; ++j)
            if (sorted[j] < sorted[i]) { std::swap(sorted[i], sorted[j]); std::swap(sortedVel[i], sortedVel[j]); }

    auto push = [&](int n, float v) {
        if (mSeqLen < kMaxSeq && n >= 0 && n <= 127) { mSeq[mSeqLen] = n; mSeqVel[mSeqLen] = v; ++mSeqLen; }
    };

    // Base order depends on mode; octave layers stack on top.
    const bool asPlayed = (mMode == AsPlayed);
    const int n = mHeldCount;

    auto emitLayer = [&](int oct) {
        if (mMode == Down) {
            for (int i = n - 1; i >= 0; --i) push(sorted[i] + 12 * oct, sortedVel[i]);
        } else if (asPlayed) {
            for (int i = 0; i < n; ++i) push(mHeld[i] + 12 * oct, mHeldVel[i]);
        } else { // Up, UpDown, DownUp, Random all build ascending base
            for (int i = 0; i < n; ++i) push(sorted[i] + 12 * oct, sortedVel[i]);
        }
    };

    for (int oct = 0; oct < mOctaves; ++oct) emitLayer(oct);

    // UpDown / DownUp append the reverse (without repeating the endpoints).
    if (mMode == UpDown || mMode == DownUp) {
        const int forwardLen = mSeqLen;
        for (int i = forwardLen - 2; i >= 1; --i) push(mSeq[i], mSeqVel[i]);
        if (mMode == DownUp) {
            // Rotate so it starts from the top: simplest is to flag start position.
            mSeqPos = std::min(mSeqPos, mSeqLen - 1);
        }
    }

    if (mSeqLen == 0) mSeqPos = 0;
    else mSeqPos %= mSeqLen;
}

void Arpeggiator::step() noexcept {
    // Release the previous note.
    if (mPlayingNote >= 0 && onNoteOff) onNoteOff(mPlayingNote);
    mPlayingNote = -1;

    if (mSeqLen == 0) return;

    int idx;
    if (mMode == Random) {
        mRng ^= mRng << 13; mRng ^= mRng >> 17; mRng ^= mRng << 5;
        idx = static_cast<int>(mRng % static_cast<uint32_t>(mSeqLen));
    } else {
        idx = mSeqPos % mSeqLen;
        mSeqPos = (mSeqPos + 1) % mSeqLen;
    }

    const int note = mSeq[idx];
    if (onNoteOn) onNoteOn(note, mSeqVel[idx]);
    mPlayingNote = note;
    mGateOpen = true;
    mGateSamples = mStepSamples * static_cast<double>(mGate);
}

void Arpeggiator::tick() noexcept {
    if (mHeldCount == 0) return;

    // Gate-off (creates space between notes when gate < 1).
    if (mGateOpen) {
        mGateSamples -= 1.0;
        if (mGateSamples <= 0.0) {
            if (mPlayingNote >= 0 && onNoteOff) onNoteOff(mPlayingNote);
            mPlayingNote = -1;
            mGateOpen = false;
        }
    }

    mPhase += 1.0;
    if (mPhase >= mStepSamples) {
        mPhase -= mStepSamples;
        step();
    }
}

} // namespace drift
