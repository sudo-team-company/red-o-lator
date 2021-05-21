#ifndef RED_O_LATOR_DEBUGGER_H
#define RED_O_LATOR_DEBUGGER_H

class EmulatorApp;

class Debugger {
   private:
    EmulatorApp* app;

   public:
    explicit Debugger(EmulatorApp* app);
    ~Debugger();

    void onSetBreakpoint(size_t address);
    void onRemoveBreakpoint(size_t address);
    void onSelectKernel(size_t index);
    void onSelectModel(size_t index);
    void onRequestMemory(size_t index);

    void onAttach();
    void onPause();
    void onResume();
    void onStep();
    void onStop();
};

#endif  // RED_O_LATOR_DEBUGGER_H
