#ifndef RED_O_LATOR_EXECUTOR_H
#define RED_O_LATOR_EXECUTOR_H

#include "debugger/breakpoint/BreakpointStorage.h"
#include "debugger/channel/Channel.h"
#include "debugger/message/DebuggerMessage.h"
#include "model/kernel/Kernel.h"

class Executor {
   private:
    static void writeArg(uint64_t&, size_t&, const KernelArgumentStore&);
    static void readArgs(size_t, KernelArgumentStore&);
    static size_t evaluateArgSize(const std::vector<KernelArgumentStore>& args);

   public:
    static void executeKernel(Kernel& kernel,
                              Channel<std::shared_ptr<DebuggerMessage>>& outMessageChannel,
                              Channel<std::shared_ptr<DebuggerMessage>>& inMessageChannel,
                              const BreakpointStorage& breakpointStorage);
};

#endif  // RED_O_LATOR_EXECUTOR_H
