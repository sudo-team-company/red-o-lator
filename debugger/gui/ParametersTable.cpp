#include "ParametersTable.h"

ParametersTable::ParametersTable(wxWindow* parent)
    : wxPropertyGrid(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE) {
    SetExtraStyle(wxPG_EX_HELP_AS_TOOLTIPS);

    Append(new wxPropertyCategory("Global Parameters"));
    Append(new wxStringProperty("amdcl2"));
    Append(new wxStringProperty("gpu", wxPG_LABEL, "Iceland"));
    Append(new wxIntProperty("64bit"));;

    Append(new wxPropertyCategory("Kernel Parameters"));
    Append(new wxStringProperty("dims", wxPG_LABEL, "xy"));
    Append(new wxArrayStringProperty("cws", wxPG_LABEL, wxArrayString()));
    Append(new wxIntProperty("sgprsnum", wxPG_LABEL, 16));

}
