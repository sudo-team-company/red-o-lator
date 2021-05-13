#include <runtime-commons.h>
#include <cstring>

#include "Command.h"

BufferWriteCommand::~BufferWriteCommand() {
    kLogger.log("Write destructor");

    clReleaseMemObject(buffer);
}

void BufferWriteCommand::execute() const {
    memcpy(buffer->address + offset, dataPtr, size);
    kLogger.log("Write");
    kLogger.log(std::to_string(offset));
}
