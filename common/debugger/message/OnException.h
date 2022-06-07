#ifndef RED_O_LATOR_ONEXCEPTION_H
#define RED_O_LATOR_ONEXCEPTION_H

#include <string>
#include "DebuggerMessage.h"

class OnException : public DebuggerMessage {
   public:
    std::string what;
    explicit OnException(std::string what)
        : DebuggerMessage{DebuggerMessageType::ON_EXC}, what{std::move(what)} {}
};

#endif  // RED_O_LATOR_ONEXCEPTION_H
