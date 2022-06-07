#ifndef RED_O_LATOR_REQUESTMEMORY_H
#define RED_O_LATOR_REQUESTMEMORY_H

#include "DebuggerMessage.h"

class RequestMemory : public DebuggerMessage {
   public:
    uint64_t address;

    explicit RequestMemory(uint64_t address)
        : DebuggerMessage{DebuggerMessageType::REQ_MEM}, address{address} {}
};

#endif  // RED_O_LATOR_REQUESTMEMORY_H
