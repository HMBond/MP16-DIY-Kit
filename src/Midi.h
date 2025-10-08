#ifndef Midi_h
#define Midi_h

#include "Hardware.h"
#include "Stored.h"

extern bool padStates[PADS_COUNT];
extern bool previousPadStates[PADS_COUNT];
extern bool midiStates[PADS_COUNT];
extern bool previousMidiStates[PADS_COUNT];

// Integer array to track how many keys are referencing each MIDI note on each
// channel Replace individual note count arrays with the new struct
extern ActiveNotes activeNotes;

void killAllNotes();
void checkMidiIn();
void stopChord(int pad);
void playChord(int pad);

#endif