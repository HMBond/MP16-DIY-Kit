#include <Arduino.h>
#include "Hardware.h"
#include "Stored.h"
#include "DisplayHelpers.h"
#include "MusicTheory.h"

using namespace MusicTheory;
using namespace Hardware;
using namespace Stored;

void drawFromRight(int value, int x, int y)
{
  char buffer[4];
  itoa(value, buffer, 10);
  int16_t x1, y1;
  uint16_t width, height;
  display.getTextBounds(buffer, 0, 0, &x1, &y1, &width, &height); // calc width of new string
  display.setCursor(x - width, y);
  display.print(buffer);
}

void drawCentered(const char *str, int y)
{
  int16_t x1, y1;
  uint16_t width, height;
  display.getTextBounds(str, 0, 0, &x1, &y1, &width, &height); // calc width of new string
  display.setCursor(64 - width / 2, y);
  display.print(str);
}

void drawCentered(String &str, int y)
{
  if (str.length() != 0)
  {
    drawCentered(const_cast<char *>(str.c_str()), y);
  }
}

int getNoteBlockX(int i)
{
  return (i % 4) * 32;
}

int getNoteBlockY(int i)
{
  return i < 4 ? 16 : 38;
}

void drawSlots(int slotSelect)
{
  for (int i = 0; i < 4; i++)
  {
    display.setCursor(44, 12 + 11 * i);
    display.print("Slot ");
    display.print(i + 1);
    if (i == slotSelect)
    {
      display.drawRect(42, 10 + 11 * i, 40, 11, WHITE);
    }
  }
}

void drawSelectedPad(int selectedPad)
{
  display.setCursor(2, 0);
  display.print("Pad ");
  display.print(selectedPad + 1);
}

void drawNoteIdentifier(int x, int y, int i)
{
  display.setTextColor(BLACK);
  switch (i)
  {
  case 0:
    display.setCursor(x + 4, y + 1);
    display.print("1");
    break;
  case 1:
    display.setCursor(x + 4, y + 1);
    display.print("3");
    break;
  case 2:
    display.setCursor(x + 4, y + 1);
    display.print("5");
    break;
  case 3:
    display.setCursor(x + 4, y + 1);
    display.print("7");
    break;
  case 4:
    display.setCursor(x + 4, y + 1);
    display.print("9");
    break;
  case 5:
    display.setCursor(x + 1, y + 1);
    display.print("11");
    break;
  case 6:
    display.setCursor(x + 1, y + 1);
    display.print("13");
    break;
  case 7:
    display.setCursor(x + 4, y + 1);
    display.print("B");
    break;
  }
  display.setTextColor(WHITE);
}

void drawNoteOffsetValue(int x, int y, int semitoneModifier, int i)
{
  display.setTextColor(BLACK);
  display.setCursor(x + 1, y + 9);

  if (semitoneModifier < 0)
  {
    display.print(semitoneModifier);
  }
  else
  {
    display.print("+");
    display.print(semitoneModifier);
  }

  display.setTextColor(WHITE);
}

void drawNote(int x, int y, int i, int selectedPad, int semitoneModifier)
{
  if (pads[selectedPad].chord.isActive[i])
  {
    String note = midiNoteNames[settings.rootNote + pads[selectedPad].chord.intervals[i] + (pads[selectedPad].chord.octaveModifiers[i] * 12) + semitoneModifier];
    display.setCursor(x + 32 - note.length() * 6, y + 1);
    display.print(note);
  }
}

void drawChordNotes(int selectedPad, bool showAllNoteOffsets)
{
  for (int i = 0; i < 8; i++)
  {
    int x = getNoteBlockX(i);
    int y = getNoteBlockY(i);
    int semitoneModifier = pads[selectedPad].chord.semitoneModifiers[i];

    display.fillRect(x, y, 13, 17, WHITE);
    drawNoteIdentifier(x, y, i);
    if (semitoneModifier != 0 || showAllNoteOffsets)
    {
      drawNoteOffsetValue(x, y, semitoneModifier, i);
    }
    drawNote(x, y, i, selectedPad, semitoneModifier);
  }
}

void drawNoteVelocities(int selectedPad)
{
  for (int i = 0; i < 8; i++)
  {
    if (pads[selectedPad].chord.isActive[i])
    {
      int x = getNoteBlockX(i);
      int y = getNoteBlockY(i);

      int velocity = settings.velocityScaling * (pads[selectedPad].padVelocity + pads[selectedPad].chord.velocityModifiers[i]);
      display.setCursor(x + 32 - String(velocity).length() * 6, y + 9);
      display.print(constrain(velocity, 1, 128));
    }
  }
}

void drawNoteVelocityModifiers(int selectedPad)
{
  for (int i = 0; i < 8; i++)
  {
    if (pads[selectedPad].chord.isActive[i])
    {
      int x = getNoteBlockX(i);
      int y = getNoteBlockY(i);

      int velocity = pads[selectedPad].chord.velocityModifiers[i];
      drawFromRight(velocity, x + 32, y + 9);
    }
  }
}

void drawNoteOctaves(int selectedPad, int noteIndex, int menuIndex)
{
  for (int i = 0; i < 8; i++)
  {
    int x = getNoteBlockX(i);
    int y = getNoteBlockY(i);

    int octave = pads[selectedPad].chord.octaveModifiers[i];
    int noSignCorrection = octave == 0 ? 6 : 0;
    display.setCursor(x + 20 + noSignCorrection, y + 9);

    if (octave < 1)
    {
      display.print(octave);
    }
    else
    {
      display.print("+");
      display.print(octave);
    }

    if (i == noteIndex)
    {
      display.drawLine(x, y - 2, x + 31, y - 2, WHITE);
      if (menuIndex - 1 == i)
      {
        display.drawLine(x + 13, y + 18, x + 31, y + 18, WHITE);
        display.drawLine(x, y + 20, x + 31, y + 20, WHITE);
      }
      else
      {
        display.drawLine(x, y + 18, x + 31, y + 18, WHITE);
      }
    }
  }
}

void drawNoteBlockSelector(int noteIndex)
{
  for (int i = 0; i < 8; i++)
  {
    int x = getNoteBlockX(i);
    int y = getNoteBlockY(i);

    if (i == noteIndex)
    {
      display.fillRect(x, y, 13, 17, INVERSE);
      display.drawLine(x, y - 2, x + 31, y - 2, WHITE);
      display.drawLine(x, y + 18, x + 31, y + 18, WHITE);
    }
  }
}

void drawNoteOffsetSelector(int noteIndex, int menuIndex)
{
  for (int i = 0; i < 8; i++)
  {
    int x = getNoteBlockX(i);
    int y = getNoteBlockY(i);

    if (i == noteIndex)
    {
      display.fillRect(x, y, 13, 17, INVERSE);
      display.drawLine(x, y - 2, x + 31, y - 2, WHITE);
      if (menuIndex - 1 == i)
      {
        display.drawLine(x, y + 18, x + 12, y + 18, WHITE);
        display.drawLine(x, y + 20, x + 31, y + 20, WHITE);
      }
      else
      {
        display.drawLine(x, y + 18, x + 31, y + 18, WHITE);
      }
    }
  }
}

void drawOctaveSelector(int noteIndex, int menuIndex)
{
  for (int i = 0; i < 8; i++)
  {
    int x = getNoteBlockX(i);
    int y = getNoteBlockY(i);

    if (i == noteIndex)
    {
      display.fillRect(x, y, 13, 17, INVERSE);
      display.drawLine(x, y - 2, x + 31, y - 2, WHITE);
      if (menuIndex - 1 == i)
      {
        display.drawLine(x + 13, y + 18, x + 31, y + 18, WHITE);
        display.drawLine(x, y + 20, x + 31, y + 20, WHITE);
      }
      else
      {
        display.drawLine(x, y + 18, x + 31, y + 18, WHITE);
      }
    }
  }
}