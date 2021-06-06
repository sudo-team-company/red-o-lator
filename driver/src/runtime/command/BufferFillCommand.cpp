#include <cstring>
#include "Command.h"

BufferFillCommand::BufferFillCommand(CLCommandQueue* commandQueue,
                                     CLMem* buffer,
                                     const void* pattern,
                                     size_t patternSize,
                                     size_t offset,
                                     size_t size)
    : Command(commandQueue),
      buffer(buffer),
      pattern(new std::byte[patternSize]),
      patternSize(patternSize),
      offset(offset),
      size(size) {
    memcpy(this->pattern, reinterpret_cast<const std::byte*>(pattern),
           patternSize);
    clRetainMemObject(buffer);
}

BufferFillCommand::~BufferFillCommand() {
    clReleaseMemObject(buffer);
    delete[] pattern;
}

cl_command_type BufferFillCommand::getCommandType() {
    return CL_COMMAND_FILL_BUFFER;
}

void BufferFillCommand::executeImpl() const {
    for (int i = 0; i < size / patternSize; ++i) {
        memcpy(buffer->address + offset + i * patternSize, pattern,
               patternSize);
    }
}
