#include "CLCommandQueue.h"

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
