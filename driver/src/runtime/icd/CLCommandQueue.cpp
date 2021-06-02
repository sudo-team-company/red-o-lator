#include <cassert>

#include "CLCommandQueue.h"

CLCommandQueue::CLCommandQueue(IcdDispatchTable* dispatchTable,
                               CLContext* context,
                               cl_command_queue_properties properties)
    : dispatchTable(dispatchTable), context(context), properties(properties) {
    clRetainContext(context);
}

CLCommandQueue::~CLCommandQueue() {
    clReleaseContext(context);
}

void CLCommandQueue::enqueue(Command* command) {
    command->requireEvent()->setQueued();
    commands.push_back(command);
}

void CLCommandQueue::flush() {
    if (flushInProcess) {
        return;
    }

    flushInProcess = true;

    std::vector<Command*> executedCommands;
    while (!commands.empty()) {
        Command* command;

        if (isOutOfOrder()) {
            command = commands.back();
            commands.pop_back();
        } else {
            command = commands.front();
            commands.pop_front();
        }

        command->execute();
        executedCommands.push_back(command);
    }

    for (auto* command : executedCommands) {
        delete command;
    }

    flushInProcess = false;
}

size_t CLCommandQueue::size() const {
    return commands.size();
}

bool CLCommandQueue::isProfilingEnabled() const {
    return properties & CL_QUEUE_PROFILING_ENABLE;
}

bool CLCommandQueue::isOutOfOrder() const {
    return properties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
}
