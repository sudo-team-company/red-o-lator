#include "EmulatorApp.h"
#include "resources/embedded/chip.c"

#include "Events.h"

bool EmulatorApp::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());
    auto bitmap = wxBITMAP_PNG_FROM_DATA(chip);
    wxIcon icon;
    icon.CopyFromBitmap(bitmap);

    frame = new MainFrame(this, "AMD Emulator", wxSize(1440, 900), preferences);

    frame->Center();
    frame->SetIcon(icon);
    frame->Show(true);

    debugger = std::make_unique<Debugger>(this);

    bindEvents();

    return true;
}

void EmulatorApp::bindEvents() {
    Bind(wxEVT_LISTBOX, &EmulatorApp::onKernelSelected, this, SELECT_KERNEL);
    Bind(wxEVT_COMMAND_CHOICE_SELECTED, &EmulatorApp::onModelSelected, this,
         SELECT_MODEL);

    Bind(wxEVT_MENU, &EmulatorApp::onAttach, this, ATTACH);
    Bind(wxEVT_MENU, &EmulatorApp::onPause, this, PAUSE);
    Bind(wxEVT_MENU, &EmulatorApp::onResume, this, RESUME);
    Bind(wxEVT_MENU, &EmulatorApp::onStep, this, STEP);
    Bind(wxEVT_MENU, &EmulatorApp::onStop, this, STOP);
}

void EmulatorApp::onAttach(wxCommandEvent& event) {
    debugger->onAttach();
}

void EmulatorApp::onPause(wxCommandEvent& event) {
    debugger->onPause();
}

void EmulatorApp::onResume(wxCommandEvent& event) {
    debugger->onResume();
}

void EmulatorApp::onStep(wxCommandEvent& event) {
    debugger->onStep();
}

void EmulatorApp::onStop(wxCommandEvent& event) {
    debugger->onStop();
}

void EmulatorApp::onKernelSelected(wxCommandEvent& event) {
    int selection = event.GetSelection();
    debugger->onSelectKernel(selection);
}

void EmulatorApp::onModelSelected(wxCommandEvent& event) {
    int selection = event.GetSelection();
    debugger->onSelectModel(selection);
}

void EmulatorApp::startExecution() {
    frame->enableTool(ATTACH, false);
    frame->enableTool(PAUSE, true);
    frame->enableTool(RESUME, false);
    frame->enableTool(STEP, false);
    frame->enableTool(STOP, true);
    frame->enableKernelList(false);
    frame->enableModelList(false);
    frame->enableMemoryPanel(false);

    frame->SetStatusText("Waiting for kernel", 0);
    frame->SetStatusText("", 1);
}

void EmulatorApp::pauseExecution(size_t address,
                                 int workGroupId) {
    frame->enableTool(ATTACH, false);
    frame->enableTool(PAUSE, false);
    frame->enableTool(RESUME, true);
    frame->enableTool(STEP, true);
    frame->enableTool(STOP, true);

    frame->enableMemoryPanel(true);
    frame->setExecutionMarker(address);

    frame->SetStatusText(
        wxString::Format("Paused at address: 0x%.6llX", address), 0);
    frame->SetStatusText(wxString::Format("Work Group: %i", workGroupId),
                         1);
}

void EmulatorApp::stopExecution() {
    frame->enableTool(ATTACH, true);
    frame->enableTool(PAUSE, false);
    frame->enableTool(RESUME, false);
    frame->enableTool(STEP, false);
    frame->enableTool(STOP, false);
    frame->enableKernelList(true);
    frame->enableModelList(true);
    frame->enableMemoryPanel(true);

    frame->removeExecutionMarker();

    frame->SetStatusText("Execution has stopped", 0);
    frame->SetStatusText("", 1);
}

void EmulatorApp::setKernelList(const std::vector<std::string>& kernels,
                                int currentIdx) {
    frame->updateKernelList(kernels);
    frame->selectKernel(currentIdx);
}

void EmulatorApp::setGlobalParameters(
    const std::vector<Parameter>& parameters) {
    frame->setParameters(ParametersCategory::GLOBAL, parameters);
}

void EmulatorApp::setKernelParameters(
    const std::vector<Parameter>& parameters) {
    frame->setParameters(ParametersCategory::KERNEL, parameters);
}

void EmulatorApp::setInstructions(
    const std::vector<Instruction>& instructions) {
    frame->setInstructions(instructions);
}

void EmulatorApp::onSetBreakpoint(size_t address) const {
    debugger->onSetBreakpoint(address);
}

void EmulatorApp::onRemoveBreakpoint(size_t address) const {
    debugger->onRemoveBreakpoint(address);
}

void EmulatorApp::onRequestMemory(size_t address) const {
    debugger->onRequestMemory(address);
}

void EmulatorApp::setMemoryView(const void* memory,
                                size_t size,
                                size_t address) {
    frame->setMemoryView(memory, size, address);
}

void EmulatorApp::setModelChoice(const std::vector<std::string>& models,
                                 size_t currentIdx) {
    frame->setModelChoice(models, currentIdx);
}
