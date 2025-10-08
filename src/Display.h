#ifndef Display_h
#define Display_h

namespace Display
{
  void drawLoadScreen(int selectedSlot);
  void drawMainMenu(int selectedPad);
  void drawRootMenu();
  void drawScaleMenu();
  void drawDegreeMenu(int selectedPad);
  void drawNoteMenu(int selectedPad, int noteIndex);
  void drawVariationMenu(int selectedPad);
  void drawVelocityMenu(int selectedPad, int noteIndex, int menuIndex);
  void drawNoteOffsetMenu(int selectedPad, int noteIndex, int menuIndex);
  void drawOctaveMenu(int selectedPad, int noteIndex, int menuIndex);
  void drawChannelMenu(int selectedPad, int noteIndex, int menuIndex);
  void drawCopyMenu(int selectedPad);
  void drawMidiMenu(int menuIndex, bool editMenuItem);
  void drawSaveMenu(int selectedSlot);
  void showMessage(String message, int duration = 2000);
}

#endif