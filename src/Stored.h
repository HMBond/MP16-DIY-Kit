#ifndef Stored_h
#define Stored_h

#include <LittleFS.h>

#include "Constants.h"
#include "Definitions.h"

extern Settings settings;
extern Pad pads[PADS_COUNT];

void loadPreset();
bool loadFromFlash(int slot);
bool saveToFlash(int slot);

#endif