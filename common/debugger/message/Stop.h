#ifndef RED_O_LATOR_STOP_H
#define RED_O_LATOR_STOP_H

#include "DebuggerMessage.h"

class Stop : public DebuggerMessage {

   public:
    Stop() : DebuggerMessage{DebuggerMessageType::STOP} {}
};

#endif  // RED_O_LATOR_STOP_H
