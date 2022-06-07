#include <cstring>
#include "Command.h"

BufferCopyCommand::BufferCopyCommand(CLCommandQueue* commandQueue,
                                     CLMem* const srcBuffer,
                                     CLMem* const dstBuffer,
                                     const size_t srcOffset,
                                     const size_t dstOffset,
                                     const size_t size)
    : Command(commandQueue),
      srcBuffer(srcBuffer),
      dstBuffer(dstBuffer),
      srcOffset(srcOffset),
      dstOffset(dstOffset),
      size(size) {
    clRetainMemObject(srcBuffer);
    clRetainMemObject(dstBuffer);
}

BufferCopyCommand::~BufferCopyCommand() {
    clReleaseMemObject(srcBuffer);
    clReleaseMemObject(dstBuffer);
}

cl_command_type BufferCopyCommand::getCommandType() {
    return CL_COMMAND_COPY_BUFFER;
}

void BufferCopyCommand::executeImpl() const {
    memcpy(dstBuffer->address + dstOffset, srcBuffer->address + srcOffset,
           size);
}
