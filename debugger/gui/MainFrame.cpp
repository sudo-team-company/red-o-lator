#include "MainFrame.h"
#include <wx/grid.h>
#include <wx/propgrid/propgrid.h>
#include "Events.h"
#include "ParametersTable.h"
#include "Toolbar.h"

MainFrame::MainFrame(const wxString& title,
                     const wxSize& size,
                     Preferences& preferences)
    : preferences(preferences),
      wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size) {
    auiManager.SetManagedWindow(this);

    menuBar = new MenuBar(preferences);
    wxFrameBase::SetMenuBar(menuBar);
    wxFrameBase::CreateStatusBar();

    wxAuiToolBar* toolbar = new Toolbar(this);

    auiManager.AddPane(toolbar, wxAuiPaneInfo()
                                    .Name("Toolbar")
                                    .Caption("Toolbar")
                                    .ToolbarPane()
                                    .Top()
                                    .Resizable(true)
                                    .Gripper(false)
                                    .DockFixed(true));

    textEditor = new CodeViewer(this, preferences);

    auto paramsPanel = new ParametersTable(this);

    auto kernelsPanel = new wxListBox(this, SELECT_KERNEL,
                                      wxDefaultPosition, wxSize(200, 150));
    wxFont font(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
                            wxFONTWEIGHT_BOLD);
    kernelsPanel->SetFont(font);

    auto registersPanel =
        new wxTextCtrl(this, wxID_ANY, _("Registers"), wxDefaultPosition,
                       wxSize(300, 150), wxNO_BORDER | wxTE_MULTILINE);


    auto memoryPanel =
        new wxTextCtrl(this, wxID_ANY, _("Memory view"), wxDefaultPosition,
                       wxSize(200, 250), wxNO_BORDER | wxTE_MULTILINE);

    auiManager.AddPane(kernelsPanel, wxLEFT, wxT("Available kernels"));
    auiManager.AddPane(registersPanel, wxRIGHT, wxT("Registers"));
    auiManager.AddPane(paramsPanel, wxBOTTOM, wxT("Parameters view"));
    auiManager.AddPane(memoryPanel, wxBOTTOM, wxT("Memory view"));
    auiManager.AddPane(textEditor, wxAuiPaneInfo().CenterPane().Hide());

    auiManager.Update();
    kernelsPanel->Append("kernelExample1");
    kernelsPanel->Append("kernelExample2");
    kernelsPanel->Append("kernelExample3");

    kernelsPanel->Select(0);

    MainFrame::bindEvents();
}

void MainFrame::onExit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::onAbout(wxCommandEvent& event) {
    wxMessageBox("This is AMD Emulator", "About AMD Emulator");
}

void MainFrame::onOpen(wxCommandEvent& event) {
    wxFileDialog openFileDialog(
        this, "Open file", "", "",
        "Source files (*.asm)|*.asm|All files (*.*)|*.*",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) return;

    fileName = openFileDialog.GetPath();
    textEditor->SetReadOnly(false);
    if (textEditor->LoadFile(fileName)) {
        SetStatusText("Current source file: " + fileName);
        auiManager.GetPane(textEditor).Show();
        auiManager.Update();
    } else {
        wxMessageBox("Can't open file", "Error", wxICON_ERROR);
    }
    textEditor->SetReadOnly(true);
}

void MainFrame::onToggleAddressMargin(wxCommandEvent& event) {
    bool value = event.IsChecked();
    preferences.showAddress = value;
    textEditor->adjustAddressMargin();
}

void MainFrame::onKernelSelected(wxCommandEvent& event) {
    int selection = event.GetSelection();
}

void MainFrame::bindEvents() {
    Bind(wxEVT_MENU, &MainFrame::onOpen, this, OPEN);
    Bind(wxEVT_MENU, &MainFrame::onExit, this, EXIT);
    Bind(wxEVT_MENU, &MainFrame::onAbout, this, ABOUT);
    Bind(wxEVT_MENU, &MainFrame::onToggleAddressMargin, this,
         TOGGLE_ADDRESS_MARGIN);
    Bind(wxEVT_LISTBOX, &MainFrame::onKernelSelected, this, SELECT_KERNEL);
}