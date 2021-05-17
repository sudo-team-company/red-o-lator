#pragma once

#include <memory>

#include "icd.h"
#include "runtime/command/Command.h"

class Command;

class CLCommandQueue {
   public:
    explicit CLCommandQueue(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;

    bool profilingEnabled = false;
    bool outOfOrderMode = false;

    unsigned int referenceCount = 1;

    void enqueue(const std::shared_ptr<const Command>& command);

    void flush();

   private:
    std::queue<std::shared_ptr<const Command>> commands =
        std::queue<std::shared_ptr<const Command>>();
};
