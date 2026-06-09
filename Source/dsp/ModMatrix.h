#pragma once
#include "Constants.h"
#include <array>

namespace drift {

enum class ModSource : int {
    None = 0,
    AmpEnv,
    FilterEnv,
    ModEnv,
    LFO1,
    LFO2,
    Velocity,
    ModWheel,
    KeyTrack,   // normalised MIDI note: 0=C0, 1=G10
    Aftertouch,
    Count
};

enum class ModDest : int {
    None = 0,
    MasterPitch,      // semitones
    Osc1Pitch,        // semitones
    Osc2Pitch,        // semitones
    Osc1PulseWidth,
    Osc2PulseWidth,
    OscMix,
    Filter1Cutoff,    // octaves (additive on log-scale)
    Filter1Resonance,
    Filter2Cutoff,
    Filter2Resonance,
    AmpVolume,
    Pan,
    LFO1Rate,
    LFO2Rate,
    Count
};

struct ModSlot {
    ModSource source = ModSource::None;
    ModDest   dest   = ModDest::None;
    float     amount = 0.0f;  // bipolar [-1, 1]
};

// All modulation values are accumulated here each voice-tick.
// Voices read from this struct; it is rebuilt each voice per sample.
struct ModValues {
    float masterPitch     = 0.0f;
    float osc1Pitch       = 0.0f;
    float osc2Pitch       = 0.0f;
    float osc1PW          = 0.0f;
    float osc2PW          = 0.0f;
    float oscMix          = 0.0f;
    float filter1Cutoff   = 0.0f; // additive octaves
    float filter1Res      = 0.0f;
    float filter2Cutoff   = 0.0f;
    float filter2Res      = 0.0f;
    float ampVolume       = 0.0f;
    float pan             = 0.0f;
    float lfo1Rate        = 0.0f;
    float lfo2Rate        = 0.0f;
};

// Stateless — routes source values to destination accumulators.
class ModMatrix {
public:
    void setSlot(int idx, ModSlot slot) noexcept;
    const ModSlot& getSlot(int idx) const noexcept { return mSlots[idx]; }

    // sourceValues: indexed by ModSource (0=None → always 0)
    // Returns accumulated ModValues for one voice-sample.
    [[nodiscard]] DRIFT_HOT
    ModValues process(const std::array<float, static_cast<int>(ModSource::Count)>& src) const noexcept;

private:
    std::array<ModSlot, kNumModSlots> mSlots{};
};

} // namespace drift
