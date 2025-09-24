#ifndef Display_h
#define Display_h

namespace Display
{
  void updateDisplay(int screenIndex, int selectedPad, int menuIndex, int slotSelect, int noteIndex, bool editMenuItem);
  void showFileSystemFailure();
  void showCopyMessage(int source, int target);
}

#endif