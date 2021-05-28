#include "AddressRows.h"
#include "MemoryView.h"
#include "hex-utils.h"

AddressRows::AddressRows(wxWindow* parent)
    : wxScrolledWindow(parent, wxID_ANY) {
    wxClientDC dc(this);

    dc.SetFont(MemoryView::hexViewFontBold);
    dc.GetTextExtent(" 000000000h", &lineWidth, &lineHeight);

    SetScrollRate(0, lineHeight);
    ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);
    setState(0, 0);

    SetDoubleBuffered(true);

    Bind(wxEVT_MOUSEWHEEL,
         [](wxEvent& e) {});  // scrolling is managed by hex view
}

void AddressRows::OnDraw(wxDC& dc) {
    dc.SetFont(MemoryView::hexViewFontBold);
    wxRect rectUpdate = GetUpdateRegion().GetBox();
    CalcUnscrolledPosition(rectUpdate.x, rectUpdate.y, &rectUpdate.x,
                           &rectUpdate.y);

    size_t lineFrom = rectUpdate.y / lineHeight,
           lineTo = rectUpdate.GetBottom() / lineHeight;

    if (lineTo > lineCount - 1) lineTo = lineCount - 1;
    int y = static_cast<int>(lineFrom) * lineHeight;
    int yPhys;

    for (size_t line = lineFrom; line <= lineTo; line++) {
        CalcScrolledPosition(0, y, nullptr, &yPhys);
        dc.DrawText(
            wxString::Format(" %.9llxh", getLineAddress(startAddress, line)), 0,
            y);
        y += lineHeight;
    }
}

void AddressRows::setState(uint64_t address, size_t lineCount) {
    this->lineCount = lineCount;
    this->startAddress = getStartAddress(address);
    SetVirtualSize(lineWidth, static_cast<int>(lineCount) * lineHeight);
}
