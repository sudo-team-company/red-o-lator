#ifndef RED_O_LATOR_MEMORYVIEW_H
#define RED_O_LATOR_MEMORYVIEW_H

#include <cstdint>
#include <iomanip>
#include <sstream>
#include "../wx.h"
#include "AddressRows.h"

class AddressRows;
class HexScrollWindow;

class MemoryView : public wxWindow {
   public:
    explicit MemoryView(wxWindow* parent);

    void setMemoryView(const void* memory, uint64_t size, uint64_t address);

    wxTextCtrl* addressInput;
    wxButton* addressButton;

    static wxFont hexViewFont;
    static wxFont hexViewFontBold;

   private:
    AddressRows* rows;
    HexScrollWindow* hexView;
};

#endif  // RED_O_LATOR_MEMORYVIEW_H
