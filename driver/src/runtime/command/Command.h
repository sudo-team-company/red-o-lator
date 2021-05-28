#pragma once

#include <memory>
#include "runtime/icd/CLMem.h"

class Command {
   public:
    virtual ~Command() = default;
    virtual void execute() const = 0;
};

struct BufferReadCommand : public Command {
    BufferReadCommand(CLMem* buffer,
                      size_t size,
                      size_t offset,
                      void* outputPtr);

    ~BufferReadCommand() override;

    void execute() const override;

    CLMem* const buffer;
    const size_t size;
    const size_t offset;
    void* const outputPtr;
};

struct BufferWriteCommand : public Command {
    BufferWriteCommand(CLMem* buffer,
                       size_t size,
                       size_t offset,
                       const void* dataPtr);

    ~BufferWriteCommand() override;

    void execute() const override;

    CLMem* const buffer;
    const size_t size;
    const size_t offset;
    const void* const dataPtr;
};

struct KernelExecutionCommand : public Command {
    KernelExecutionCommand(CLKernel* kernel,
                         cl_uint workDim,
                         const size_t* globalWorkOffset,
                         const size_t* globalWorkSize,
                         const size_t* localWorkSize);

    ~KernelExecutionCommand() override;

    void execute() const override;

    CLKernel* const kernel;
    const cl_uint workDim;
    const size_t* const globalWorkOffset;
    const size_t* const globalWorkSize;
    const size_t* const localWorkSize;
};
