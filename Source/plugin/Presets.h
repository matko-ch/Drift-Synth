#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>

namespace drift {

// One parameter override within a preset (real-world value, not normalised).
struct PresetParam {
    const char* id;
    float       value;
};

// A factory preset: a name, a genre category, and the parameters it sets.
// Anything a preset doesn't list is reset to its default, so presets are
// fully reproducible — and every sound is achievable by hand, no preset needed.
struct Preset {
    const char*              name;
    const char*              category;
    std::vector<PresetParam> params;
};

// The built-in factory bank (built once, on first call).
const std::vector<Preset>& getFactoryPresets();

// Reset every parameter to its default, then apply the preset's overrides.
void applyPreset(juce::AudioProcessor& processor,
                 juce::AudioProcessorValueTreeState& apvts,
                 const Preset& preset);

} // namespace drift
