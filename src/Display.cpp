#include "DisplayHelpers.h"
#include "Hardware.h"
#include "Stored.h"
#include "Display.h"
#include "MusicTheory.h"

using namespace MusicTheory;
using namespace Hardware;
using namespace Stored;

namespace Display
{
  void drawLoadScreen(int selectedSlot)
  {
    display.setCursor(40, 0);
    display.print("Load:");
    drawSlots(selectedSlot);
  }

  void drawMainMenu(int selectedPad)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(66, 0);
    display.print("Degree: ");
    display.print(pads[selectedPad].chord.degree + 1);
    drawChordNotes(selectedPad);
    drawNoteVelocities(selectedPad);
  }

  void drawRootMenu()
  {
    drawCentered("Root", 16);
    display.drawLine(10, 28, 118, 28, WHITE);
    drawCentered(midiNoteNames[settings.rootNote], 34);
  }

  void drawScaleMenu()
  {
    drawCentered("Scale", 16);
    display.drawLine(10, 28, 118, 28, WHITE);
    drawCentered(scaleNames[settings.scaleIndex], 34);
  }

  void drawDegreeMenu(int selectedPad)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(66, 0);
    display.print("Degree: ");
    display.print(pads[selectedPad].chord.degree + 1);
    display.drawLine(66, 11, 120, 11, WHITE);
    drawChordNotes(selectedPad);
  }

  void drawNoteMenu(int selectedPad, int noteIndex)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(60, 0);
    display.print("Note ON/OFF");
    drawChordNotes(selectedPad);
    drawNoteBlockSelector(noteIndex);
  }

  void drawVariationMenu(int selectedPad)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(4, 28);
    display.print("Velocity Random: ");
    display.print(pads[selectedPad].velocityRandom);
  }

  void drawVelocityMenu(int selectedPad, int noteIndex, int menuIndex)
  {
    drawSelectedPad(selectedPad);
    drawFromRight("Velocity Offset", 128, 0);
    drawChordNotes(selectedPad);
    drawNoteVelocityModifiers(selectedPad);
    drawOctaveSelector(noteIndex, menuIndex);
  }

  void drawNoteOffsetMenu(int selectedPad, int noteIndex, int menuIndex)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(62, 0);
    display.print("Note Offset");
    drawChordNotes(selectedPad, true);
    drawNoteOffsetSelector(noteIndex, menuIndex);
  }

  void drawOctaveMenu(int selectedPad, int noteIndex, int menuIndex)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(46, 0);
    display.print("Octave Offset");
    drawChordNotes(selectedPad);
    drawNoteOctaves(selectedPad, noteIndex, menuIndex);
    drawOctaveSelector(noteIndex, menuIndex);
  }

  void drawCopyMenu(int selectedPad)
  {
    drawCentered("Copy Pad " + String(selectedPad + 1), 16);
    display.drawLine(10, 28, 118, 28, WHITE);
    drawCentered("Select Destination", 34);
  }

  void drawMidiMenu(int menuIndex, bool editMenuItem)
  {
    display.setCursor(5, 1);
    display.print("MIDI Out A");
    drawFromRight(settings.midiOutputAChannel + 1, 124, 1);
    display.setCursor(5, 11);
    display.print("MIDI Out B");
    drawFromRight(settings.midiOutputBChannel + 1, 124, 11);
    display.setCursor(5, 21);
    display.print("MIDI Out C");
    drawFromRight(settings.midiOutputCChannel + 1, 124, 21);
    display.setCursor(5, 31);
    display.print("MIDI Out D");
    drawFromRight(settings.midiOutputDChannel + 1, 124, 31);
    display.setCursor(5, 41);
    display.print("MIDI In Rec");
    drawFromRight(settings.midiRecChannel + 1, 124, 41);
    display.setCursor(5, 51);
    display.print("MIDI In Trigger");
    drawFromRight(settings.midiTrigChannel + 1, 124, 51);

    if (editMenuItem)
    {
      display.fillRect(112, menuIndex * 10, 12, 9, INVERSE);
    }
    if (!editMenuItem)
    {
      display.fillRect(3, menuIndex * 10, 121, 9, INVERSE);
    }
  }

  void drawSaveScreen(int selectedSlot)
  {
    display.setCursor(40, 0);
    display.print("Save to:");
    drawSlots(selectedSlot);
  }

  // Main function for updating the OLED display
  void updateDisplay(int screenIndex,
                     int selectedPad,
                     int menuIndex,
                     int selectedSlot,
                     int noteIndex,
                     bool editMenuItem)
  {
    display.clearDisplay();
    switch (screenIndex)
    {
    case -2:
      drawLoadScreen(selectedSlot);
      break;
    case -1:
      drawMainMenu(selectedPad);
      break;
    case 0:
      drawRootMenu();
      break;
    case 1:
      drawScaleMenu();
      break;
    case 2:
      drawDegreeMenu(selectedPad);
      break;
    case 3:
      drawNoteMenu(selectedPad, noteIndex);
      break;
    case 4:
      drawVariationMenu(selectedPad);
      break;
    case 5:
      drawVelocityMenu(selectedPad, noteIndex, menuIndex);
      break;
    case 6:
      drawNoteOffsetMenu(selectedPad, noteIndex, menuIndex);
      break;
    case 7:
      drawOctaveMenu(selectedPad, noteIndex, menuIndex);
      break;
    case 13:
      drawCopyMenu(selectedPad);
      break;
    case 14:
      drawMidiMenu(menuIndex, editMenuItem);
      break;
    case 15:
      drawSaveScreen(selectedSlot);
      break;
    }

    display.display();
  }

  void showMessage(String message, int duration)
  {
    display.clearDisplay();
    drawCentered(message, 28);
    display.display();
    delay(duration);
  }
}