#include <cstring>

#include "Command.h"

BufferWriteCommand::BufferWriteCommand(CLMem* buffer,
                                       size_t size,
                                       size_t offset,
                                       const void* dataPtr)
    : buffer(buffer), size(size), offset(offset), dataPtr(dataPtr) {
    clRetainMemObject(buffer);
}

BufferWriteCommand::~BufferWriteCommand() {
    clReleaseMemObject(buffer);
}

void BufferWriteCommand::execute() const {
    memcpy(buffer->address + offset, dataPtr, size);
}
