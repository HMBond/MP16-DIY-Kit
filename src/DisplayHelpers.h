#ifndef DisplayHelpers_h
#define DisplayHelpers_h

#include <Arduino.h>

void displayRight(int value, int x, int y);
void displayCentered(const char *str, int y);
void displayCentered(String &str, int y);
void drawSlots(int slotSelect);
void drawSelectedPad(int selectedPad);
void drawChordNotes(int pad, bool showAllNoteOffsets = false);
void drawNoteVelocities(int selectedPad);
void drawNoteVelocityModifiers(int selectedPad);
void drawNoteOctaves(int selectedPad, int noteIndex, int menuIndex);
void drawNoteBlockSelector(int noteIndex);
void drawNoteOffsetSelector(int noteIndex, int menuIndex);
void drawOctaveSelector(int noteIndex, int menuIndex);

#endif