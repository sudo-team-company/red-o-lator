#ifndef RED_O_LATOR_HEXSCROLLWINDOW_H
#define RED_O_LATOR_HEXSCROLLWINDOW_H

#include <cstdint>
#include "../wx.h"
#include "AddressRows.h"
#include "ColumnLabels.h"
#include "MemoryView.h"

class HexScrollWindow : public wxScrolledWindow {
   public:
    HexScrollWindow(wxWindow* parent, AddressRows* rows, ColumnLabels* columns);

    void ScrollWindow(int dx, int dy, const wxRect* rect) override;

    void setHex(const void* memory, uint64_t size, uint64_t address);

   private:
    int lineHeight;
    int lineWidth;
    size_t lineCount;
    uint64_t startAddress;
    uint64_t endAddress;
    uint64_t originalAddress;
    const uint8_t* memory;
    uint64_t memorySize;

    AddressRows* rows;
    ColumnLabels* columns;

    void OnDraw(wxDC& dc) override;

    wxString drawLine(const uint8_t* mem, uint64_t lineAddr) const;
};

#endif  // RED_O_LATOR_HEXSCROLLWINDOW_H
