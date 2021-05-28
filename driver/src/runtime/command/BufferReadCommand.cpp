#include <cstring>

#include "Command.h"

BufferReadCommand::BufferReadCommand(CLMem* buffer,
                                     size_t size,
                                     size_t offset,
                                     void* outputPtr)
    : buffer(buffer), size(size), offset(offset), outputPtr(outputPtr) {
    clRetainMemObject(buffer);
}

BufferReadCommand::~BufferReadCommand() {
    clReleaseMemObject(buffer);
}

void BufferReadCommand::execute() const {
    memcpy(outputPtr, buffer->address + offset, size);
}
