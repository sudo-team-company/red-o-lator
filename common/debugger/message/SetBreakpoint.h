#ifndef RED_O_LATOR_SETBREAKPOINT_H
#define RED_O_LATOR_SETBREAKPOINT_H

#include "DebuggerMessage.h"

class SetBreakpoint : public DebuggerMessage {
   public:
    Breakpoint breakpoint;
    bool remove;

    explicit SetBreakpoint(Breakpoint breakpoint, bool remove = false)
        : DebuggerMessage{DebuggerMessageType::SET_BREAKPOINT},
          breakpoint{breakpoint},
          remove{remove} {}
};

#endif  // RED_O_LATOR_SETBREAKPOINT_H
