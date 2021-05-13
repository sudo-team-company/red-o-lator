#pragma once

#include <memory>
#include "runtime/icd/icd.h"

class Command {
   public:
    virtual ~Command() = default;
    virtual void execute() const = 0;
};

struct BufferReadCommand : public Command {
    ~BufferReadCommand() override;

    void execute() const override;

    CLMem* buffer{};
    size_t size{};
    size_t offset{};
    void* outputPtr{};
};

struct BufferWriteCommand : public Command {
    ~BufferWriteCommand() override;

    void execute() const override;

    CLMem* buffer{};
    size_t size{};
    size_t offset{};
    const void* dataPtr{};
};
