#ifndef DisplayHelpers_h
#define DisplayHelpers_h

void drawVelocitySelect(int noteIndex, int menuIndex);
void drawSlots(int slotSelect);
void drawSelectedPad(int selectedPad);
void drawChordNotes(int pad);
void drawNoteVelocities(int selectedPad);
void drawNoteOctaves(int selectedPad, int noteIndex, int menuIndex);
void drawNoteSelect(int noteIndex);
void drawModSelect(int noteIndex, int menuIndex);

#endif