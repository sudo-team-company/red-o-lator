#pragma once

#include <memory>
#include <stack>

#include "runtime/icd/CLEvent.h"
#include "runtime/icd/CLMem.h"

class Command {
   public:
    explicit Command(CLCommandQueue* commandQueue);

    virtual ~Command();

    virtual cl_command_type getCommandType() = 0;

    virtual void execute();

    CLEvent* requireEvent();
    void setEvent(CLEvent* event);

    void addEventsToWaitList(cl_uint count, const cl_event* originWaitList);

    CLCommandQueue* const commandQueue;

    std::stack<CLEvent*> waitList{};

   protected:
    virtual void executeImpl() const = 0;
    CLEvent* mEvent = nullptr;
};

struct BufferReadCommand : public Command {
    BufferReadCommand(CLCommandQueue* commandQueue,
                      CLMem* buffer,
                      size_t size,
                      size_t offset,
                      void* outputPtr);

    ~BufferReadCommand() override;
    cl_command_type getCommandType() override;

    CLMem* const buffer;
    const size_t size;
    const size_t offset;
    void* const outputPtr;

   protected:
    void executeImpl() const override;
};

struct BufferWriteCommand : public Command {
    BufferWriteCommand(CLCommandQueue* commandQueue,
                       CLMem* buffer,
                       size_t size,
                       size_t offset,
                       const void* dataPtr);

    ~BufferWriteCommand() override;
    cl_command_type getCommandType() override;

    CLMem* const buffer;
    const size_t size;
    const size_t offset;
    const void* const dataPtr;

   protected:
    void executeImpl() const override;
};

struct BufferFillCommand : public Command {
    BufferFillCommand(CLCommandQueue* commandQueue,
                      CLMem* buffer,
                      const void* pattern,
                      size_t patternSize,
                      size_t offset,
                      size_t size);

    ~BufferFillCommand() override;
    cl_command_type getCommandType() override;

    CLMem* const buffer;
    std::byte* const pattern;
    const size_t patternSize;
    const size_t offset;
    const size_t size;

   protected:
    void executeImpl() const override;
};

struct BufferCopyCommand : public Command {
    BufferCopyCommand(CLCommandQueue* commandQueue,
                      CLMem* srcBuffer,
                      CLMem* dstBuffer,
                      size_t srcOffset,
                      size_t dstOffset,
                      size_t size);

    ~BufferCopyCommand() override;
    cl_command_type getCommandType() override;

    CLMem* const srcBuffer;
    CLMem* const dstBuffer;
    const size_t srcOffset;
    const size_t dstOffset;
    const size_t size;

   protected:
    void executeImpl() const override;
};

struct KernelExecutionCommand : public Command {
    KernelExecutionCommand(CLCommandQueue* commandQueue,
                           CLKernel* kernel,
                           cl_uint workDim,
                           const size_t* globalWorkOffset,
                           const size_t* globalWorkSize,
                           const size_t* localWorkSize);

    ~KernelExecutionCommand() override;
    cl_command_type getCommandType() override;

    CLKernel* const kernel;
    const cl_uint workDim;
    const size_t* const globalWorkOffset;
    const size_t* const globalWorkSize;
    const size_t* const localWorkSize;

   protected:
    void executeImpl() const override;
};
