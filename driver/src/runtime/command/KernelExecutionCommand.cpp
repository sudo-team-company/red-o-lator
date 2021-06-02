#include <cstring>
#include "Command.h"
#include "runtime/icd/kernel/CLKernel.h"

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
    for (const auto& arg : kernel->getArguments()) {
        const auto uncastedArgValue = arg.value.value().value;
        if (auto argInfo = std::dynamic_pointer_cast<PointerKernelArgumentInfo>(
                arg.info)) {
            if (argInfo->accessQualifier == CL_KERNEL_ARG_ACCESS_READ_WRITE ||
                argInfo->accessQualifier == CL_KERNEL_ARG_ACCESS_WRITE_ONLY) {
                if (std::holds_alternative<CLMem*>(uncastedArgValue)) {
                    const auto argValue = std::get<CLMem*>(uncastedArgValue);
                    memset(argValue->address, 0, argValue->size);
                }
            }
        }
    }
}
