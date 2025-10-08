#ifndef MusicTheory_h
#define MusicTheory_h

const int CHORD_NOTE_COUNT = 8;  // How many notes in a chord
const int ROOT_MIN = 12;         // The minimal root note value
const int ROOT_MAX = 107;        // The maximal root note value
const int SCALE_COUNT = 10;      // Amount of scales

// Array of scale names, each shortened to 8 characters
extern const char *scaleNames[SCALE_COUNT];

// Array of scales (in semitones)
extern int scales[SCALE_COUNT][7];

extern const char *midiNoteNames[128];

extern int chordDegrees[7];

#endif