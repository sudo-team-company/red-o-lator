#ifndef RED_O_LATOR_EMULATORAPP_H
#define RED_O_LATOR_EMULATORAPP_H

#include "Preferences.h"
#include "wx/wx.h"

class EmulatorApp : public wxApp {
   private:
    Preferences preferences;
   public:
    virtual bool OnInit();
};

#endif  // RED_O_LATOR_EMULATORAPP_H
