#include "./src/Constants.h"
#include "./src/Definitions.h"
#include "./src/Display.h"
#include "./src/MusicTheory.h"
#include "./src/Pads.h"
#include "./src/Stored.h"

int screenIndex = -1;                // Which screen is shown?
const int copyMenuScreenIndex = 13;  // screen index for the copy menu
int menuIndex = 0;                   // Which menu item is selected?
bool editMenuItem = false;           // Edit the selected menu item
int noteIndex = 0;                   // Selected note in menu
float dimFactor = 0.5;               // Dim factor for NeoPixel LEDS

void setup() {
  initHardware();
  if (!LittleFS.begin()) {
    drawMessage("LittleFS failed");
  }
  loadPreset();
  setAllNoteIntervals();
  pixels.setBrightness(settings.ledBrightness);
  pinMode(RX_PIN, INPUT_PULLUP);
  Serial1.begin(31250);
}

void loop() {
  checkKeys();
  updateMenu();
  checkMidiIn();
  handleKeyChanges(screenIndex, menuIndex, copyMenuScreenIndex);
  checkStoppingQueue();
  checkPlayingQueue();
  updatePixels();
}

// Main function to update the menu
void updateMenu() {
  display.clearDisplay();
  switch (screenIndex) {
    case -2:
      loadMenu();
      break;
    case -1:
      mainMenu();
      break;
    case 0:
      rootMenu();
      break;
    case 1:
      scaleMenu();
      break;
    case 2:
      sustainMenu();
      break;
    case 3:
      muteMenu();
      break;
    case 4:
      velocityRandomMenu();
      break;
    case 5:
      timingVariationMenu();
      break;
    case 6:
      degreeMenu();
      break;
    case 7:
      noteActiveMenu();
      break;
    case 8:
      noteChannelMenu();
      break;
    case 9:
      noteVelocityMenu();
      break;
    case 10:
      noteOffsetMenu();
      break;
    case 11:
      noteOctavesMenu();
      break;
    case copyMenuScreenIndex:
      drawCopyMenu(selectedPad);
      break;
    case 14:
      midiMenu();
      break;
    case 15:
      saveMenu();
      break;
  }
  display.display();
}

void loadMenu() {
  selectedSlot = readEncoder(selectedSlot, SLOT_COUNT);
  drawLoadScreen(selectedSlot);
  if (encoderState && !previousEncoderState) {
    killAllNotes();
    screenIndex = -1;
    if (loadFromFlash(selectedSlot)) {
      setAllNoteIntervals();
      drawMessage("Loaded Slot " + String(selectedSlot + 1));
    } else {
      drawMessage("Loading Failed");
    }
  }
}

void mainMenu() {
  settings.velocityScaling =
      readEncoderFast(settings.velocityScaling, 0.01, 0.015625, 2.0);
  if (encoderState && !previousEncoderState) {
    screenIndex = -2;
  }
  drawMainMenu(selectedPad);
}

void rootMenu() {
  killAllNotes();
  settings.rootNote =
      readEncoderConstrained(settings.rootNote, 1, ROOT_MIN, ROOT_MAX);
  drawRootMenu();
}

void scaleMenu() {
  if (encoderValue != 0) {
    killAllNotes();
    settings.scaleIndex = readEncoder(settings.scaleIndex, SCALE_COUNT);
    setAllNoteIntervals();
  }
  drawScaleMenu();
}

void degreeMenu() {
  if (encoderValue != 0) {
    killAllNotes();
    pads[selectedPad].chord.degree =
        readEncoderConstrained(pads[selectedPad].chord.degree, 1, 0, 6);
    setNoteIntervals(selectedPad);
  }
  drawDegreeMenu(selectedPad);
}

void noteActiveMenu() {
  noteIndex = readEncoder(noteIndex, CHORD_NOTE_COUNT);
  if (encoderState && !previousEncoderState) {
    killAllNotes();
    pads[selectedPad].chord.isActive[noteIndex] =
        !pads[selectedPad].chord.isActive[noteIndex];
  }
  drawNoteActiveMenu(selectedPad, noteIndex);
}

void sustainMenu() {
  settings.sustain = !settings.sustain;
  screenIndex = -1;
  killAllNotes();
  drawMessage(settings.sustain ? "Sustain ON" : "Sustain OFF", 500);
}

void muteMenu() {
  settings.mute = !settings.mute;
  screenIndex = -1;
  drawMessage(settings.mute ? "Mute ON" : "Mute OFF", 500);
}

void velocityRandomMenu() {
  pads[selectedPad].velocityRandom =
      readEncoderFast(pads[selectedPad].velocityRandom, 1, 0, 127);
  drawVelocityRandomMenu(selectedPad);
}

void timingVariationMenu() {
  pads[selectedPad].timingVariation =
      readEncoderFast(pads[selectedPad].timingVariation, 1, -50, 50);
  drawTimingVariationMenu(selectedPad);
}

void noteVelocityMenu() {
  if (menuIndex == 0) {
    noteIndex = readEncoder(noteIndex, CHORD_NOTE_COUNT);
    if (encoderState && !previousEncoderState) {
      menuIndex = noteIndex + 1;
    }
  } else if (menuIndex - 1 < CHORD_NOTE_COUNT) {
    if (encoderValue != 0) {
      killAllNotes();
      int& velocityModifier =
          pads[selectedPad].chord.velocityModifiers[menuIndex - 1];
      velocityModifier = readEncoderFast(velocityModifier, 1, -127, 127);
    }
    if (encoderState && !previousEncoderState) {
      menuIndex = 0;
    }
  }
  drawNoteVelocityMenu(selectedPad, noteIndex, menuIndex);
}

void noteOffsetMenu() {
  if (menuIndex == 0) {
    noteIndex = readEncoder(noteIndex, CHORD_NOTE_COUNT);
    if (encoderState && !previousEncoderState) {
      menuIndex = noteIndex + 1;
    }
  } else if (menuIndex - 1 < CHORD_NOTE_COUNT) {
    if (encoderValue != 0) {
      killAllNotes();
      int& semitoneModifier =
          pads[selectedPad].chord.semitoneModifiers[menuIndex - 1];
      semitoneModifier = readEncoderConstrained(semitoneModifier, 1, -7, 7);
    }
    if (encoderState && !previousEncoderState) {
      menuIndex = 0;
    }
  }
  drawNoteOffsetMenu(selectedPad, noteIndex, menuIndex);
}

void noteOctavesMenu() {
  if (menuIndex == 0) {
    noteIndex = readEncoder(noteIndex, CHORD_NOTE_COUNT);
    if (encoderState && !previousEncoderState) {
      menuIndex = noteIndex + 1;
    }
  } else if (menuIndex - 1 < CHORD_NOTE_COUNT) {
    if (encoderValue != 0) {
      killAllNotes();
      int& octaveModifier =
          pads[selectedPad].chord.octaveModifiers[menuIndex - 1];
      octaveModifier = readEncoderConstrained(octaveModifier, 1, -5, 5);
    }
    if (encoderState && !previousEncoderState) {
      menuIndex = 0;
    }
  }
  drawNoteOctaveMenu(selectedPad, noteIndex, menuIndex);
}

void noteChannelMenu() {
  if (menuIndex == 0) {
    noteIndex = readEncoder(noteIndex, CHORD_NOTE_COUNT);
    if (encoderState && !previousEncoderState) {
      menuIndex = noteIndex + 1;
    }
  } else if (menuIndex - 1 < CHORD_NOTE_COUNT) {
    if (encoderValue != 0) {
      killAllNotes();
      pads[selectedPad].chord.channel[menuIndex - 1] = readEncoderConstrained(
          pads[selectedPad].chord.channel[menuIndex - 1], 1, 0, 3);
    }
    if (encoderState && !previousEncoderState) {
      menuIndex = 0;
    }
  }
  drawNoteChannelMenu(selectedPad, noteIndex, menuIndex);
}

// Helper function to handle MIDI channel updates
int updateMidiChannel(int channel) {
  if (encoderValue != 0) {
    killAllNotes();
    return readEncoder(channel, MIDI_CHANNELS);
  }
  return channel;
}

void midiMenu() {
  if (encoderState && !previousEncoderState) {
    editMenuItem = !editMenuItem;
  }
  if (!editMenuItem) {
    menuIndex = readEncoder(menuIndex, 5);
  }
  if (editMenuItem) {
    switch (menuIndex) {
      case 0:
        settings.midiOutputAChannel =
            updateMidiChannel(settings.midiOutputAChannel);
        break;

      case 1:
        settings.midiOutputBChannel =
            updateMidiChannel(settings.midiOutputBChannel);
        break;

      case 2:
        settings.midiOutputCChannel =
            updateMidiChannel(settings.midiOutputCChannel);
        break;

      case 3:
        settings.midiOutputDChannel =
            updateMidiChannel(settings.midiOutputDChannel);
        break;

      case 4:
        settings.midiInChannel = updateMidiChannel(settings.midiInChannel);
        break;
    }
  }
  drawMidiMenu(menuIndex, editMenuItem);
}

void saveMenu() {
  selectedSlot = readEncoder(selectedSlot, SLOT_COUNT);
  if (encoderState && !previousEncoderState) {
    if (saveToFlash(selectedSlot)) {
      screenIndex = -1;
      menuIndex = 0;
      String message = "Saved to Slot " + String(selectedSlot + 1);
      drawMessage(message);
    } else {
      drawMessage("Saving Failed");
    }
  }
  drawSaveMenu(selectedSlot);
}

// Helper function for updating all note intervals after a scale change
void setAllNoteIntervals() {
  for (int padIndex = 0; padIndex < PADS_COUNT; padIndex++) {
    setNoteIntervals(padIndex);
  }
}

void setNoteIntervals(int padIndex) {
  for (int noteIndex = 0; noteIndex < 7; noteIndex++) {
    pads[padIndex].chord.intervals[noteIndex] =
        getInterval(pads[padIndex].chord.degree + chordDegrees[noteIndex],
                    scales[settings.scaleIndex], 7);
  }
  // set 8th note to root
  pads[padIndex].chord.intervals[7] =
      getInterval(pads[padIndex].chord.degree + chordDegrees[0],
                  scales[settings.scaleIndex], 7);
}

int getInterval(int degree, int scale[], int scaleLength) {
  int baseDegree = degree % scaleLength;
  int octaveShift = (degree / scaleLength) * 12;
  return scale[baseDegree] + octaveShift;
}

// main function for updating the NeoPixels
void updatePixels() {
  pixels.clear();

  // Shift key is pressed
  if (shiftState) {
    pixels.setPixelColor(0, settings.shiftColor);
  }

  // Some menu is in operation
  if (!shiftState && screenIndex != -1) {
    // Shift key lighting dimly
    pixels.setPixelColor(0, dimColor(settings.shiftColor, dimFactor));
  }

  for (int i = 0; i < PADS_COUNT; i++) {
    if (i == selectedPad && !shiftState) {
      // highlight selected pad unless shift key is pressed
      pixels.setPixelColor(i + 1, pads[i].color);
    } else if (settings.mute) {
      pixels.setPixelColor(i + 1, dimColor(0xFF0000, dimFactor));
    } else if (settings.sustain) {
      pixels.setPixelColor(i + 1, dimColor(0x00FF00, dimFactor));
    } else {
      pixels.setPixelColor(i + 1, dimColor(pads[i].color, dimFactor));
    }

    if ((!settings.mute && keyStates[i]) || midiStates[i]) {
      pixels.setPixelColor(i + 1, 0xFFFFFF);
    }
  }
  pixels.show();
}
