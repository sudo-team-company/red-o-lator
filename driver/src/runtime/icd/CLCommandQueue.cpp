#include "CLCommandQueue.h"
#include <runtime-commons.h>

CLCommandQueue::CLCommandQueue(IcdDispatchTable* dispatchTable,
                               CLContext* context,
                               cl_command_queue_properties properties)
    : dispatchTable(dispatchTable), context(context), properties(properties) {
    clRetainContext(context);
}

CLCommandQueue::~CLCommandQueue() {
    clReleaseContext(context);
}

void CLCommandQueue::enqueue(const std::shared_ptr<Command>& command) {
    commands.push_back(command);
}

void CLCommandQueue::flush() {
    if (flushInProcess) {
        return;
    }

    flushInProcess = true;


    while (!commands.empty()) {
        std::shared_ptr<Command> command;

        if (isOutOfOrder()) {
            command = commands.back();
            commands.pop_back();
        } else {
            command = commands.front();
            commands.pop_front();
        }

        command->execute(isOutOfOrder());
    }

    flushInProcess = false;
}

size_t CLCommandQueue::size() {
    return commands.size();
}

bool CLCommandQueue::isOutOfOrder() {
    return properties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
}
