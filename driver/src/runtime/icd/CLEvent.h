#pragma once

#include <utility>

#include "icd.h"
#include "runtime/command/Command.h"

using CommandExecutionStatus = cl_int;

class Command;

struct CLEvent {
    explicit CLEvent(IcdDispatchTable* dispatchTable,
                     std::shared_ptr<Command> command,
                     CommandExecutionStatus commandExecutionStatus = CL_QUEUED)
        : dispatchTable(dispatchTable),
          command(std::move(command)),
          commandExecutionStatus(commandExecutionStatus){};

    IcdDispatchTable* const dispatchTable;
    std::shared_ptr<Command> const command;

    CommandExecutionStatus commandExecutionStatus;

    [[nodiscard]] bool isCompleted() const;

    [[nodiscard]] bool isQueued() const;
};
