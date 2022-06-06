#include "ParametersTable.h"

ParametersTable::ParametersTable(wxWindow* parent)
    : wxPropertyGrid(parent,
                     wxID_ANY,
                     wxDefaultPosition,
                     wxSize(200, 250),
                     wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE) {
    SetExtraStyle(wxPG_EX_HELP_AS_TOOLTIPS);

    globalCategory = Append(new wxPropertyCategory("Global Parameters"));
    globalCategory->Hide(true);
    kernelCategory = Append(new wxPropertyCategory("Kernel Parameters"));
}
