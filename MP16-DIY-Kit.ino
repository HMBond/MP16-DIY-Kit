#include "./src/Constants.h"
#include "./src/Definitions.h"
#include "./src/Hardware.h"
#include "./src/Stored.h"
#include "./src/Display.h"
#include "./src/MusicTheory.h"
#include "./src/Pads.h"

using namespace MusicTheory;
using namespace Hardware;
using namespace Stored;
using namespace Display;
using namespace Pads;

// Menu and selection indices
int screenIndex = -1;      // Which screen is shown?
int menuIndex = 0;         // Which menu item is selected?
bool editMenuItem = false; // Edit the selected menu item
int noteIndex = 0;         // Which of the 8 notes in a chord to select

// How much to dim the LED colors
float dimFactor = 0.3;

void setup()
{
  initHardware();

  pinMode(RX_PIN, INPUT_PULLUP);
  Serial1.begin(31250);

  if (!LittleFS.begin())
  {
    showMessage("LittleFS failed");
  }

  Stored::load();
}

void loop()
{
  checkKeys();
  updateMenu();
  updateMIDI();
  actOnPadStates(screenIndex, menuIndex);
  updatePixels();
  updateDisplay(screenIndex, selectedPad, menuIndex, selectedSlot, noteIndex, editMenuItem);
}

// ********************************** Loop Functions ************************************

// Main function to update the menu
void updateMenu()
{
  switch (screenIndex)
  {
  case -2: // Load preset
    menuLoad();
    break;
  case -1: // Main screen
    menuMain();
    break;
  case 0: // Edit Root Note
    menuRoot();
    break;
  case 1: // Edit Scale
    menuScale();
    break;
  case 2: // Edit Degree
    menuDegree();
    break;
  case 3: // Edit Active Notes
    menuNotes();
    break;
  case 4: // Edit Velocity Randomness
    menuVariation();
    break;
  case 5: // Edit Velocity
    menuVelocity();
    break;
  case 6: // Edit Interval Mods
    menuNoteOffset();
    break;
  case 7: // Edit Octave
    menuOctaves();
    break;
  case 11: // Note Channel
    menuNoteChannel();
    break;
  case 14: // Edit MIDI Settings
    menuMidi();
    break;
  case 15: // Saving
    menuSave();
    break;
  }
}

void menuLoad()
{
  selectedSlot = readEncoder(selectedSlot, SLOT_COUNT);
  if (encoderState && !previousEncoderState)
  {
    killAllNotes();
    screenIndex = -1;
    if (loadFromFlash(selectedSlot))
    {
      showMessage("Loaded Slot " + String(selectedSlot + 1));
    }
    else
    {
      showMessage("Loading Failed");
    }
  }
}

void menuMain()
{
  settings.velocityScaling = readEncoderFast(settings.velocityScaling, 0.01, 0.015625, 2.0);
  if (encoderState && !previousEncoderState)
  {
    screenIndex = -2;
  }
}

void menuRoot()
{
  killAllNotes();
  settings.rootNote = readEncoderConstrained(settings.rootNote, 1, ROOT_MIN, ROOT_MAX);
}

void menuScale()
{
  if (encoderValue != 0)
  {
    killAllNotes();
    settings.scaleIndex = readEncoder(settings.scaleIndex, SCALE_COUNT);
    setAllChordIntervals();
  }
}

void menuDegree()
{
  if (encoderValue != 0)
  {
    killAllNotes();
    pads[selectedPad].chord.degree = readEncoderConstrained(
        pads[selectedPad].chord.degree, 1, 0, 6);
    setChordIntervals(selectedPad);
  }
}

void menuNotes()
{
  noteIndex = readEncoder(noteIndex, CHORD_NOTE_COUNT);
  if (encoderState && !previousEncoderState)
  {
    killAllNotes();
    pads[selectedPad].chord.isActive[noteIndex] = !pads[selectedPad].chord.isActive[noteIndex];
  }
}

void menuVariation()
{
  pads[selectedPad].velocityRandom = readEncoderFast(
      pads[selectedPad].velocityRandom, 1, 0, VELOCITY_RANDOM_MAX);
}

void menuVelocity()
{
  if (menuIndex == 0)
  {
    noteIndex = readEncoder(noteIndex, CHORD_NOTE_COUNT);
    if (encoderState && !previousEncoderState)
    {
      menuIndex = noteIndex + 1;
    }
  }
  else if (menuIndex - 1 < CHORD_NOTE_COUNT)
  {
    if (encoderValue != 0)
    {
      killAllNotes();
      pads[selectedPad].chord.velocityModifiers[menuIndex - 1] = readEncoderFast(
          pads[selectedPad].chord.velocityModifiers[menuIndex - 1], 1, -128, 128);
    }
    if (encoderState && !previousEncoderState)
    {
      menuIndex = 0;
    }
  }
}

void menuNoteOffset()
{
  if (menuIndex == 0)
  {
    noteIndex = readEncoder(noteIndex, CHORD_NOTE_COUNT);
    if (encoderState && !previousEncoderState)
    {
      menuIndex = noteIndex + 1;
    }
  }
  else if (menuIndex - 1 < CHORD_NOTE_COUNT)
  {
    if (encoderValue != 0)
    {
      killAllNotes();
      int &semitoneModifier = pads[selectedPad].chord.semitoneModifiers[menuIndex - 1];
      semitoneModifier = readEncoderConstrained(semitoneModifier, 1, -7, 7);
    }
    if (encoderState && !previousEncoderState)
    {
      menuIndex = 0;
    }
  }
}

void menuOctaves()
{
  if (menuIndex == 0)
  {
    noteIndex = readEncoder(noteIndex, CHORD_NOTE_COUNT);
    if (encoderState && !previousEncoderState)
    {
      menuIndex = noteIndex + 1;
    }
  }
  else if (menuIndex - 1 < CHORD_NOTE_COUNT)
  {
    if (encoderValue != 0)
    {
      killAllNotes();
      pads[selectedPad].chord.octaveModifiers[menuIndex - 1] = readEncoderConstrained(
          pads[selectedPad].chord.octaveModifiers[menuIndex - 1], 1, -3, 3);
    }
    if (encoderState && !previousEncoderState)
    {
      menuIndex = 0;
    }
  }
}

void menuNoteChannel()
{
  if (menuIndex == 0)
  {
    noteIndex = readEncoder(noteIndex, CHORD_NOTE_COUNT);
    if (encoderState && !previousEncoderState)
    {
      menuIndex = noteIndex + 1;
    }
  }
  else if (menuIndex - 1 < CHORD_NOTE_COUNT)
  {
    if (encoderValue != 0)
    {
      killAllNotes();
      pads[selectedPad].chord.channel[menuIndex - 1] = readEncoderConstrained(
          pads[selectedPad].chord.channel[menuIndex - 1], 1, 0, 3);
    }
    if (encoderState && !previousEncoderState)
    {
      menuIndex = 0;
    }
  }
}

// Helper function to handle MIDI channel updates
int updateMidiChannel(int channel)
{
  if (encoderValue != 0)
  {
    killAllNotes();
    return readEncoder(channel, MIDI_CHANNELS);
  }
  return channel;
}

void menuMidi()
{
  if (encoderState && !previousEncoderState)
  {
    editMenuItem = !editMenuItem;
  }
  if (!editMenuItem)
  {
    menuIndex = readEncoder(menuIndex, 6);
  }
  if (editMenuItem)
  {
    switch (menuIndex)
    {
    case 0:
      settings.midiOutputAChannel = updateMidiChannel(settings.midiOutputAChannel);
      break;

    case 1:
      settings.midiOutputBChannel = updateMidiChannel(settings.midiOutputBChannel);
      break;

    case 2:
      settings.midiOutputCChannel = updateMidiChannel(settings.midiOutputCChannel);
      break;

    case 3:
      settings.midiOutputDChannel = updateMidiChannel(settings.midiOutputDChannel);
      break;

    case 4:
      settings.midiRecChannel = updateMidiChannel(settings.midiRecChannel);
      break;

    case 5:
      settings.midiTrigChannel = updateMidiChannel(settings.midiTrigChannel);
      break;
    }
  }
}

void menuSave()
{
  selectedSlot = readEncoder(selectedSlot, SLOT_COUNT);
  if (encoderState && !previousEncoderState)
  {
    bool success = saveToFlash(selectedSlot);
    if (success)
    {
      screenIndex = -1;
      menuIndex = 0;
      String message = "Saved to Slot " + String(selectedSlot + 1);
      showMessage(message);
    }
    else
    {
      showMessage("Saving Failed");
    }
  }
}

// Menu helper function for updating all chord intervals after a scale change
void setAllChordIntervals()
{
  for (int i = 0; i < PADS_COUNT; i++)
  {
    setChordIntervals(i);
  }
}

// Menu helper function for updating chord intervals after a scale or degree change
void setChordIntervals(int i)
{
  for (int j = 0; j < 7; j++)
  {
    pads[i].chord.intervals[j] = degreeToScaleInterval(
        pads[i].chord.degree + chordDegrees[j], scales[settings.scaleIndex], 7);
  }
  pads[i].chord.intervals[7] = pads[i].chord.intervals[0];
}

// Menu helper function to calculate interval within the scale,
// accounting for octave shifts
int degreeToScaleInterval(int degree, int scale[], int scaleLength)
{
  int baseDegree = degree % scaleLength;
  int octaveShift = (degree / scaleLength) * 12;
  return scale[baseDegree] + octaveShift;
}

// main function for updating the neopixels
void updatePixels()
{
  pixels.clear();

  if (shiftState)
  {
    // Shift key lighting
    pixels.setPixelColor(0, settings.shiftColor);
  }
  else if (screenIndex != -1)
  {
    // Shift key lighting dimly
    pixels.setPixelColor(0, dimColor(settings.shiftColor, dimFactor));
  }

  for (int i = 0; i < PADS_COUNT; i++)
  {
    if (i == selectedPad && !shiftState)
    {
      pixels.setPixelColor(i + 1, pads[i].color);
    }
    else
    {
      pixels.setPixelColor(i + 1, dimColor(pads[i].color, dimFactor));
    }

    if (keyStates[i] || midiStates[i])
    {
      pixels.setPixelColor(i + 1, 0xFFFFFF);
    }
  }
  pixels.show();
}

// Helper function for dimming a pixel color into a dimmer color
uint32_t dimColor(uint32_t color, float factor)
{
  // Extract the red, green, and blue components
  uint8_t red = (color >> 16) & 0xFF;  // Extract red
  uint8_t green = (color >> 8) & 0xFF; // Extract green
  uint8_t blue = color & 0xFF;         // Extract blue

  // Scale each component by the factor
  red = static_cast<uint8_t>(red * factor);
  green = static_cast<uint8_t>(green * factor);
  blue = static_cast<uint8_t>(blue * factor);

  // Reassemble the dimmed color
  uint32_t dimmedColor = (red << 16) | (green << 8) | blue;
  return dimmedColor;
}
