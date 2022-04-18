#pragma once

/** It is important that rename-api.h is included before <opencl.h> to rename
 *  OpenCL API entries to avoid clash with icd functions.
 *
 *  For that reason, this file should be included instead of <opencl.h> in
 *  outer files.
 */
#include "rename-api.h"

#include <common/cl.h>

#include "IcdDispatchTable.h"
