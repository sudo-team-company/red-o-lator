#include "HexScrollWindow.h"
#include "hex-utils.h"

HexScrollWindow::HexScrollWindow(wxWindow* parent,
                                 AddressRows* rows,
                                 ColumnLabels* columns)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(500, 250)) {
    this->rows = rows;
    this->columns = columns;

    wxClientDC dc(this);
    dc.SetFont(MemoryView::hexViewFontBold);

    dc.GetTextExtent(
        " FF FF FF FF FF FF FF FF  FF FF FF FF FF FF FF FF   ................",
        &lineWidth, &lineHeight);

    SetScrollRate(1, lineHeight);
    ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_NEVER);

    SetBackgroundColour("WHITE");
    SetDoubleBuffered(true);
}

void HexScrollWindow::OnDraw(wxDC& dc) {
    dc.SetFont(MemoryView::hexViewFontBold);
    dc.SetTextForeground("DARK SLATE GREY");
    wxRect rectUpdate = GetUpdateRegion().GetBox();
    CalcUnscrolledPosition(rectUpdate.x, rectUpdate.y, &rectUpdate.x,
                           &rectUpdate.y);

    size_t lineFrom = rectUpdate.y / lineHeight,
           lineTo = rectUpdate.GetBottom() / lineHeight;

    if (lineTo > lineCount - 1) lineTo = lineCount - 1;

    int y = static_cast<int>(lineFrom) * lineHeight;

    for (size_t line = lineFrom; line <= lineTo; line++) {
        dc.DrawText(
            drawLine(line * 16 - (originalAddress - startAddress),
                     getLineAddress(startAddress, line)),
            0, y);
        y += lineHeight;
    }
}

void HexScrollWindow::ScrollWindow(int dx, int dy, const wxRect* rect) {
    wxScrolledWindow::ScrollWindow(dx, dy, rect);
    rows->Scroll(GetViewStart());
    columns->Scroll(GetViewStart());
}

void HexScrollWindow::setHex(const std::vector<uint8_t>& memory, uint64_t address) {
    startAddress = getStartAddress(address);
    endAddress = getEndAddress(address, memory.size());
    originalAddress = address;
    this->memory = memory;
    this->memorySize = memory.size();
    lineCount = getLineCount(startAddress, endAddress);

    SetVirtualSize(lineWidth, static_cast<int>(lineCount) * lineHeight);
}

wxString HexScrollWindow::drawLine(size_t start, uint64_t lineAddr) const {
    auto hexArray = wxString();
    auto charArray = wxString();
    for (int i = 0; i < 16; i++) {
        auto curAddr = lineAddr + i;
        if (curAddr < originalAddress ||
            curAddr >= originalAddress + memorySize) {
            hexArray.Append(" ..");
            charArray.Append('.');
        } else {
            hexArray.Append(wxString::Format(" %.2X", memory[start + i]));
            charArray.Append(memory[start + i]);
        }
        if (i == 7) {
            hexArray.Append(' ');
        }
    }

    return wxString::Format("%s   %s", hexArray, charArray);
}