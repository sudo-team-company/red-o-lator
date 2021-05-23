#include "MemoryView.h"
#include "../Events.h"
#include "ColumnLabels.h"
#include "HexScrollWindow.h"
#include "hex-utils.h"

MemoryView::MemoryView(wxWindow* parent)
    : wxWindow(parent,
               wxID_ANY,
               wxDefaultPosition,
               wxSize(500, 250),
               wxBORDER_STATIC) {
    wxTextValidator addressValidator(wxFILTER_INCLUDE_CHAR_LIST);
    addressValidator.SetCharIncludes("0123456789abcdef");

    addressInput =
        new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                       wxDefaultSize, wxBORDER_NONE, addressValidator);
    addressInput->SetMaxLength(9);
    addressInput->SetFont(hexViewFont);

    addressButton = new wxButton(this, REQUEST_MEMORY, _("Load"));

    auto offsetText = new wxStaticText(this, wxID_ANY, _("   Offset   "));
    offsetText->SetFont(hexViewFontBold);

    auto addressText = new wxStaticText(this, wxID_ANY, _("Address:"));
    addressText->SetFont(hexViewFontBold);

    auto* columns = new ColumnLabels(this);

    rows = new AddressRows(this);
    hexView = new HexScrollWindow(this, rows, columns);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    auto topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->AddStretchSpacer(1);
    topSizer->Add(addressText, 0, wxALIGN_CENTER_VERTICAL);
    topSizer->AddSpacer(10);
    topSizer->Add(addressInput, 0, wxALIGN_CENTER_VERTICAL);
    topSizer->AddSpacer(10);
    topSizer->Add(addressButton, 0, wxALIGN_CENTER_VERTICAL);
    topSizer->AddStretchSpacer(1);

    auto bottomSizer = new wxFlexGridSizer(2, 2, 0, 0);

    bottomSizer->Add(offsetText, 1, wxEXPAND, 0);
    bottomSizer->Add(columns, 1, wxEXPAND, 0);
    bottomSizer->Add(rows, 1, wxEXPAND, 0);
    bottomSizer->Add(hexView, 1, wxEXPAND, 0);
    bottomSizer->AddGrowableCol(1);
    bottomSizer->AddGrowableRow(1);

    mainSizer->AddSpacer(5);
    mainSizer->Add(topSizer, 0, wxEXPAND, 0);
    mainSizer->AddSpacer(5);
    mainSizer->Add(bottomSizer, 1, wxEXPAND, 0);

    SetSizer(mainSizer);
}

void MemoryView::setMemoryView(const void* memory,
                               uint64_t size,
                               uint64_t address) {
    auto startAddress = getStartAddress(address);
    auto endAddress = getEndAddress(address, size);
    auto lineCount = getLineCount(startAddress, endAddress);
    rows->setState(address, static_cast<int>(lineCount));
    hexView->setHex(memory, size, address);
    rows->Refresh();
    hexView->Refresh();
}

wxFont MemoryView::hexViewFontBold =
    wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

wxFont MemoryView::hexViewFont =
    wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
