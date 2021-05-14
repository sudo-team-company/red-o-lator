#include <runtime-commons.h>
#include <cstring>

#include "Command.h"

BufferWriteCommand::~BufferWriteCommand() {
    clReleaseMemObject(buffer);
}

void BufferWriteCommand::execute() const {
    memcpy(buffer->address + offset, dataPtr, size);
    kLogger.debug(std::to_string(offset));
}
