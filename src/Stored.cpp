#include "Stored.h"

#include <LittleFS.h>

#include "Constants.h"
#include "Definitions.h"

Settings settings;
Pad pads[PADS_COUNT];

// initialize the chords in all pads from the preset.h file
void loadInitialPreset() {
  // Initialize the global `chords` array from the `presets` array
  for (int i = 0; i < PADS_COUNT; i++) {
    // Copy the preset chord data to the global chords array
    for (int j = 0; j < 8; j++) {
      pads[i].chord.intervals[j] = initialPreset[i].intervals[j];
      pads[i].chord.octaveModifiers[j] = initialPreset[i].octaveModifiers[j];
      pads[i].chord.semitoneModifiers[j] =
          initialPreset[i].semitoneModifiers[j];
      pads[i].chord.isActive[j] = initialPreset[i].isActive[j];
      pads[i].chord.velocityModifiers[j] =
          initialPreset[i].velocityModifiers[j];
      pads[i].chord.timing[j] = initialPreset[i].timing[j];
      pads[i].chord.channel[j] = initialPreset[i].channel[j];
    }
    pads[i].chord.degree = initialPreset[i].degree;  // Copy the degree value
    pads[i].triggerNote = settings.rootNote + i;
  }
}

void loadPreset() {
  if (!loadFromFlash(0)) {
    // No saved data available, initialize from preset
    loadInitialPreset();
  }
}

// Load a memory slot from flash memory,
// returns true if successful, false on failure
bool loadFromFlash(int slot) {
  String filePath =
      "/slot" + String(slot) + ".txt";       // Use String for concatenation
  File file = LittleFS.open(filePath, "r");  // Pass the constructed file path
  if (!file) {
    return false;
  } else {
    file.read((uint8_t *)&settings, sizeof(Settings));
    file.read((uint8_t *)&pads, sizeof(pads));
    file.close();
    return true;
  }
}

// Save current settings to a memory slot
bool saveToFlash(int slot) {
  String filePath =
      "/slot" + String(slot) + ".txt";       // Use String for concatenation
  File file = LittleFS.open(filePath, "w");  // Pass the constructed file path
  if (!file) {
    Serial.println("Failed to open file for writing");
    return false;
  }
  file.write((uint8_t *)&settings, sizeof(settings));
  file.write((uint8_t *)&pads, sizeof(pads));
  file.close();
  return true;
}
