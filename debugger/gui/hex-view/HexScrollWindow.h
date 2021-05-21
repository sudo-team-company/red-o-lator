#ifndef RED_O_LATOR_HEXSCROLLWINDOW_H
#define RED_O_LATOR_HEXSCROLLWINDOW_H

#include "../wx.h"
#include "AddressRows.h"
#include "ColumnLabels.h"
#include "MemoryView.h"

class HexScrollWindow : public wxScrolledWindow {
   public:
    HexScrollWindow(wxWindow* parent, AddressRows* rows, ColumnLabels* columns);

    void ScrollWindow(int dx, int dy, const wxRect* rect) override;

    void setHex(const void* memory, size_t startAddress, size_t endAddress);

   private:
    int lineHeight;
    int lineWidth;
    size_t lineCount;
    size_t startAddress;
    size_t endAddress;
    size_t originalAddress;
    const uint8_t* memory;
    size_t memorySize;

    AddressRows* rows;
    ColumnLabels* columns;

    void OnDraw(wxDC& dc) override;

    wxString drawLine(const uint8_t* mem, size_t lineAddr) const;
};

#endif  // RED_O_LATOR_HEXSCROLLWINDOW_H
