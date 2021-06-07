#include <cstring>
#include "Command.h"
#include "icd/CLKernel.h"

KernelExecutionCommand::KernelExecutionCommand(CLCommandQueue* commandQueue,
                                               CLKernel* kernel,
                                               cl_uint workDim,
                                               const size_t* globalWorkOffset,
                                               const size_t* globalWorkSize,
                                               const size_t* localWorkSize)
    : Command(commandQueue),
      kernel(kernel),
      workDim(workDim),
      globalWorkOffset(globalWorkOffset),
      globalWorkSize(globalWorkSize),
      localWorkSize(localWorkSize) {
    clRetainKernel(kernel);
}

KernelExecutionCommand::~KernelExecutionCommand() {
    clReleaseKernel(kernel);
}

cl_command_type KernelExecutionCommand::getCommandType() {
    return CL_COMMAND_NDRANGE_KERNEL;
}

void KernelExecutionCommand::executeImpl() const {
    executeCommand(*this);
}
