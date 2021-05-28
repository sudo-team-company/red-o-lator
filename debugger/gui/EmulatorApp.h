#ifndef RED_O_LATOR_EMULATORAPP_H
#define RED_O_LATOR_EMULATORAPP_H

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include "../Debugger.h"
#include "../data/Parameter.h"
#include "../data/Preferences.h"
#include "MainFrame.h"
#include "wx/wx.h"

class EmulatorApp : public wxApp {
   private:
    Preferences preferences;
    MainFrame* frame;
    std::unique_ptr<Debugger> debugger;

    void bindEvents();

    bool OnInit() override;
    void onAttach(wxCommandEvent& event);
    void onPause(wxCommandEvent& event);
    void onResume(wxCommandEvent& event);
    void onStep(wxCommandEvent& event);
    void onStop(wxCommandEvent& event);
    void onKernelSelected(wxCommandEvent& event);
    void onModelSelected(wxCommandEvent& event);

   public:
    void startExecution();
    void pauseExecution(uint64_t address, int workGroupId);
    void stopExecution();

    void setKernelList(const std::vector<std::string>& kernels,
                       int currentIdx = 0);

    void setGlobalParameters(const std::vector<Parameter>& parameters);
    void setKernelParameters(const std::vector<Parameter>& parameters);

    void setInstructions(const std::vector<Instruction>& instructions);

    void onSetBreakpoint(uint64_t address) const;
    void onRemoveBreakpoint(uint64_t address) const;
    void onRequestMemory(uint64_t address) const;

    void setMemoryView(const void* memory, uint64_t size, uint64_t address);

    void setModelChoice(const std::vector<std::string>& models,
                        size_t currentIdx = 0);
};

#endif  // RED_O_LATOR_EMULATORAPP_H
