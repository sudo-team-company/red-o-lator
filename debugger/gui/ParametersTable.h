#ifndef RED_O_LATOR_PARAMETERSTABLE_H
#define RED_O_LATOR_PARAMETERSTABLE_H

#include <wx/propgrid/propgrid.h>

class ParametersTable : public wxPropertyGrid {
   public:
    explicit ParametersTable(wxWindow* parent);

    wxPGProperty* globalCategory;
    wxPGProperty* kernelCategory;
};

#endif  // RED_O_LATOR_PARAMETERSTABLE_H
