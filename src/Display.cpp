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
    display.print(pads[selectedPad].velocityVariation);
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

  void drawMidiMenu(int menuIndex)
  {
    display.setCursor(5, 0);
    display.print("MIDI In Rec:  ");
    display.println(settings.midiRecChannel + 1);
    display.setCursor(5, 10);
    display.print("MIDI In Trig: ");
    display.println(settings.midiTrigChannel + 1);
    display.setCursor(5, 20);
    display.print("MIDI Out A:   ");
    display.println(settings.midiOutputAChannel + 1);
    display.setCursor(5, 30);
    display.print("MIDI Out B:   ");
    display.println(settings.midiOutputBChannel + 1);
    display.setCursor(5, 40);
    display.print("MIDI Out C:   ");
    display.println(settings.midiOutputCChannel + 1);
    display.setCursor(5, 50);
    display.print("MIDI Out D:   ");
    display.println(settings.midiOutputDChannel + 1);

    switch (menuIndex)
    {
    case 0:
      display.drawLine(5, 8, 124, 8, WHITE);
      break;
    case 1:
      display.drawLine(5, 18, 124, 18, WHITE);
      break;
    case 2:
      display.drawLine(5, 28, 124, 28, WHITE);
      break;
    case 3:
      display.drawLine(5, 38, 124, 38, WHITE);
      break;
    case 4:
      display.drawLine(5, 48, 124, 48, WHITE);
      break;
    case 5:
      display.drawLine(5, 58, 124, 58, WHITE);
      break;
    }
  }

  void drawSaveScreen(int slotSelect)
  {
    display.setCursor(40, 0);
    display.print("Save to:");
    drawSlots(slotSelect);
  }

  // Main function for updating the OLED display
  void updateDisplay(int screenIndex, int selectedPad, int menuIndex, int slotSelect, int noteIndex)
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
      drawMidiMenu(menuIndex);
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