#ifndef RED_O_LATOR_MAINFRAME_H
#define RED_O_LATOR_MAINFRAME_H

#include "wx.h"

class MainFrame : public wxFrame {
   public:
    MainFrame(const wxString& title, const wxSize& size);

   private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
   wxDECLARE_EVENT_TABLE();
};

enum { ID_Hello = 1 };

#endif  // RED_O_LATOR_MAINFRAME_H
