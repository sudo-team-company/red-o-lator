#include "EmulatorApp.h"
#include "resources/embedded/chip.c"

#include "Events.h"

bool EmulatorApp::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());
    auto bitmap = wxBITMAP_PNG_FROM_DATA(chip);
    wxIcon icon;
    icon.CopyFromBitmap(bitmap);

    frame = new MainFrame(this, "AMD GPU Emulator", wxSize(1440, 900), preferences);

    frame->Center();
    frame->SetIcon(icon);
    frame->Show(true);

    debugger = std::make_unique<Debugger>(this);

    bindEvents();

    return true;
}

void EmulatorApp::bindEvents() {
    Bind(wxEVT_LISTBOX, &EmulatorApp::onKernelSelected, this, SELECT_KERNEL);
    Bind(wxEVT_COMMAND_CHOICE_SELECTED, &EmulatorApp::onWavefrontSelected, this,
         SELECT_WAVEFRONT);
    Bind(wxEVT_COMMAND_CHOICE_SELECTED, &EmulatorApp::onWavefrontSelected, this,
         SELECT_WORK_ITEM_X);

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
//    debugger->onPause();
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

void EmulatorApp::onWavefrontSelected(wxCommandEvent& event) {
    int selection = event.GetSelection();
    ExecutionContext context = debugger->currentExecutionContext;
    switch (event.GetId()) {
        case SELECT_WAVEFRONT:
            context.wavefrontId = selection;
            break;
        case SELECT_WORK_ITEM_X:
            context.workItemX = selection;
            break;
        case SELECT_WORK_ITEM_Y:
            context.workItemY = selection;
            break;
        case SELECT_WORK_ITEM_Z:
            context.workItemZ = selection;
            break;
    }
    debugger->onSelectExecutionContext(context);
}

void EmulatorApp::startExecution() {
    frame->enableTool(ATTACH, false);
    frame->enableTool(PAUSE, true);
    frame->enableTool(RESUME, false);
    frame->enableTool(STEP, false);
    frame->enableTool(STOP, true);
    frame->enableKernelList(false);
    frame->enableMemoryPanel(false);

    frame->SetStatusText("Waiting for kernel", 0);
    frame->SetStatusText("", 1);
}

void EmulatorApp::pauseExecution(const ExecutionContext& context) {
    frame->enableTool(ATTACH, false);
    frame->enableTool(PAUSE, false);
    frame->enableTool(RESUME, true);
    frame->enableTool(STEP, true);
    frame->enableTool(STOP, true);

    frame->enableMemoryPanel(true);
    frame->setExecutionMarker(context.address);
    frame->setExecutionContext(context);

    frame->SetStatusText(
        wxString::Format("Paused at address: 0x%.6llX", context.address), 0);
    frame->SetStatusText(wxString::Format("Wavefront: %i", context.wavefrontId),
                         1);
}

void EmulatorApp::stopExecution() {
    frame->enableTool(ATTACH, true);
    frame->enableTool(PAUSE, false);
    frame->enableTool(RESUME, false);
    frame->enableTool(STEP, false);
    frame->enableTool(STOP, false);
    frame->enableKernelList(true);
    frame->enableMemoryPanel(true);

    frame->removeExecutionMarker();

    frame->SetStatusText("Execution has stopped", 0);
    frame->SetStatusText("", 1);
}

void EmulatorApp::setException(const std::string& what) {
    frame->enableTool(ATTACH, true);
    frame->enableTool(PAUSE, false);
    frame->enableTool(RESUME, false);
    frame->enableTool(STEP, false);
    frame->enableTool(STOP, false);
    frame->enableKernelList(true);
    frame->enableMemoryPanel(true);

    frame->removeExecutionMarker();

    frame->SetStatusText("Exception encountered", 0);
    frame->SetStatusText(what, 1);
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
    const std::vector<InstructionView>& instructions) {
    frame->setInstructions(instructions);
}

void EmulatorApp::onSetBreakpoint(uint64_t address) const {
    debugger->onSetBreakpoint(address);
}

void EmulatorApp::onRemoveBreakpoint(uint64_t address) const {
    debugger->onRemoveBreakpoint(address);
}

void EmulatorApp::onRequestMemory(uint64_t address) const {
    debugger->onRequestMemory(address);
}

void EmulatorApp::setMemoryView(const std::vector<uint8_t>& memory,
                                uint64_t address) {
    frame->setMemoryView(memory, address);
}

void EmulatorApp::setRegisters(const RegData& data) {
    frame->setRegisters(data);
}
