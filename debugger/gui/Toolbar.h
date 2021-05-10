#ifndef RED_O_LATOR_TOOLBAR_H
#define RED_O_LATOR_TOOLBAR_H

#include "wx.h"
#include <wx/aui/aui.h>


class Toolbar : public wxAuiToolBar {
   private:
    const wxSize toolbarElementSize = wxSize(24, 24);
    wxImage loadPNG(const wxString& resourceName);

   public:
    Toolbar(wxWindow* parent);
};

#endif  // RED_O_LATOR_TOOLBAR_H
