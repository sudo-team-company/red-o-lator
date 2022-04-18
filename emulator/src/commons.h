#pragma once

#include <common/logger/Logger.h>
#include <cassert>

extern Logger logger;

static inline void unsupported_instruction(const char* const instrType, const char* const instrKey) {
    logger.error(std::string("Unsupported ") + instrType + " instruction: "+ instrKey);
    assert(false && "Unsupported instruction");
}
