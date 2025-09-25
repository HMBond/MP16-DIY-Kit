#ifndef Midi_h
#define Midi_h

#include "Stored.h"
#include "Hardware.h"

using namespace Stored;
using namespace Hardware;

namespace Midi
{
  extern bool padStates[PADS_COUNT];
  extern bool previousPadStates[PADS_COUNT];
  extern bool midiStates[PADS_COUNT];
  extern bool previousMidiStates[PADS_COUNT];
  extern bool recording; // TODO

  // Integer array to track how many keys are referencing each MIDI note on each channel
  // Replace individual note count arrays with the new struct
  extern ActiveNotes activeNotes;

  void killAllNotes();
  void updateMIDI();
  void stopChord(int i);
  void playChord(int i);
}

#endif