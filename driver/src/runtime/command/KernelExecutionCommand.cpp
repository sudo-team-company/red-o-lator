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

//    for (const auto& arg : kernel->getArguments()) {
//        if (std::holds_alternative<CLMem*>(arg.value.value().value)) {
//            const auto argInfo =
//                std::dynamic_pointer_cast<PointerKernelArgumentInfo>(arg.info);
//            const auto argValue = std::get<CLMem*>(arg.value.value().value);
//            if (argInfo->accessQualifier == CL_KERNEL_ARG_ACCESS_READ_WRITE ||
//                argInfo->accessQualifier == CL_KERNEL_ARG_ACCESS_WRITE_ONLY) {
//                memset(argValue->address, 0, argValue->size);
//            }
//        }
//    }
}
