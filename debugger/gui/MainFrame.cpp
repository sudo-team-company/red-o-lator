#include "MainFrame.h"
#include <wx/grid.h>
#include "EmulatorApp.h"
#include "Events.h"

MainFrame::MainFrame(const EmulatorApp* parent,
                     const wxString& title,
                     const wxSize& size,
                     Preferences& preferences)
    : app(parent),
      preferences(preferences),
      wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size) {
    auiManager.SetManagedWindow(this);

    menuBar = new MenuBar(preferences);
    wxFrameBase::SetMenuBar(menuBar);
    wxFrameBase::CreateStatusBar(5);

    toolbar = new Toolbar(this);

    auiManager.AddPane(toolbar, wxAuiPaneInfo()
                                    .Name("Toolbar")
                                    .Caption("Toolbar")
                                    .ToolbarPane()
                                    .Top()
                                    .Resizable(true)
                                    .Gripper(false)
                                    .DockFixed(true));

    textEditor = new CodeViewer(this, preferences);
    paramsPanel = new ParametersTable(this);
    kernelPanel = new KernelPanel(this);
    memoryPanel = new MemoryView(this);
    registersPanel = new RegistersTable(this);

    auiManager.AddPane(kernelPanel, wxLEFT, wxT("Available kernels"));
    auiManager.AddPane(registersPanel, wxRIGHT, wxT("Registers"));
    auiManager.AddPane(paramsPanel, wxBOTTOM, wxT("Parameters view"));
    auiManager.AddPane(memoryPanel, wxBOTTOM, wxT("Memory view"));
    auiManager.AddPane(textEditor, wxAuiPaneInfo().CenterPane().Hide());

    auiManager.Update();

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

void MainFrame::bindEvents() {
    Bind(wxEVT_MENU, &MainFrame::onExit, this, EXIT);
    Bind(wxEVT_MENU, &MainFrame::onAbout, this, ABOUT);
    Bind(wxEVT_MENU, &MainFrame::onToggleAddressMargin, this,
         TOGGLE_ADDRESS_MARGIN);
    Bind(wxEVT_MENU, &MainFrame::onOpen, this, OPEN);
    Bind(wxEVT_BUTTON, &MainFrame::onRequestMemory, this, REQUEST_MEMORY);
}

void MainFrame::enableTool(int toolId, bool state) {
    toolbar->EnableTool(toolId, state);
}

void MainFrame::selectKernel(int kernelIdx) {
    kernelPanel->Select(kernelIdx);
}

void MainFrame::updateKernelList(const std::vector<std::string>& kernels) {
    auto toWx = [](const std::string& name) { return wxString(name); };
    std::vector<wxString> out(kernels.size());
    std::transform(kernels.begin(), kernels.end(), out.begin(), toWx);
    kernelPanel->Set(out);
}

void MainFrame::enableKernelList(bool state) {
    kernelPanel->Enable(state);
}

void MainFrame::enableModelList(bool state) {
    toolbar->choiceModel->Enable(state);
}

void MainFrame::enableMemoryPanel(bool state) {
    memoryPanel->addressButton->Enable(state);
    memoryPanel->addressInput->Enable(state);
}

void MainFrame::setParameters(const ParametersCategory& category,
                              const std::vector<Parameter>& parameters) {
    wxPGProperty* categoryPtr;

    switch (category) {
        case ParametersCategory::GLOBAL:
            categoryPtr = paramsPanel->globalCategory;
            break;

        case ParametersCategory::KERNEL:
            categoryPtr = paramsPanel->kernelCategory;
            break;
        default:
            return;
    }

    categoryPtr->DeleteChildren();
    for (auto& param : parameters) {
        auto prop = categoryPtr->AppendChild(
            new wxStringProperty(param.name, wxPG_LABEL, param.value));
        paramsPanel->SetPropertyReadOnly(prop, true);
    }
}

void MainFrame::setInstructions(const std::vector<Instruction>& instructions) {
    textEditor->setInstructions(instructions);

    auiManager.GetPane(textEditor).Show();
    auiManager.Update();
}

void MainFrame::setExecutionMarker(uint64_t address) {
    textEditor->setExecutionMarker(address);
}

void MainFrame::removeExecutionMarker() {
    textEditor->clearExecutionMarker();
}

void MainFrame::onSetBreakpoint(uint64_t address) const {
    app->onSetBreakpoint(address);
}

void MainFrame::onRemoveBreakpoint(uint64_t address) const {
    app->onRemoveBreakpoint(address);
}

void MainFrame::onRequestMemory(wxCommandEvent& event) {
    auto value = memoryPanel->addressInput->GetValue();
    auto hexString = value.utf8_string();
    if (hexString.empty()) {
        hexString = "0";
    }
    auto address = std::stoull(hexString, nullptr, 16);
    app->onRequestMemory(address);
}

void MainFrame::setMemoryView(const void* memory, uint64_t size, uint64_t address) {
    memoryPanel->setMemoryView(memory, size, address);
}

void MainFrame::setModelChoice(const std::vector<std::string>& models,
                               size_t currentIdx) {
    auto choice = toolbar->choiceModel;
    for (auto& model : models) {
        choice->Append(model);
    }
    choice->SetSelection(static_cast<int>(currentIdx));
}
