#ifndef RED_O_LATOR_ONRESPONDREGISTERS_H
#define RED_O_LATOR_ONRESPONDREGISTERS_H

#include "DebuggerMessage.h"
#include "model/reg/RegData.h"

class OnRespondRegisters : public DebuggerMessage {
   public:
    RegData data;
    explicit OnRespondRegisters(RegData data)
        : DebuggerMessage{DebuggerMessageType::ON_RES_REG}, data{data} {}
};

#endif  // RED_O_LATOR_ONRESPONDREGISTERS_H
