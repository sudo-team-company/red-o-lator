#include <runtime-commons.h>
#include <cstring>

#include "Command.h"

BufferReadCommand::~BufferReadCommand() {
    clReleaseMemObject(buffer);
}

void BufferReadCommand::execute() const {
    memcpy(outputPtr, buffer->address + offset, size);
}
