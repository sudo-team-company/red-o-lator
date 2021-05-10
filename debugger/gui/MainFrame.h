#ifndef RED_O_LATOR_MAINFRAME_H
#define RED_O_LATOR_MAINFRAME_H

#include <wx/aui/aui.h>
#include "CodeViewer.h"
#include "MenuBar.h"
#include "Preferences.h"
#include "wx.h"

class MainFrame : public wxFrame {
   private:
    wxAuiManager auiManager;
    Preferences& preferences;

    MenuBar* menuBar;
    CodeViewer* textEditor;
    wxString fileName;

    void onOpen(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
    void onToggleAddressMargin(wxCommandEvent& event);
    void onKernelSelected(wxCommandEvent& event);

    void bindEvents();

   public:
    MainFrame(const wxString& title,
              const wxSize& size,
              Preferences& preferences);

};

#endif  // RED_O_LATOR_MAINFRAME_H
