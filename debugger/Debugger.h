#ifndef RED_O_LATOR_DEBUGGER_H
#define RED_O_LATOR_DEBUGGER_H

#include <cstdint>
#include <cstddef>

class EmulatorApp;

class Debugger {
   private:
    EmulatorApp* app;

   public:
    explicit Debugger(EmulatorApp* app);
    ~Debugger();

    void onSetBreakpoint(uint64_t address);
    void onRemoveBreakpoint(uint64_t address);
    void onSelectKernel(size_t index);
    void onSelectModel(size_t index);
    void onRequestMemory(uint64_t address);

    void onAttach();
    void onPause();
    void onResume();
    void onStep();
    void onStop();
};

#endif  // RED_O_LATOR_DEBUGGER_H
