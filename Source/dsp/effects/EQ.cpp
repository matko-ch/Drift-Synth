#include "EQ.h"
#include <cmath>

namespace drift {

namespace {
// RBJ low-shelf coefficients (normalised by a0).
void lowShelf(float fs, float f0, float dB, float& b0, float& b1, float& b2, float& a1, float& a2) {
    const float A = std::pow(10.0f, dB / 40.0f);
    const float w0 = kTwoPi * f0 / fs;
    const float c = std::cos(w0), s = std::sin(w0);
    const float alpha = s * 0.5f * kSqrt2;
    const float tsa = 2.0f * std::sqrt(A) * alpha;
    const float a0 = (A + 1) + (A - 1) * c + tsa;
    b0 = A * ((A + 1) - (A - 1) * c + tsa) / a0;
    b1 = 2 * A * ((A - 1) - (A + 1) * c) / a0;
    b2 = A * ((A + 1) - (A - 1) * c - tsa) / a0;
    a1 = -2 * ((A - 1) + (A + 1) * c) / a0;
    a2 = ((A + 1) + (A - 1) * c - tsa) / a0;
}

void highShelf(float fs, float f0, float dB, float& b0, float& b1, float& b2, float& a1, float& a2) {
    const float A = std::pow(10.0f, dB / 40.0f);
    const float w0 = kTwoPi * f0 / fs;
    const float c = std::cos(w0), s = std::sin(w0);
    const float alpha = s * 0.5f * kSqrt2;
    const float tsa = 2.0f * std::sqrt(A) * alpha;
    const float a0 = (A + 1) - (A - 1) * c + tsa;
    b0 = A * ((A + 1) + (A - 1) * c + tsa) / a0;
    b1 = -2 * A * ((A - 1) + (A + 1) * c) / a0;
    b2 = A * ((A + 1) + (A - 1) * c - tsa) / a0;
    a1 = 2 * ((A - 1) - (A + 1) * c) / a0;
    a2 = ((A + 1) - (A - 1) * c - tsa) / a0;
}

void peaking(float fs, float f0, float dB, float Q, float& b0, float& b1, float& b2, float& a1, float& a2) {
    const float A = std::pow(10.0f, dB / 40.0f);
    const float w0 = kTwoPi * f0 / fs;
    const float c = std::cos(w0), s = std::sin(w0);
    const float alpha = s / (2.0f * Q);
    const float a0 = 1 + alpha / A;
    b0 = (1 + alpha * A) / a0;
    b1 = (-2 * c) / a0;
    b2 = (1 - alpha * A) / a0;
    a1 = (-2 * c) / a0;
    a2 = (1 - alpha / A) / a0;
}
}

void EQ::prepare(double sampleRate) noexcept {
    mSampleRate = static_cast<float>(sampleRate);
    mDirty = true;
    reset();
}

void EQ::reset() noexcept {
    mLowL.reset(); mLowR.reset(); mMidL.reset(); mMidR.reset(); mHighL.reset(); mHighR.reset();
}

void EQ::setParameters(float lowDb, float midDb, float highDb) noexcept {
    if (lowDb != mLowDb || midDb != mMidDb || highDb != mHighDb) {
        mLowDb = lowDb; mMidDb = midDb; mHighDb = highDb;
        mDirty = true;
    }
}

void EQ::recompute() noexcept {
    float b0, b1, b2, a1, a2;
    lowShelf(mSampleRate, 200.0f, mLowDb, b0, b1, b2, a1, a2);
    for (auto* f : { &mLowL, &mLowR }) { f->b0 = b0; f->b1 = b1; f->b2 = b2; f->a1 = a1; f->a2 = a2; }
    peaking(mSampleRate, 1000.0f, mMidDb, 0.8f, b0, b1, b2, a1, a2);
    for (auto* f : { &mMidL, &mMidR }) { f->b0 = b0; f->b1 = b1; f->b2 = b2; f->a1 = a1; f->a2 = a2; }
    highShelf(mSampleRate, 4000.0f, mHighDb, b0, b1, b2, a1, a2);
    for (auto* f : { &mHighL, &mHighR }) { f->b0 = b0; f->b1 = b1; f->b2 = b2; f->a1 = a1; f->a2 = a2; }
    mDirty = false;
}

void EQ::processStereo(float& L, float& R) noexcept {
    if (mBypassed) return;
    if (mDirty) recompute();
    L = mHighL.process(mMidL.process(mLowL.process(L)));
    R = mHighR.process(mMidR.process(mLowR.process(R)));
}

} // namespace drift
