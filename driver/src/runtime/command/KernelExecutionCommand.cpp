#include <cstring>
#include "Command.h"
#include "icd/CLKernel.h"
#include "ipc/IPCWrapper.h"
#include "ipc/IpcClient.h"
#include "kernel_request.pb.h"

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
    KernelProto proto;
    proto.name = kernel->name;
    proto.instructions = kernel->instructions;
    proto.config.dims = workDim;
    if (globalWorkOffset) {
        for (size_t i = 0; i < workDim; i++) {
            proto.config.globalWorkOffset.push_back(globalWorkOffset[i]);
        }
    } else {
        proto.config.globalWorkOffset = {};
    }
    if (globalWorkSize) {
        for (size_t i = 0; i < workDim; i++) {
            proto.config.globalWorkSize.push_back(globalWorkSize[i]);
        }
    } else {
        proto.config.globalWorkSize = {};
    }
    if (localWorkSize) {
        for (size_t i = 0; i < workDim; i++) {
            proto.config.localWorkSize.push_back(localWorkSize[i]);
        }
    } else {
        proto.config.localWorkSize = {};
    }
    proto.config.parameters = kernel->config;

    for (const auto& arg : kernel->getArguments()) {
        const auto uncastedArgValue = arg.value.value().value;
        if (auto argInfo = std::dynamic_pointer_cast<PointerKernelArgumentInfo>(
                arg.info)) {
            if (std::holds_alternative<CLMem*>(uncastedArgValue)) {
                ArgumentProto a;
                a.name = arg.info->argumentName;
                a.kind = ArgumentProto::POINTER;
                const auto argValue = std::get<CLMem*>(uncastedArgValue);
                a.size = argValue->size;
                std::string tmp;
                tmp.resize(a.size);
                memcpy(tmp.data(), argValue->address, a.size);
                a.data = tmp;
                a.readPermission = argValue->kernelCanRead;
                a.writePermission = argValue->kernelCanWrite;
                proto.args.push_back(a);
            }
        }
    }


    IPCWrapper::sendKernelForExecution(proto);
}
