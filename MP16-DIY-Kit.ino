#include "./src/Constants.h"
#include "./src/Definitions.h"
#include "./src/Hardware.h"
#include "./src/Stored.h"
#include "./src/Display.h"
#include "./src/MusicTheory.h"

using namespace MusicTheory;
using namespace Hardware;
using namespace Stored;
using namespace Display;

// Key state arrays
bool shiftState = false;
bool previousShiftState = false;
bool encoderState = false;
bool previousEncoderState = false;
bool keyStates[PADS_COUNT] = {false};
bool previousKeyStates[PADS_COUNT] = {false};
bool shiftPressedAtKeyDown[PADS_COUNT] = {false};
bool padStates[PADS_COUNT] = {false};
bool previousPadStates[PADS_COUNT] = {false};
bool midiStates[PADS_COUNT] = {false};
bool previousMidiStates[PADS_COUNT] = {false};

// Menu and selection indices
int screenIndex = -1;      // Which screen is shown?
int menuIndex = 0;         // Which menu item is selected?
bool editMenuItem = false; // Edit the selected menu item
int noteIndex = 0;         // Which of the 8 notes in a chord to select
int selectedPad = 0;       // Currently selected pad
int slotSelect = 0;
int copyIndex = -1;
bool recording = false;

// How much to dim the LED colors
float dimFactor = 0.3;

// Integer array to track how many keys are referencing each MIDI note on each channel
int noteCountA[128] = {0};
int noteCountB[128] = {0};
int noteCountC[128] = {0};
int noteCountD[128] = {0};

void setup()
{
  initHardware();

  pinMode(RX_PIN, INPUT_PULLUP);
  Serial1.begin(31250);

  if (!LittleFS.begin())
  {
    showFileSystemFailure();
  }

  Stored::load();
}

void loop()
{
  checkKeys();
  updateMenu();
  updateMIDI();
  updatePixels();
  updateDisplay(screenIndex, selectedPad, menuIndex, slotSelect, noteIndex, editMenuItem);
}

// ********************************** Loop Functions ************************************

// Checking all keys to update their input
void checkKeys()
{
  // Set all previous states to current states
  previousShiftState = shiftState;
  previousEncoderState = encoderState;
  for (int i = 0; i < PADS_COUNT; i++)
  {
    previousKeyStates[i] = keyStates[i];
  }

  // Now update all current states
  shiftState = !digitalRead(SHIFT_PIN);
  encoderState = !digitalRead(ENCODER_S);

  // Read through the keymatrix to update key states
  for (int row = 0; row < ROW_COUNT; row++)
  {
    digitalWrite(ROW0_PIN, row == 0 ? LOW : HIGH);
    digitalWrite(ROW1_PIN, row == 1 ? LOW : HIGH);
    digitalWrite(ROW2_PIN, row == 2 ? LOW : HIGH);
    digitalWrite(ROW3_PIN, row == 3 ? LOW : HIGH);
    for (int col = 0; col < COL_COUNT; col++)
    {
      int keyIndex = row * COL_COUNT + col;
      bool currentKeyState = !digitalRead(COL0_PIN + col);
      keyStates[keyIndex] = currentKeyState;
    }
  }
  // set last row high again after scanning
  digitalWrite(ROW3_PIN, HIGH);
}

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
  slotSelect = readEncoder(slotSelect, SLOT_COUNT);
  if (encoderState && !previousEncoderState)
  {
    killAllNotes();
    screenIndex = -1;
    if (Stored::loadFromFlash(slotSelect))
    {
      display.clearDisplay();
      display.setCursor(22, 28);
      display.print("Loaded Slot ");
      display.print(slotSelect + 1);
      display.display();
      delay(1000);
    }
    else
    {
      display.clearDisplay();
      display.setCursor(22, 28);
      display.print("Loading Failed");
      display.display();
      delay(2000);
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
    pads[selectedPad].chord.degree = readEncoderConstrained(pads[selectedPad].chord.degree, 1, 0, 6);
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
  pads[selectedPad].velocityRandom = readEncoderFast(pads[selectedPad].velocityRandom, 1, 0, VELOCITY_RANDOM_MAX);
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
      pads[selectedPad].chord.velocityModifiers[menuIndex - 1] = readEncoderFast(pads[selectedPad].chord.velocityModifiers[menuIndex - 1], 1, -128, 128);
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
      pads[selectedPad].chord.octaveModifiers[menuIndex - 1] =
          readEncoderConstrained(pads[selectedPad].chord.octaveModifiers[menuIndex - 1], 1, -3, 3);
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
  slotSelect = readEncoder(slotSelect, SLOT_COUNT);
  if (encoderState && !previousEncoderState)
  {
    saveToFlash(slotSelect);
    screenIndex = -1;
    menuIndex = 0;
    display.clearDisplay();
    display.setCursor(22, 28);
    display.print("Settings Saved");
    display.display();
    delay(1000);
  }
}

// Menu helper function to kill all notes in reference
void killAllNotes()
{
  for (int i = 0; i < 128; i++)
  {
    if (noteCountA[i] > 0)
    {
      sendNoteOff(i, 0, settings.midiOutputAChannel);
    }
    noteCountA[i] = 0;
    if (noteCountB[i] > 0)
    {
      sendNoteOff(i, 0, settings.midiOutputBChannel);
    }
    noteCountB[i] = 0;
    if (noteCountC[i] > 0)
    {
      sendNoteOff(i, 0, settings.midiOutputCChannel);
    }
    noteCountC[i] = 0;
    if (noteCountD[i] > 0)
    {
      sendNoteOff(i, 0, settings.midiOutputDChannel);
    }
    noteCountD[i] = 0;
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
    pads[i].chord.intervals[j] =
        degreeToScaleInterval(pads[i].chord.degree + chordDegrees[j], scales[settings.scaleIndex], 7);
  }
  pads[i].chord.intervals[7] = pads[i].chord.intervals[0];
}

// Menu helper function to calculate interval within the scale, accounting for octave shifts
int degreeToScaleInterval(int degree, int scale[], int scaleLength)
{
  int baseDegree = degree % scaleLength;
  int octaveShift = (degree / scaleLength) * 12;
  return scale[baseDegree] + octaveShift;
}

// Save current settings to a memory slot
void saveToFlash(int slot)
{
  String filePath = "/slot" + String(slot) + ".txt"; // Use String for concatenation
  File file = LittleFS.open(filePath, "w");          // Pass the constructed file path
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  file.write((uint8_t *)&settings, sizeof(settings));
  file.write((uint8_t *)&pads, sizeof(pads));
  file.close();
}

// Main MIDI update function
void updateMIDI()
{
  for (int i = 0; i < PADS_COUNT; i++)
  {
    previousMidiStates[i] = midiStates[i];
    previousPadStates[i] = padStates[i];
  }

  if (midiMessageReady)
  {
    midiMessageReady = false; // Reset the flag
    // Process the MIDI message (status, data1, data2)
    processIncomingMIDI(status, data1, data2);
  }

  // Check for incoming USB MIDI messages
  if (usb_midi.available())
  {
    uint32_t packet = usb_midi.read();
    uint8_t status = (packet >> 8) & 0xFF; // Status byte
    uint8_t data1 = (packet >> 16) & 0xFF; // Data1 (e.g., note)
    uint8_t data2 = (packet >> 24) & 0xFF; // Data2 (e.g., velocity)
    processIncomingMIDI(status, data1, data2);
  }

  if (shiftState && !previousShiftState)
  { // Reset shortcut screen index is shift is pressed
    screenIndex = -1;
  }

  for (int i = 0; i < PADS_COUNT; i++)
  { // Go through all pads

    // Check for rising or falling edges in the keys
    if (keyStates[i] && !previousKeyStates[i])
    { // Key is pressed
      if (shiftState)
      { // Check if we're using a SHIFT shortcut
        shiftPressedAtKeyDown[i] = true;
        screenIndex = i;
        if (i == 13)
        {
          copyIndex = selectedPad;
        }
        menuIndex = 0;
      }
      else if (screenIndex == 13)
      { // Check whether we're copying a pad
        killAllNotes();
        if (copyIndex != -1 && copyIndex != i)
        {
          copyPad(i, copyIndex);
          copyIndex = -1;
        }
        screenIndex = -1;
      }
      else
      { // Change pad state
        shiftPressedAtKeyDown[i] = false;
        padStates[i] = true;
        selectedPad = i;
      }
    }
    else if (!keyStates[i] && previousKeyStates[i])
    { // Key is released
      if (!shiftPressedAtKeyDown[i])
      {
        for (int j = 0; j < 8; j++)
        {
          if (pads[i].chord.isActive[j])
          {
            if (!midiStates[i])
            {
              padStates[i] = false;
            }
          }
        }
      }
    }

    // Check for rising and falling edges in the midiTriggers
    if (midiStates[i] && !previousMidiStates[i])
    {
      padStates[i] = true;
    }
    else if (!midiStates[i] && previousMidiStates[i])
    {
      if (!keyStates[i])
      {
        padStates[i] = false;
      }
    }

    // Finally check rising or falling edges in pad playing status
    if (padStates[i] && !previousPadStates[i])
    {
      playChord(i);
    }
    else if (!padStates[i] && previousPadStates[i])
    {
      stopChord(i);
    }
  }
}

void processIncomingMIDI(uint8_t status, uint8_t data1, uint8_t data2)
{
  uint8_t command = status & 0xF0; // Mask out the channel bits
  uint8_t channel = status & 0x0F; // Get the channel
  if (settings.midiThru)
  {
    forwardMIDI(status, data1, data2);
  }

  if (channel == settings.midiRecChannel && recording)
  {
    // store incoming notes into a buffer for structuring into a chord later?
  }

  if (channel == settings.midiTrigChannel && (command == 144 || command == 128))
  {
    for (int i = 0; i < PADS_COUNT; i++)
    {
      if (data1 == pads[i].triggerNote)
      {
        if (data2 > 0 && command == 144)
        {
          midiStates[i] = true;
        }
        else
        {
          midiStates[i] = false;
        }
      }
    }
  }
}

void forwardMIDI(uint8_t status, uint8_t data1, uint8_t data2)
{
  // Forward to USB MIDI
  uint8_t usb_packet[] = {status, data1, data2};
  usb_midi.write(usb_packet, 3);

  // Forward to Serial MIDI
  Serial1.write(status);
  Serial1.write(data1);
  Serial1.write(data2);
}

void playChord(int i)
{
  for (int j = 0; j < 8; j++)
  {
    if (pads[i].chord.isActive[j])
    {
      playNote(i, j);
    }
  }
}

void playNote(int pad, int j)
{
  int note = settings.rootNote + pads[pad].chord.intervals[j] + (pads[pad].chord.octaveModifiers[j] * 12) + pads[pad].chord.semitoneModifiers[j];
  int velocity = constrain(settings.velocityScaling * (pads[pad].padVelocity + pads[pad].chord.velocityModifiers[j] + random(-pads[pad].velocityRandom, pads[pad].velocityRandom)), 1, 128);

  // Send NoteOff to stop any existing note in the correct channel, then increment reference count and play new note in that channel
  switch (pads[pad].chord.channel[j])
  {
  case 0:
    if (noteCountA[note] > 0)
    {
      sendNoteOff(note, velocity, settings.midiOutputAChannel);
    }
    noteCountA[note]++;
    sendNoteOn(note, velocity, settings.midiOutputAChannel);
    break;
  case 1:
    if (noteCountB[note] > 0)
    {
      sendNoteOff(note, velocity, settings.midiOutputBChannel);
    }
    noteCountB[note]++;
    sendNoteOn(note, velocity, settings.midiOutputBChannel);
    break;
  case 2:
    if (noteCountC[note] > 0)
    {
      sendNoteOff(note, velocity, settings.midiOutputCChannel);
    }
    noteCountC[note]++;
    sendNoteOn(note, velocity, settings.midiOutputCChannel);
    break;
  case 3:
    if (noteCountD[note] > 0)
    {
      sendNoteOff(note, velocity, settings.midiOutputDChannel);
    }
    noteCountD[note]++;
    sendNoteOn(note, velocity, settings.midiOutputDChannel);
    break;
  }
}

void stopChord(int i)
{
  for (int j = 0; j < 8; j++)
  {
    if (pads[i].chord.isActive[j])
    {
      stopNote(i, j);
    }
  }
}

void stopNote(int pad, int j)
{
  int note = settings.rootNote + pads[pad].chord.intervals[j] + (pads[pad].chord.octaveModifiers[j] * 12) + pads[pad].chord.semitoneModifiers[j];

  // Decrement the reference count and send NoteOff if it's the last reference
  switch (pads[pad].chord.channel[j])
  {
  case 0:
    if (noteCountA[note] > 0)
    {
      noteCountA[note]--;
      if (noteCountA[note] == 0)
      {
        sendNoteOff(note, 0, settings.midiOutputAChannel);
      }
    }
    break;
  case 1:
    if (noteCountB[note] > 0)
    {
      noteCountB[note]--;
      if (noteCountB[note] == 0)
      {
        sendNoteOff(note, 0, settings.midiOutputBChannel);
      }
    }
    break;
  case 2:
    if (noteCountC[note] > 0)
    {
      noteCountC[note]--;
      if (noteCountC[note] == 0)
      {
        sendNoteOff(note, 0, settings.midiOutputCChannel);
      }
    }
    break;
  case 3:
    if (noteCountD[note] > 0)
    {
      noteCountD[note]--;
      if (noteCountD[note] == 0)
      {
        sendNoteOff(note, 0, settings.midiOutputDChannel);
      }
    }
    break;
  }
}

void sendNoteOn(int note, int velocity, int channel)
{
  if (channel < 0 || channel > 15)
    return;
  uint8_t status = 0x90 | (channel); // 0x90 is "Note On", and channel is adjusted to 0-based
  uint8_t usb_packet[] = {status, (uint8_t)note, (uint8_t)velocity};
  usb_midi.write(usb_packet, 3);
  Serial1.write(status);
  Serial1.write(note);
  Serial1.write(velocity);
}

void sendNoteOff(int note, int velocity, int channel)
{
  if (channel < 0 || channel > 15)
    return;
  uint8_t status = 0x80 | (channel);
  uint8_t usb_packet[] = {status, (uint8_t)note, (uint8_t)velocity};
  usb_midi.write(usb_packet, 3);
  Serial1.write(status);
  Serial1.write(note);
  Serial1.write(velocity);
}

void copyPad(int target, int source)
{
  pads[target].chord = pads[source].chord;
  showCopyMessage(source, target);
}

// main function for updating the neopixels
void updatePixels()
{
  pixels.clear();

  if (shiftState)
  {
    pixels.setPixelColor(0, settings.shiftColor); // Shift key lighting
  }
  else if (screenIndex != -1)
  {
    pixels.setPixelColor(0, dimColor(settings.shiftColor, dimFactor)); // Shift key lighting dimly
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
