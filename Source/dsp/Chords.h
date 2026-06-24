#pragma once
#include <array>

namespace drift {

// Chord types selectable in chord mode. Order must match the parameter choices.
enum class ChordType : int {
    Major = 0, Minor, Dim, Aug, Sus2, Sus4,
    Maj7, Min7, Dom7, Dim7, Min7b5,
    Maj9, Min9, Add9, Power5, Octave,
    Count
};

inline constexpr const char* kChordNames[] = {
    "Major", "Minor", "Dim", "Aug", "Sus2", "Sus4",
    "Maj7", "Min7", "Dom7", "Dim7", "Min7b5",
    "Maj9", "Min9", "Add9", "Power5", "Octave"
};

// Semitone offsets from the root (root always included). Up to 5 notes.
// `count` valid entries per chord.
struct ChordShape { std::array<int, 5> iv; int count; };

inline ChordShape chordShape(ChordType t) noexcept {
    switch (t) {
        case ChordType::Major:  return {{0,4,7,0,0}, 3};
        case ChordType::Minor:  return {{0,3,7,0,0}, 3};
        case ChordType::Dim:    return {{0,3,6,0,0}, 3};
        case ChordType::Aug:    return {{0,4,8,0,0}, 3};
        case ChordType::Sus2:   return {{0,2,7,0,0}, 3};
        case ChordType::Sus4:   return {{0,5,7,0,0}, 3};
        case ChordType::Maj7:   return {{0,4,7,11,0}, 4};
        case ChordType::Min7:   return {{0,3,7,10,0}, 4};
        case ChordType::Dom7:   return {{0,4,7,10,0}, 4};
        case ChordType::Dim7:   return {{0,3,6,9,0}, 4};
        case ChordType::Min7b5: return {{0,3,6,10,0}, 4};
        case ChordType::Maj9:   return {{0,4,7,11,14}, 5};
        case ChordType::Min9:   return {{0,3,7,10,14}, 5};
        case ChordType::Add9:   return {{0,4,7,14,0}, 4};
        case ChordType::Power5: return {{0,7,0,0,0}, 2};
        case ChordType::Octave: return {{0,12,0,0,0}, 2};
        default:                return {{0,0,0,0,0}, 1};
    }
}

} // namespace drift
