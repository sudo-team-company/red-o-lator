#pragma once

#include <memory>
#include <queue>

#include "IcdDispatchTable.h"
#include "runtime/command/Command.h"
#include "runtime/icd/CLContext.h"

class Command;

struct CLCommandQueue {
   public:
    explicit CLCommandQueue(IcdDispatchTable* dispatchTable,
                            CLContext* context,
                            cl_command_queue_properties properties);

    virtual ~CLCommandQueue();

    IcdDispatchTable* const dispatchTable;
    CLContext* const context;
    cl_command_queue_properties properties;

    unsigned int referenceCount = 1;

    void enqueue(const std::shared_ptr<const Command>& command);

    void flush();

    size_t size();

   private:
    std::queue<std::shared_ptr<const Command>> commands =
        std::queue<std::shared_ptr<const Command>>();
};
