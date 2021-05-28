#pragma once

#include <iostream>
#include <memory>
#include <queue>

#include "IcdDispatchTable.h"
#include "runtime/command/Command.h"
#include "runtime/icd/CLContext.h"

struct CLCommandQueue {
   public:
    explicit CLCommandQueue(IcdDispatchTable* dispatchTable,
                            CLContext* context,
                            cl_command_queue_properties properties);

    ~CLCommandQueue();

    IcdDispatchTable* const dispatchTable;
    CLContext* const context;
    cl_command_queue_properties properties;

    unsigned int referenceCount = 1;

    void enqueue(const std::shared_ptr<Command>& command);

    void flush();

    size_t size();

    bool isOutOfOrder();

   private:
    std::deque<std::shared_ptr<Command>> commands =
        std::deque<std::shared_ptr<Command>>();

    bool flushInProcess = false;
};
