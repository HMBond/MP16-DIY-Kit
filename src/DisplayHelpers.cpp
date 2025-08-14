#include "Hardware.h"
#include "Stored.h"
#include "DisplayHelpers.h"
#include "MusicTheory.h"

using namespace MusicTheory;
using namespace Hardware;
using namespace Stored;

void drawVelocitySelect(int noteIndex, int menuIndex)
{
  for (int i = 0; i < 8; i++)
  {
    int x = (i % 4) * 32;
    int y = 0;
    if (i < 4)
    {
      y = 20;
    }
    else
    {
      y = 42;
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

void drawSlots(int slotSelect)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  for (int i = 0; i < 4; i++)
  {
    display.setCursor(44, 17 + 11 * i);
    display.print("Slot ");
    display.print(i + 1);
    if (i == slotSelect)
    {
      display.drawRect(42, 15 + 11 * i, 40, 11, WHITE);
    }
  }
}

void drawSelectedPad(int selectedPad)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(2, 5);
  display.print("Pad ");
  display.print(selectedPad + 1);
}

void drawChordNotes(int selectedPad)
{
  for (int i = 0; i < 8; i++)
  {
    int x = (i % 4) * 32;
    int y = 0;
    if (i < 4)
    {
      y = 20;
    }
    else
    {
      y = 42;
    }

    display.fillRect(x, y, 13, 17, WHITE);
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

    display.setCursor(x + 1, y + 9);
    if (pads[selectedPad].chord.semitoneModifiers[i] != 0)
    {
      if (pads[selectedPad].chord.semitoneModifiers[i] < 0)
      {
        display.print(pads[selectedPad].chord.semitoneModifiers[i]);
      }
      else
      {
        display.print("+");
        display.print(pads[selectedPad].chord.semitoneModifiers[i]);
      }
    }

    display.setTextColor(WHITE);
    if (pads[selectedPad].chord.isActive[i])
    {
      display.setCursor(x + 14, y + 1);
      display.print(midiNoteNames[settings.rootNote + pads[selectedPad].chord.intervals[i] + (pads[selectedPad].chord.octaveModifiers[i] * 12) + pads[selectedPad].chord.semitoneModifiers[i]]);
    }
    else
    {
      display.setCursor(x + 14, y + 1);
      display.print("-");
    }
  }
}

void drawNoteVelocities(int selectedPad)
{
  for (int i = 0; i < 8; i++)
  {
    int x = (i % 4) * 32;
    int y = 0;
    if (i < 4)
    {
      y = 20;
    }
    else
    {
      y = 42;
    }
    if (pads[selectedPad].chord.isActive[i])
    {
      display.setCursor(x + 14, y + 9);
      display.print((int)constrain(settings.velocityScaling * (pads[selectedPad].padVelocity + pads[selectedPad].chord.velocityModifiers[i]), 1, 128));
    }
  }
}

void drawNoteOctaves(int selectedPad, int noteIndex, int menuIndex)
{
  for (int i = 0; i < 8; i++)
  {
    int x = (i % 4) * 32;
    int y = 0;
    if (i < 4)
    {
      y = 20;
    }
    else
    {
      y = 42;
    }

    display.setCursor(x + 14, y + 9);
    if (pads[selectedPad].chord.octaveModifiers[i] < 1)
    {
      display.print(pads[selectedPad].chord.octaveModifiers[i]);
    }
    else
    {
      display.print("+");
      display.print(pads[selectedPad].chord.octaveModifiers[i]);
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

void drawNoteSelect(int noteIndex)
{
  for (int i = 0; i < 8; i++)
  {
    int x = (i % 4) * 32;
    int y = 0;
    if (i < 4)
    {
      y = 20;
    }
    else
    {
      y = 42;
    }
    if (i == noteIndex)
    {
      display.drawLine(x, y - 2, x + 31, y - 2, WHITE);
      display.drawLine(x, y + 18, x + 31, y + 18, WHITE);
    }
  }
}

void drawModSelect(int noteIndex, int menuIndex)
{
  for (int i = 0; i < 8; i++)
  {
    int x = (i % 4) * 32;
    int y = 0;
    if (i < 4)
    {
      y = 20;
    }
    else
    {
      y = 42;
    }
    if (i == noteIndex)
    {
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