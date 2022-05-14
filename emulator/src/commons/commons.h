#pragma once

#include "common/logger/Logger.h"
#include <cassert>

extern Logger logger;

#define UNSUPPORTED_INSTRUCTION(instrType, instrKey) \
do {                                                 \
    logger.error(std::string("Unsupported ") + (instrType) + " instruction: "+ (instrKey)); \
    assert(false && "Can not execute kernel due to unsupported instruction"); \
} while(false)