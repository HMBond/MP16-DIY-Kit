#include "Pads.h"

#include "Constants.h"
#include "Display.h"
#include "Midi.h"
#include "Stored.h"

int selectedPad = 0;
int selectedSlot = 0;
int copySource = -1;
bool shiftState = false;
bool previousShiftState = false;
bool encoderState = false;
bool previousEncoderState = false;
bool keyStates[PADS_COUNT] = {false};
bool previousKeyStates[PADS_COUNT] = {false};
bool shiftPressedAtKeyDown[PADS_COUNT] = {false};

void copyPad(int target, int source) {
  killAllNotes();
  pads[target].chord = pads[source].chord;
  String message =
      "Copied pad " + String(source + 1) + " to " + String(target + 1);
  drawMessage(message);
  copySource = -1;
}

// Checking all keys to update their input
void checkKeys() {
  // Set all previous states to current states
  previousShiftState = shiftState;
  previousEncoderState = encoderState;
  for (int i = 0; i < PADS_COUNT; i++) {
    previousKeyStates[i] = keyStates[i];
  }

  // Now update all current states
  shiftState = !digitalRead(SHIFT_PIN);
  encoderState = !digitalRead(ENCODER_S);

  // Read through the keymatrix to update key states
  for (int row = 0; row < ROW_COUNT; row++) {
    digitalWrite(ROW0_PIN, row == 0 ? LOW : HIGH);
    digitalWrite(ROW1_PIN, row == 1 ? LOW : HIGH);
    digitalWrite(ROW2_PIN, row == 2 ? LOW : HIGH);
    digitalWrite(ROW3_PIN, row == 3 ? LOW : HIGH);
    for (int col = 0; col < COL_COUNT; col++) {
      int keyIndex = row * COL_COUNT + col;
      bool currentKeyState = !digitalRead(COL0_PIN + col);
      keyStates[keyIndex] = currentKeyState;
    }
  }
  // set last row high again after scanning
  digitalWrite(ROW3_PIN, HIGH);
}

void handleKeyChanges(int &screenIndex, int &menuIndex,
                      int copyMenuScreenIndex) {
  // Return to main menu is shift is pressed
  if (shiftState && !previousShiftState) {
    screenIndex = -1;
  }

  for (int i = 0; i < PADS_COUNT; i++) {
    // Key is pressed
    if (keyStates[i] && !previousKeyStates[i]) {
      // Using a SHIFT shortcut
      if (shiftState) {
        shiftPressedAtKeyDown[i] = true;
        screenIndex = i;
        menuIndex = 0;
        // Remember the last selected pad when entering copy menu
        if (i == copyMenuScreenIndex) {
          copySource = selectedPad;
        }
      } else {
        shiftPressedAtKeyDown[i] = false;
        if (!settings.mute) {
          padStates[i] = true;
        }
        selectedPad = i;
      }

      // Copy source pad to selected target pad (i)
      if (!shiftState && screenIndex == copyMenuScreenIndex) {
        killAllNotes();
        if (copySource != -1 && copySource != i) {
          copyPad(i, copySource);
          copySource = -1;
        }
        screenIndex = -1;
      }
    }

    // Key is released
    if (!keyStates[i] && previousKeyStates[i]) {
      if (!shiftPressedAtKeyDown[i]) {
        if (!midiStates[i] && !settings.mute) {
          padStates[i] = false;
        }
      }
    }

    // Check if pads get triggered by MIDI IN
    if (midiStates[i] && !previousMidiStates[i]) {
      padStates[i] = true;
    } else if (!midiStates[i] && previousMidiStates[i]) {
      if (!keyStates[i]) {
        padStates[i] = false;
      }
    }

    // Finally check rising or falling edges in pad playing status
    if (padStates[i] && !previousPadStates[i]) {
      playChord(i);
    } else if (!padStates[i] && previousPadStates[i]) {
      stopChord(i);
    }
  }
}
