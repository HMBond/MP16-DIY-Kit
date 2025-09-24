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
  void drawLoadScreen(int slotSelect)
  {
    display.setCursor(40, 0);
    display.print("Load:");
    drawSlots(slotSelect);
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
    displayCentered("Root", 16);
    display.drawLine(10, 28, 118, 28, WHITE);
    displayCentered(midiNoteNames[settings.rootNote], 34);
  }

  void drawScaleMenu()
  {
    displayCentered("Scale", 16);
    display.drawLine(10, 28, 118, 28, WHITE);
    displayCentered(scaleNames[settings.scaleIndex], 34);
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
    display.setCursor(50, 0);
    display.print("Note Velocity");
    drawChordNotes(selectedPad);
    drawNoteVelocities(selectedPad);
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
    String copyPad = "Copy Pad ";
    String label = copyPad + (selectedPad + 1);
    displayCentered(label, 16);
    display.drawLine(10, 28, 118, 28, WHITE);
    displayCentered("Select Destination", 34);
  }

  void drawMidiMenu(int menuIndex, bool editMenuItem)
  {
    char buffer[2];
    display.setCursor(5, 1);
    display.print("MIDI Out A");
    itoa(settings.midiOutputAChannel + 1, buffer, 10);
    displayRight(buffer, 124, 1);
    display.setCursor(5, 11);
    display.print("MIDI Out B");
    itoa(settings.midiOutputBChannel + 1, buffer, 10);
    displayRight(buffer, 124, 11);
    display.setCursor(5, 21);
    display.print("MIDI Out C");
    itoa(settings.midiOutputCChannel + 1, buffer, 10);
    displayRight(buffer, 124, 21);
    display.setCursor(5, 31);
    display.print("MIDI Out D");
    itoa(settings.midiOutputDChannel + 1, buffer, 10);
    displayRight(buffer, 124, 31);
    display.setCursor(5, 41);
    display.print("MIDI In Rec");
    itoa(settings.midiRecChannel + 1, buffer, 10);
    displayRight(buffer, 124, 41);
    display.setCursor(5, 51);
    display.print("MIDI In Trigger");
    itoa(settings.midiTrigChannel + 1, buffer, 10);
    displayRight(buffer, 124, 51);

    if (editMenuItem)
    {
      display.fillRect(112, menuIndex * 10, 12, 9, INVERSE);
    }
    if (!editMenuItem)
    {
      display.fillRect(3, menuIndex * 10, 121, 9, INVERSE);
    }
  }

  void drawSaveScreen(int slotSelect)
  {
    display.setCursor(40, 0);
    display.print("Save to:");
    drawSlots(slotSelect);
  }

  // Main function for updating the OLED display
  void updateDisplay(int screenIndex, int selectedPad, int menuIndex, int slotSelect, int noteIndex, bool editMenuItem)
  {
    display.clearDisplay();
    switch (screenIndex)
    {
    case -2:
      drawLoadScreen(slotSelect);
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
      drawSaveScreen(slotSelect);
      break;
    }

    display.display();
  }

  void showFileSystemFailure()
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("LittleFS failed");
    display.display();
    delay(1000);
  }

  void showCopyMessage(int source, int target)
  {
    display.clearDisplay();
    display.setCursor(22, 28);
    display.print("Copied ");
    display.print(source + 1);
    display.print(" to ");
    display.print(target + 1);
    display.display();
    delay(1000);
  }
}