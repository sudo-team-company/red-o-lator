#pragma once

#include <memory>
#include <stack>

#include "runtime/icd/CLEvent.h"
#include "runtime/icd/CLMem.h"

class Command {
   public:
    virtual ~Command();
    virtual void execute(bool outOfOrder);

    CLEvent* event = nullptr;
    std::stack<CLEvent*> waitList{};

   protected:
    virtual void executeImpl() const = 0;
};

struct BufferReadCommand : public Command {
    BufferReadCommand(CLMem* buffer,
                      size_t size,
                      size_t offset,
                      void* outputPtr);

    ~BufferReadCommand() override;

    CLMem* const buffer;
    const size_t size;
    const size_t offset;
    void* const outputPtr;

   protected:
    void executeImpl() const override;
};

struct BufferWriteCommand : public Command {
    BufferWriteCommand(CLMem* buffer,
                       size_t size,
                       size_t offset,
                       const void* dataPtr);

    ~BufferWriteCommand() override;

    CLMem* const buffer;
    const size_t size;
    const size_t offset;
    const void* const dataPtr;

   protected:
    void executeImpl() const override;
};

struct KernelExecutionCommand : public Command {
    KernelExecutionCommand(CLKernel* kernel,
                         cl_uint workDim,
                         const size_t* globalWorkOffset,
                         const size_t* globalWorkSize,
                         const size_t* localWorkSize);

    ~KernelExecutionCommand() override;

    CLKernel* const kernel;
    const cl_uint workDim;
    const size_t* const globalWorkOffset;
    const size_t* const globalWorkSize;
    const size_t* const localWorkSize;

   protected:
    void executeImpl() const override;
};
