#include <LittleFS.h>
#include "Constants.h"
#include "Definitions.h"
#include "Stored.h"

namespace Stored
{
  Settings settings;
  Pad pads[PADS_COUNT];
  
  // Load a memory slot from flash memory, returns true is successful, otherwise returns false
  bool loadFromFlash(int slot)
  {
    String filePath = "/slot" + String(slot) + ".txt"; // Use String for concatenation
    File file = LittleFS.open(filePath, "r");          // Pass the constructed file path
    if (!file)
    {
      return false;
    }
    else
    {
      file.read((uint8_t *)&settings, sizeof(Settings));
      file.read((uint8_t *)&pads, sizeof(pads));
      file.close();
      return true;
    }
  }

  // initialize the chords in all pads from the preset.h file
  void initFromPreset()
  {
        // Initialize the global `chords` array from the `presets` array
        for (int i = 0; i < 16; i++)
    {
      // Copy the preset chord data to the global chords array
      for (int j = 0; j < 8; j++)
      {
        pads[i].chord.intervals[j] = initialPreset[i].intervals[j];
        pads[i].chord.octaveModifiers[j] = initialPreset[i].octaveModifiers[j];
        pads[i].chord.semitoneModifiers[j] = initialPreset[i].semitoneModifiers[j];
        pads[i].chord.isActive[j] = initialPreset[i].isActive[j];
        pads[i].chord.velocityModifiers[j] = initialPreset[i].velocityModifiers[j];
        pads[i].chord.timing[j] = initialPreset[i].timing[j];
        pads[i].chord.channel[j] = initialPreset[i].channel[j];
      }
      pads[i].chord.degree = initialPreset[i].degree; // Copy the degree value
      pads[i].triggerNote = settings.rootNote + i;
    }
  }

  // Load in all the data for the chords from flash memory, or initialize from the preset
  void load()
  {
    if (!loadFromFlash(0))
    {
      // No saved data available, initialize chords from preset
      initFromPreset();
    }
  }
}