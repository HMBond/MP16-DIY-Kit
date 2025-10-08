#ifndef Display_h
#define Display_h

#include <Arduino.h>

void drawLoadScreen(int selectedSlot);
void drawMainMenu(int selectedPad);
void drawRootMenu();
void drawScaleMenu();
void drawDegreeMenu(int selectedPad);
void drawNoteActiveMenu(int selectedPad, int noteIndex);
void drawRandomVelocityMenu(int selectedPad);
void drawNoteVelocityMenu(int selectedPad, int noteIndex, int menuIndex);
void drawNoteOffsetMenu(int selectedPad, int noteIndex, int menuIndex);
void drawNoteOctaveMenu(int selectedPad, int noteIndex, int menuIndex);
void drawNoteChannelMenu(int selectedPad, int noteIndex, int menuIndex);
void drawCopyMenu(int selectedPad);
void drawMidiMenu(int menuIndex, bool editMenuItem);
void drawSaveMenu(int selectedSlot);
void drawMessage(String message, int duration = 2000);

#endif