#ifndef MusicTheory_h
#define MusicTheory_h

namespace MusicTheory
{
    const int CHORD_NOTE_COUNT = 8; // How many notes can be selected within a chord
    const int ROOT_MIN = 12;        // The minimal value for selecting the root note
    const int ROOT_MAX = 107;       // The maximal value for selecting the root note
    const int SCALE_COUNT = 10;     // Amount of scales

    // Array of scale names, each shortened to 8 characters
    extern const char *scaleNames[SCALE_COUNT];

    // Array of scales (in semitones)
    extern int scales[SCALE_COUNT][7];

    extern const char *midiNoteNames[128];

    extern int chordDegrees[7];
}

#endif