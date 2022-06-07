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
#include "model/reg/RegData.h"
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
    void onWavefrontSelected(wxCommandEvent& event);

   public:
    void startExecution();
    void pauseExecution(const ExecutionContext& context);
    void stopExecution();
    void setException(const std::string& what);

    void setKernelList(const std::vector<std::string>& kernels,
                       int currentIdx = 0);

    void setGlobalParameters(const std::vector<Parameter>& parameters);
    void setKernelParameters(const std::vector<Parameter>& parameters);

    void setInstructions(const std::vector<InstructionView>& instructions);

    void onSetBreakpoint(uint64_t address) const;
    void onRemoveBreakpoint(uint64_t address) const;
    void onRequestMemory(uint64_t address) const;

    void setMemoryView(const std::vector<uint8_t>& memory, uint64_t address);
    void setRegisters(const RegData& data);

};

#endif  // RED_O_LATOR_EMULATORAPP_H
