#ifndef Stored_h
#define Stored_h

#include <LittleFS.h>
#include "Constants.h"
#include "Definitions.h"

namespace Stored
{
  extern Settings settings;
  extern Pad pads[PADS_COUNT];

  // Load in all the data for the chords from flash memory, or initialize from the preset
  void load();
  bool loadFromFlash(int slot);
  bool saveToFlash(int slot);
}

#endif