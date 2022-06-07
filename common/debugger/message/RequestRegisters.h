#ifndef RED_O_LATOR_REQUESTREGISTERS_H
#define RED_O_LATOR_REQUESTREGISTERS_H

#include "DebuggerMessage.h"

class RequestRegisters : public DebuggerMessage {
   public:
    ExecutionContext context;
    explicit RequestRegisters(ExecutionContext context)
        : DebuggerMessage{DebuggerMessageType::REQ_REG}, context{context} {}
};

#endif  // RED_O_LATOR_REQUESTREGISTERS_H
