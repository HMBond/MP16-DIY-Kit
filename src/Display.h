#ifndef Display_h
#define Display_h

namespace Display
{
  void updateDisplay(int screenIndex,
                     int selectedPad,
                     int menuIndex,
                     int selectedSlot,
                     int noteIndex,
                     bool editMenuItem);
  void showMessage(String message, int duration = 2000);
}

#endif