#include "ColumnLabels.h"
#include "MemoryView.h"

ColumnLabels::ColumnLabels(wxWindow* parent)
    : wxScrolledWindow(parent, wxID_ANY) {
    wxClientDC dc(this);

    int lineWidth, lineHeight;
    dc.SetFont(MemoryView::hexViewFontBold);
    dc.GetTextExtent(columns, &lineWidth, &lineHeight);

    SetVirtualSize(lineWidth, lineHeight);
    SetScrollRate(1, 0);
    ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);
}

void ColumnLabels::OnDraw(wxDC& dc) {
    dc.SetFont(MemoryView::hexViewFontBold);
    dc.DrawText(columns, 0, 0);
}

wxString ColumnLabels::columns =
    "  0  1  2  3  4  5  6  7   8  9  a  b  c  d  e  f                   ";