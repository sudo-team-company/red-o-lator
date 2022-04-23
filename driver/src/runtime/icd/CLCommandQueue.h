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
    const cl_command_queue_properties properties;

    unsigned int referenceCount = 1;

    void enqueue(Command* command);

    void flush();

    [[nodiscard]] size_t size() const;

    [[nodiscard]] bool isProfilingEnabled() const;
    [[nodiscard]] bool isOutOfOrder() const;

   private:
    std::deque<Command*> commands = std::deque<Command*>();

    bool flushInProcess = false;
};
