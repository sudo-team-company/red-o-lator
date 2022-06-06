#ifndef RED_O_LATOR_DEBUGGER_H
#define RED_O_LATOR_DEBUGGER_H

#include <cstddef>
#include <cstdint>
#include <list>
#include <mutex>
#include "core/Launch.h"
#include "debugger/breakpoint/BreakpointStorage.h"
#include "debugger/exec/ExecutionContext.h"
#include "ipc/IpcServer.h"
#include "model/kernel/Kernel.h"
#include "src/Executor.h"

class EmulatorApp;

class Debugger {
    friend class EmulatorApp;

   private:
    static constexpr int WORKGROUP_ALL = -1;
    static constexpr int KERNEL_SELECT_NONE = -1;

    EmulatorApp* app;

    std::vector<std::unique_ptr<Kernel>> programs;
    BreakpointStorage breakpointStorage;
    int currentKernelIdx = KERNEL_SELECT_NONE;
    bool hasKernelRuntime = false;
    ExecutionContext currentExecutionContext;
    std::unique_ptr<IpcServer> ipcServer;
    std::unique_ptr<Launch> launch;

    std::mutex mutex;
    Channel<std::shared_ptr<DebuggerMessage>> outMessageChannel;
    Channel<std::shared_ptr<DebuggerMessage>> inMessageChannel;
    std::unique_ptr<std::thread> emulatorThread;
    std::unique_ptr<std::thread> messageReaderThread;

    void runKernel();
    void selectKernel(int kernelIdx);
    void processDebuggerMessages();
    void changeExecutionContext(const ExecutionContext& context);
    void pauseExecution(const ExecutionContext& context);
    void requestRegisters(const ExecutionContext& context);

   public:
    explicit Debugger(EmulatorApp* app);
    ~Debugger();

    void addKernel(Kernel kernel);
    void onSetBreakpoint(uint64_t address);
    void onRemoveBreakpoint(uint64_t address);
    void onSelectKernel(size_t index);
    void onSelectExecutionContext(const ExecutionContext& context);
    void onRequestMemory(uint64_t address);

    void onAttach();
    void onPause(uint64_t address, int workGroupId);
    void onResume();
    void onStep();
    void onStop();
};

#endif  // RED_O_LATOR_DEBUGGER_H
