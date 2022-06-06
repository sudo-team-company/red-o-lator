#ifndef RED_O_LATOR_ONPAUSE_H
#define RED_O_LATOR_ONPAUSE_H

#include "DebuggerMessage.h"

class OnPause : public DebuggerMessage {
   public:
    ExecutionContext context;

    explicit OnPause(const ExecutionContext& context)
        : DebuggerMessage{DebuggerMessageType::ON_PAUSE}, context{context} {}
};

#endif  // RED_O_LATOR_ONPAUSE_H
