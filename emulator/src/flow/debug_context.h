#ifndef RED_O_LATOR_DEBUG_CONTEXT_H
#define RED_O_LATOR_DEBUG_CONTEXT_H

#include "debugger/breakpoint/BreakpointStorage.h"
#include "debugger/channel/Channel.h"
#include "debugger/exec/ExecutionContext.h"
#include "debugger/message/DebuggerMessage.h"
#include "model/reg/RegData.h"
#include "wavefront.h"

enum class ExecutionMode { STOP, PAUSE, RUN, STEP, EXC };

class DebugContext {
   private:
    Channel<std::shared_ptr<DebuggerMessage>>* outMessageChannel;
    Channel<std::shared_ptr<DebuggerMessage>>* inMessageChannel;
    BreakpointStorage breakpoints;
    WorkGroup* currentWorkGroup;

    void processMessage(const std::shared_ptr<DebuggerMessage>& msg);

    bool isTerminal();

   public:
    ExecutionMode executionMode = ExecutionMode::RUN;

    explicit DebugContext(Channel<std::shared_ptr<DebuggerMessage>>* outMessageChannel,
                          Channel<std::shared_ptr<DebuggerMessage>>* inMessageChannel,
                          BreakpointStorage breakpoints);
    void setWorkGroup(WorkGroup* wg);
    void readMessages();
    void waitForResume();
    void processInstruction(const ExecutionContext& context);
    void processException(const std::string& what, const ExecutionContext& context);
    ExecutionContext createContext(uint32_t address, Wavefront* wf);
    RegData getRegisters(const ExecutionContext& context);
};

#endif  // RED_O_LATOR_DEBUG_CONTEXT_H
