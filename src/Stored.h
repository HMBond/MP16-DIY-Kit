#ifndef Stored_h
#define Stored_h

#include <LittleFS.h>
#include "Constants.h"
#include "Definitions.h"

namespace Stored
{
  Settings settings;
  Pad pads[PADS_COUNT];

  // Load in all the data for the chords from flash memory, or initialize from the Stored
  void load();
  bool loadFromFlash(int slot);
  void initFromPreset();
}

#endif