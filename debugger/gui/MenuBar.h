#ifndef RED_O_LATOR_MENUBAR_H
#define RED_O_LATOR_MENUBAR_H

#include "../data/Preferences.h"
#include "wx.h"

class MenuBar : public wxMenuBar {
   private:
    wxMenuItem* addressMarginSwitch;
    void setPreferredState(const Preferences& preferences);

   public:
    MenuBar(const Preferences& preferences);
};

#endif  // RED_O_LATOR_MENUBAR_H
