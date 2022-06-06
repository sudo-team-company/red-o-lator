#ifndef RED_O_LATOR_RUN_H
#define RED_O_LATOR_RUN_H

#include "DebuggerMessage.h"

class Run : public DebuggerMessage {
   public:
    bool stepping;

    explicit Run(bool stepping = false)
        : DebuggerMessage{DebuggerMessageType::RUN}, stepping{stepping} {}
};

#endif  // RED_O_LATOR_RUN_H
