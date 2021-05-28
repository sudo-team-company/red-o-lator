#ifndef RED_O_LATOR_TOOLBAR_H
#define RED_O_LATOR_TOOLBAR_H

#include <wx/aui/aui.h>
#include "wx.h"

class Toolbar : public wxAuiToolBar {
   private:
    const wxSize toolbarElementSize = wxSize(24, 24);
    template <size_t N>
    wxImage loadToolbarIcon(const unsigned char (&data)[N]);

   public:
    Toolbar(wxWindow* parent);

    wxChoice* choiceModel;
};

#endif  // RED_O_LATOR_TOOLBAR_H
