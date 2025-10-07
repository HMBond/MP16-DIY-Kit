#ifndef DisplayHelpers_h
#define DisplayHelpers_h

#include <Arduino.h>

void drawFromRight(const char *str, int x, int y);
void drawFromRight(int value, int x, int y);
void drawCentered(const char *str, int y);
void drawCentered(String &str, int y);
void drawSlots(int selectedSlot);
void drawSelectedPad(int selectedPad);
void drawChordNotes(int pad, bool showAllNoteOffsets = false, bool hideAllNoteOffsets = false);
void drawNoteVelocities(int selectedPad);
void drawNoteVelocityModifiers(int selectedPad);
void drawNoteOctaves(int selectedPad, int noteIndex, int menuIndex);
void drawNoteChannels(int selectedPad, int noteIndex, int menuIndex);
void drawNoteBlockSelector(int noteIndex);
void drawNoteOffsetSelector(int noteIndex, int menuIndex);
void drawRightBottomCornerSelector(int noteIndex, int menuIndex);

#endif