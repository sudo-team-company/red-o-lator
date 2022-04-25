#include "command_executor.h"
#include "runtime/icd/kernel/CLKernel.h"
#include "src/flow/kernel_config.h" //todo

void executeCommand(const KernelExecutionCommand &command) {
    auto kernel = command.kernel;
    auto kernelConfig = KernelConfig(
        command.workDim,
        command.globalWorkOffset,
        command.globalWorkSize,
        command.localWorkSize,
        command.kernel->config
    );
    //todo move arguments data to "storage"

}