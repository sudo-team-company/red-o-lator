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

void CLCommandQueue::enqueue(const std::shared_ptr<const Command>& command) {
    commands.emplace(command);
}

void CLCommandQueue::flush() {
    if (flushInProcess) {
        return;
    }

    flushInProcess = true;

    while (!commands.empty()) {
        const auto command = commands.front();
        command->execute();
        commands.pop();
    }

    flushInProcess = false;
}

size_t CLCommandQueue::size() {
    return commands.size();
}
