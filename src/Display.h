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
  void showFileSystemFailure();
  void showCopyMessage(int source, int target);
}

#endif