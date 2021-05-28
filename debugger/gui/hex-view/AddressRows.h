#ifndef RED_O_LATOR_ADDRESSROWS_H
#define RED_O_LATOR_ADDRESSROWS_H

#include "../wx.h"
#include <cstddef>
#include <cstdint>

class AddressRows : public wxScrolledWindow {
   public:
    explicit AddressRows(wxWindow* parent);

    void setState(uint64_t address, size_t lineCount);

   private:
    size_t lineCount = 0;
    int lineHeight = 0;
    int lineWidth = 0;

    uint64_t startAddress = 0;

    void OnDraw(wxDC& dc) override;
};

#endif  // RED_O_LATOR_ADDRESSROWS_H
