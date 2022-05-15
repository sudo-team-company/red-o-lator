#include <cstring>
#include "Command.h"
#include "runtime/icd/kernel/CLKernel.h"
#include "emulator/command_executor.h"

KernelExecutionCommand::KernelExecutionCommand(CLKernel* kernel,
                                               cl_uint workDim,
                                               const size_t* globalWorkOffset,
                                               const size_t* globalWorkSize,
                                               const size_t* localWorkSize)
    : kernel(kernel),
      workDim(workDim),
      globalWorkOffset(globalWorkOffset),
      globalWorkSize(globalWorkSize),
      localWorkSize(localWorkSize) {
    clRetainKernel(kernel);
}

KernelExecutionCommand::~KernelExecutionCommand() {
    clReleaseKernel(kernel);
}

void KernelExecutionCommand::execute() const {
    executeCommand(*this);
}
