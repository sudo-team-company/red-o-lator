#ifndef RED_O_LATOR_ADDRESSROWS_H
#define RED_O_LATOR_ADDRESSROWS_H

#include "../wx.h"

class AddressRows : public wxScrolledWindow {
   public:
    explicit AddressRows(wxWindow* parent);

    void setState(size_t address, size_t lineCount);

   private:
    size_t lineCount = 0;
    int lineHeight = 0;
    int lineWidth = 0;

    size_t startAddress = 0;

    void OnDraw(wxDC& dc) override;
};

#endif  // RED_O_LATOR_ADDRESSROWS_H
