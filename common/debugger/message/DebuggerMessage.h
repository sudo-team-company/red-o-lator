#ifndef RED_O_LATOR_DEBUGGERMESSAGE_H
#define RED_O_LATOR_DEBUGGERMESSAGE_H

enum class DebuggerMessageType {
    ON_STOP,
    ON_PAUSE,
    REQ_MEM,
    REQ_REG,
    SET_BREAKPOINT,
    RUN,
    STOP,
    ON_RES_MEM,
    ON_RES_REG,
    ON_EXC
};

class DebuggerMessage {
   public:
    DebuggerMessageType type;
    explicit DebuggerMessage(DebuggerMessageType type) : type{type} {}
    virtual ~DebuggerMessage() = default;
};

#endif  // RED_O_LATOR_DEBUGGERMESSAGE_H
