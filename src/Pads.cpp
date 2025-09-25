#include "Constants.h"
#include "Display.h"
#include "Stored.h"
#include "Midi.h"
#include "Pads.h"

using namespace Stored;
using namespace Display;
using namespace Midi;

namespace Pads
{
  int selectedPad = 0;
  int selectedSlot = 0;
  int copyIndex = -1;
  bool shiftState = false;
  bool previousShiftState = false;
  bool encoderState = false;
  bool previousEncoderState = false;
  bool keyStates[PADS_COUNT] = {false};
  bool previousKeyStates[PADS_COUNT] = {false};
  bool shiftPressedAtKeyDown[PADS_COUNT] = {false};

  void copyPad(int target, int source)
  {
    pads[target].chord = pads[source].chord;
    String message = "Copied " + String(source + 1) + String(target + 1);
    showMessage(message);
  }

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

  void actOnPadStates(int &screenIndex, int &menuIndex)
  {
    // Return to main menu is shift is pressed
    if (shiftState && !previousShiftState)
    {
      screenIndex = -1;
    }

    for (int i = 0; i < PADS_COUNT; i++)
    {
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
}