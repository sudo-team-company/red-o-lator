#include <cassert>

#include "Command.h"

Command::~Command() {
    delete event;
}

void Command::execute(bool outOfOrder) {
    assert(event != nullptr);

    if (!event->isQueued()) {
        assert(waitList.empty());
        return;
    }

    if (outOfOrder) {
        while (!waitList.empty()) {
            const auto waitEvent = waitList.top();
            waitList.pop();

            if (waitEvent->isQueued()) {
                waitEvent->command->execute(outOfOrder);
            }
        }
    }

    event->commandExecutionStatus = CL_RUNNING;
    executeImpl();
    event->commandExecutionStatus = CL_COMPLETE;
}

