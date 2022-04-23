#include <cstring>

#include "Command.h"

BufferWriteCommand::BufferWriteCommand(CLCommandQueue* commandQueue,CLMem* buffer,
                                       size_t size,
                                       size_t offset,
                                       const void* dataPtr)
    : Command(commandQueue),
      buffer(buffer),
      size(size),
      offset(offset),
      dataPtr(dataPtr) {
    clRetainMemObject(buffer);
}

BufferWriteCommand::~BufferWriteCommand() {
    clReleaseMemObject(buffer);
}

cl_command_type BufferWriteCommand::getCommandType() {
    return CL_COMMAND_WRITE_BUFFER;
}

void BufferWriteCommand::executeImpl() const {
    memcpy(buffer->address + offset, dataPtr, size);
}
