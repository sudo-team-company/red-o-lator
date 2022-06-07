#ifndef RED_O_LATOR_ONSTOP_H
#define RED_O_LATOR_ONSTOP_H

#include "DebuggerMessage.h"

class OnStop : public DebuggerMessage {

   public:
    OnStop() : DebuggerMessage{DebuggerMessageType::ON_STOP} {}
};

#endif  // RED_O_LATOR_ONSTOP_H
