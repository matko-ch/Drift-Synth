#include "ModMatrix.h"

namespace drift {

void ModMatrix::setSlot(int idx, ModSlot slot) noexcept {
    if (idx >= 0 && idx < kNumModSlots)
        mSlots[idx] = slot;
}

ModValues ModMatrix::process(
    const std::array<float, static_cast<int>(ModSource::Count)>& src) const noexcept
{
    ModValues out{};

    for (const auto& slot : mSlots) {
        if (slot.source == ModSource::None || slot.dest == ModDest::None)
            continue;

        const float sig = src[static_cast<int>(slot.source)] * slot.amount;

        switch (slot.dest) {
            case ModDest::MasterPitch:       out.masterPitch    += sig; break;
            case ModDest::Osc1Pitch:         out.osc1Pitch      += sig; break;
            case ModDest::Osc2Pitch:         out.osc2Pitch      += sig; break;
            case ModDest::Osc1PulseWidth:    out.osc1PW         += sig; break;
            case ModDest::Osc2PulseWidth:    out.osc2PW         += sig; break;
            case ModDest::OscMix:            out.oscMix         += sig; break;
            case ModDest::Filter1Cutoff:     out.filter1Cutoff  += sig; break;
            case ModDest::Filter1Resonance:  out.filter1Res     += sig; break;
            case ModDest::Filter2Cutoff:     out.filter2Cutoff  += sig; break;
            case ModDest::Filter2Resonance:  out.filter2Res     += sig; break;
            case ModDest::AmpVolume:         out.ampVolume      += sig; break;
            case ModDest::Pan:               out.pan            += sig; break;
            case ModDest::LFO1Rate:          out.lfo1Rate       += sig; break;
            case ModDest::LFO2Rate:          out.lfo2Rate       += sig; break;
            default: break;
        }
    }

    return out;
}

} // namespace drift
