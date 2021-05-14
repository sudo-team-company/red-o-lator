#pragma once

#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 120
#endif

/** It is important that rename-api.h is included before <opencl.h> to rename
 *  OpenCL API entries to avoid clash with icd functions.
 *
 *  For that reason, this file should be included instead of <opencl.h> in
 *  outer files.
 */
#include "rename-api.h"

#include <CL/opencl.h>

#include "IcdDispatchTable.h"

//#include "CLPlatformId.hpp"
//#include "CLDeviceId.hpp"
//#include "CLContext.h"
//#include "CLCommandQueue.h"
//#include "CLProgram.hpp"
//#include "CLMem.hpp"
//#include "CLKernel.h"
//#include "CLEvent.hpp"
//#include "CLSampler.hpp"
