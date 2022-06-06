#ifndef RED_O_LATOR_ONRESPONDMEMORY_H
#define RED_O_LATOR_ONRESPONDMEMORY_H

#include <cstdint>
#include <vector>
#include "DebuggerMessage.h"

class OnRespondMemory : public DebuggerMessage {
   public:
    uint64_t address;
    std::vector<uint8_t> data;

    explicit OnRespondMemory(uint64_t address, std::vector<uint8_t> data)
        : DebuggerMessage{DebuggerMessageType::ON_RES_MEM},
          address{address},
          data{std::move(data)} {}
};

#endif  // RED_O_LATOR_ONRESPONDMEMORY_H
