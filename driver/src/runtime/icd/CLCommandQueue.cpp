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

void CLCommandQueue::enqueue(const std::shared_ptr<const Command>& command) {
    commands.emplace(command);
}

void CLCommandQueue::flush() {
    while (!commands.empty()) {
        const auto command = commands.front();
        commands.pop();
        command->execute();
    }
}

size_t CLCommandQueue::size() {
    return commands.size();
}
