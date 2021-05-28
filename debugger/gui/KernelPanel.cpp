#include "KernelPanel.h"
#include "Events.h"

KernelPanel::KernelPanel(wxWindow* parent)
    : wxListBox(parent, SELECT_KERNEL, wxDefaultPosition, wxSize(200, 150)) {
    wxFont font(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    SetFont(font);
}
