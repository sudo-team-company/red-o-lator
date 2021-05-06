#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title,
                     const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size) {
    Center();

    SetIcon(wxICON(icon));

    auto* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    menuFile->Append(wxID_EXIT);
    auto* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    auto* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    auto *panel = new wxPanel(this, wxID_ANY);
    auto *button = new wxButton(panel, ID_Hello, wxT("button"),
                                    wxPoint(20, 20));
    Connect(ID_Hello, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MainFrame::OnHello));
    button->SetFocus();


    wxFrameBase::SetMenuBar(menuBar);
    wxFrameBase::CreateStatusBar();
    wxFrameBase::SetStatusText("");
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets' Hello world sample", "About Hello World",
                 wxOK | wxICON_INFORMATION);
}

void MainFrame::OnHello(wxCommandEvent& event) {
    wxLogMessage("Hello world from wxWidgets!");
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
        EVT_MENU(ID_Hello, MainFrame::OnHello)
        EVT_MENU(wxID_EXIT, MainFrame::OnExit)
        EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()
