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

int screenIndex = -1;               // Which screen is shown?
const int copyMenuScreenIndex = 13; // shared screen index for the copy menu
int menuIndex = 0;                  // Which menu item is selected?
bool editMenuItem = false;          // Edit the selected menu item
int noteIndex = 0;                  // Which of the 8 notes in a chord to select
float dimFactor = 0.3;              // How much to dim the LED colors

void setup()
{

  Stored::load();
  setAllNoteIntervals();
  initHardware(Stored::settings);

  pinMode(RX_PIN, INPUT_PULLUP);
  Serial1.begin(31250);

  if (!LittleFS.begin())
  {
    showMessage("LittleFS failed");
  }
}

void loop()
{
  checkKeys();
  updateMenu();
  checkMidiIn();
  handleKeyChanges(screenIndex, menuIndex, copyMenuScreenIndex);
  updatePixels();
}

// ********************************** Loop Functions ************************************

// Main function to update the menu
void updateMenu()
{
  Hardware::display.clearDisplay();
  switch (screenIndex)
  {
  case -2:
    menuLoad();
    Display::drawLoadScreen(selectedSlot);
    break;
  case -1:
    menuMain();
    Display::drawMainMenu(selectedPad);
    break;
  case 0:
    menuRoot();
    Display::drawRootMenu();
    break;
  case 1:
    menuScale();
    Display::drawScaleMenu();
    break;
  case 2:
    // menuSustain()
    break;
  case 3:
    // menuMute()
    break;
  case 4:
    menuChordDegree();
    Display::drawDegreeMenu(selectedPad);
    break;
  case 5:
    menuChordRandomVelocity();
    Display::drawVariationMenu(selectedPad);
    break;
  case 6:
    // menuTime()
    break;
  case 7:
    menuNoteActive();
    Display::drawNoteMenu(selectedPad, noteIndex);
    break;
  case 8:
    menuNoteChannel();
    Display::drawChannelMenu(selectedPad, noteIndex, menuIndex);
    break;
  case 9:
    menuNoteVelocity();
    Display::drawVelocityMenu(selectedPad, noteIndex, menuIndex);
    break;
  case 10:
    menuNoteOffset();
    Display::drawNoteOffsetMenu(selectedPad, noteIndex, menuIndex);
    break;
  case 11:
    menuNoteOctaves();
    Display::drawOctaveMenu(selectedPad, noteIndex, menuIndex);
    break;
  case copyMenuScreenIndex:
    Display::drawCopyMenu(selectedPad);
    break;
  case 14:
    menuMidi();
    Display::drawMidiMenu(menuIndex, editMenuItem);
    break;
  case 15:
    menuSave();
    Display::drawSaveMenu(selectedSlot);
    break;
  }
  Hardware::display.display();
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
    setAllNoteIntervals();
  }
}

void menuChordDegree()
{
  if (encoderValue != 0)
  {
    killAllNotes();
    pads[selectedPad].chord.degree = readEncoderConstrained(
        pads[selectedPad].chord.degree, 1, 0, 6);
    setNoteIntervals(selectedPad);
  }
}

void menuNoteActive()
{
  noteIndex = readEncoder(noteIndex, CHORD_NOTE_COUNT);
  if (encoderState && !previousEncoderState)
  {
    killAllNotes();
    pads[selectedPad].chord.isActive[noteIndex] = !pads[selectedPad].chord.isActive[noteIndex];
  }
}

void menuChordRandomVelocity()
{
  pads[selectedPad].velocityRandom = readEncoderFast(
      pads[selectedPad].velocityRandom, 1, 0, VELOCITY_RANDOM_MAX);
}

void menuNoteVelocity()
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
      int &velocityModifier = pads[selectedPad].chord.velocityModifiers[menuIndex - 1];
      velocityModifier = readEncoderFast(velocityModifier, 1, -127, 127);
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

void menuNoteOctaves()
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
      int &octaveModifier = pads[selectedPad].chord.octaveModifiers[menuIndex - 1];
      octaveModifier = readEncoderConstrained(octaveModifier, 1, -5, 5);
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
    menuIndex = readEncoder(menuIndex, 5);
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
      settings.midiInChannel = updateMidiChannel(settings.midiInChannel);
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

// Helper function for updating all note intervals after a scale change
void setAllNoteIntervals()
{
  for (int padIndex = 0; padIndex < PADS_COUNT; padIndex++)
  {
    setNoteIntervals(padIndex);
  }
}

void setNoteIntervals(int padIndex)
{
  for (int noteIndex = 0; noteIndex < 8; noteIndex++)
  {
    if (noteIndex == 7) // set 8th note to root
    {
      pads[padIndex].chord.intervals[noteIndex] = 0;
    }
    else
    {
      pads[padIndex].chord.intervals[noteIndex] = getInterval(
          pads[padIndex].chord.degree + chordDegrees[noteIndex],
          scales[settings.scaleIndex],
          7);
    }
  }
}

int getInterval(int degree, int scale[], int scaleLength)
{
  int baseDegree = degree % scaleLength;
  int octaveShift = (degree / scaleLength) * 12;
  return scale[baseDegree] + octaveShift;
}

// main function for updating the neopixels
void updatePixels()
{
  pixels.clear();

  if (shiftState) // Shift key is pressed
  {
    pixels.setPixelColor(0, settings.shiftColor);
  }

  if (!shiftState && screenIndex != -1) // Some menu is in operation
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
