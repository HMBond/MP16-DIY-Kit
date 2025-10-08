#ifndef Pads_h
#define Pads_h

#include "Constants.h"
#include "Display.h"
#include "Stored.h"
#include "Midi.h"

using namespace Stored;
using namespace Midi;

namespace Pads
{
  extern int selectedPad;  // Last pressed pad
  extern int selectedSlot; // Selected save/load preset slot
  extern int copyIndex;    // Which pad to copy
  extern bool shiftState;
  extern bool previousShiftState;
  extern bool encoderState;
  extern bool previousEncoderState;
  extern bool keyStates[PADS_COUNT];
  extern bool previousKeyStates[PADS_COUNT];
  extern bool shiftPressedAtKeyDown[PADS_COUNT];

  void checkKeys();
  void handleKeyChanges(int &screenIndex, int &menuIndex, int copyMenuScreenIndex);
}

#endif
