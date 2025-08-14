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
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(40, 6);
    display.print("Load:");
    drawSlots(slotSelect);
  }

  void drawMain(int selectedPad)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(66, 5);
    display.print("Degree: ");
    display.print(pads[selectedPad].chord.degree + 1);
    drawChordNotes(selectedPad);
    drawNoteVelocities(selectedPad);
  }

  void drawRootMenu()
  {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(37, 28);
    display.print("Root: ");
    display.print(midiNoteNames[settings.rootNote]);
  }

  void drawScaleMenu()
  {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(18, 28);
    display.print("Scale: ");
    display.print(scaleNames[settings.scaleIndex]);
  }

  void drawDegreeMenu(int selectedPad)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(66, 5);
    display.print("Degree: ");
    display.print(pads[selectedPad].chord.degree + 1);
    display.drawLine(66, 14, 120, 14, WHITE);
    drawChordNotes(selectedPad);
    drawNoteVelocities(selectedPad);
  }

  void drawNoteMenu(int selectedPad, int noteIndex)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(62, 5);
    display.print("Edit Notes");
    drawChordNotes(selectedPad);
    drawNoteVelocities(selectedPad);
    drawNoteSelect(noteIndex);
  }

  void drawVariationMenu(int selectedPad)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(4, 28);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("Velocity Random: ");
    display.print(pads[selectedPad].velocityVariation);
  }

  void drawVelocityMenu(int selectedPad, int noteIndex, int menuIndex)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(50, 5);
    display.print("Edit Velocity");
    drawChordNotes(selectedPad);
    drawNoteVelocities(selectedPad);
    drawVelocitySelect(noteIndex, menuIndex);
  }

  void drawModsMenu(int selectedPad, int noteIndex, int menuIndex)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(44, 5);
    display.print("Edit Intervals");
    drawChordNotes(selectedPad);
    drawModSelect(noteIndex, menuIndex);
  }

  void drawOctaveMenu(int selectedPad, int noteIndex, int menuIndex)
  {
    drawSelectedPad(selectedPad);
    display.setCursor(50, 5);
    display.print("Edit Octaves");
    drawChordNotes(selectedPad);
    drawNoteOctaves(selectedPad, noteIndex, menuIndex);
    drawNoteVelocities(selectedPad);
    drawVelocitySelect(noteIndex, menuIndex);
  }

  void drawCopyMenu(int selectedPad)
  {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(31, 20);
    display.print("Copy pad ");
    display.print(selectedPad + 1);
    display.setCursor(10, 40);
    display.print("Select destination");
  }

  void drawMidiMenu(int menuIndex)
  {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(5, 0);
    display.print("MIDI In Rec:  ");
    display.println(settings.midiRecChannel + 1);
    display.setCursor(5, 11);
    display.print("MIDI In Trig: ");
    display.println(settings.midiTrigChannel + 1);
    display.setCursor(5, 22);
    display.print("MIDI Out A:   ");
    display.println(settings.midiOutputAChannel + 1);
    display.setCursor(5, 33);
    display.print("MIDI Out B:   ");
    display.println(settings.midiOutputBChannel + 1);
    display.setCursor(5, 44);
    display.print("MIDI Out C:   ");
    display.println(settings.midiOutputCChannel + 1);
    display.setCursor(5, 55);
    display.print("MIDI Out D:   ");
    display.println(settings.midiOutputDChannel + 1);

    switch (menuIndex)
    {
    case 0:
      display.drawLine(5, 8, 124, 8, WHITE);
      break;
    case 1:
      display.drawLine(5, 19, 124, 19, WHITE);
      break;
    case 2:
      display.drawLine(5, 30, 124, 30, WHITE);
      break;
    case 3:
      display.drawLine(5, 41, 124, 41, WHITE);
      break;
    case 4:
      display.drawLine(5, 52, 124, 52, WHITE);
      break;
    case 5:
      display.drawLine(5, 63, 124, 63, WHITE);
      break;
    }
  }

  void drawSaveScreen(int slotSelect)
  {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(40, 6);
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
      drawMain(selectedPad);
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
      drawModsMenu(selectedPad, noteIndex, menuIndex);
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
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.print("Copied ");
    display.print(source + 1);
    display.print(" to ");
    display.print(target + 1);
    display.display();
    delay(1000);
  }
}