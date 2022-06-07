#include <cstring>

#include "Command.h"

BufferReadCommand::BufferReadCommand(CLCommandQueue* commandQueue,
                                     CLMem* buffer,
                                     size_t size,
                                     size_t offset,
                                     void* outputPtr)
    : Command(commandQueue),
      buffer(buffer),
      size(size),
      offset(offset),
      outputPtr(outputPtr) {
    clRetainMemObject(buffer);
}

BufferReadCommand::~BufferReadCommand() {
    clReleaseMemObject(buffer);
}

cl_command_type BufferReadCommand::getCommandType() {
    return CL_COMMAND_READ_BUFFER;
}

void BufferReadCommand::executeImpl() const {
    memcpy(outputPtr, buffer->address + offset, size);
}
