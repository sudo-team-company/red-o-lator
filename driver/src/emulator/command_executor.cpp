#include "command_executor.h"
#include "commons/commons.h"
#include "flow/kernel_config.h"
#include "flow/wavefront.h"
#include "gpu/compute_unit.h"
#include "gpu/dispatcher.h"
#include "gpu/storage.h"
#include "instr/instruction.h"
#include "runtime/icd/kernel/CLKernel.h"

void writeArg(uint64_t&, size_t&, const KernelArgument&);
void readArgs(size_t, const KernelArgument&);

size_t evaluateArgSize(const std::vector<KernelArgument>& args);

void executeCommand(const KernelExecutionCommand& command) {
    CLKernel* kernel = command.kernel;
    KernelConfig kernelConfig(command.workDim, command.globalWorkOffset,
                              command.globalWorkSize, command.localWorkSize,
                              kernel->config);
    auto storage = Storage::get_instance();
    storage->init(10000000);
    uint64_t addr = 0;
    storage->write_data(addr, 0, uint64_t(kernelConfig.get_X_offset()));
    storage->write_data(addr, 8, uint64_t(kernelConfig.get_Y_offset()));
    storage->write_data(addr, 16, uint64_t(kernelConfig.get_Z_offset()));
    addr = 8 * 6;
    size_t offsetForValues = evaluateArgSize(kernel->getArguments());
    std::vector<size_t> argAddresses;
    for (const auto& arg : kernel->getArguments()) {
        argAddresses.push_back(addr);
        writeArg(addr, offsetForValues, arg);
    }
    KernelCode kernelCode(kernel->instructions);
    Dispatcher dispatcher(&kernelConfig, &kernelCode);
    while (dispatcher.has_next_wg()) {
        std::unique_ptr<WorkGroup> workGroup(dispatcher.next_wg());
        ComputeUnit::run_work_group(workGroup.get());
    }
    for (size_t i = 0; i < kernel->getArguments().size(); ++i) {
        readArgs(argAddresses[i], kernel->getArguments()[i]);
    }
}

size_t evaluateArgSize(const std::vector<KernelArgument>& args) {
    size_t result = 0;
    for (const auto& arg : args) {
        auto info = arg.info.get();
        if (!arg.value.has_value()) continue;
        switch (info->kind) {
            case POINTER: result += 8; break;
            case SCALAR: result += arg.value->size; break;
            default:
                throw std::runtime_error("Unsupported argument type: " +
                                         info->argumentName);
        }
    }
    return result;
}

void writeArg(uint64_t& addr, size_t& offsetForValue, const KernelArgument& arg) {
    if (!arg.value.has_value()) {
        logger.debug(std::string("Kernel argument ") + arg.info->argumentName +
                     " has no value to read");
        return;
    }
    auto storage = Storage::get_instance();
    auto info = arg.info.get();
    if (std::holds_alternative<CLMem*>(arg.value.value().value)) {
        auto value = std::get<CLMem*>(arg.value.value().value);
        switch (info->kind) {
            case POINTER:
                storage->write_data(addr, 0, uint64_t(addr) + offsetForValue);
                if (value->kernelCanRead) {  // write to the gpu storage data that
                    for (size_t i = 0; i < value->size; ++i) {
                        auto data = uint8_t(value->address[i]);
                        storage->write_data(addr, offsetForValue, data);
                        offsetForValue += 1;
                    }
                }
                addr += 8;
                break;
            case SCALAR:
                for (size_t i = 0; i < value->size; ++i) {
                    auto data = uint8_t(value->address[i]);
                    storage->write_data(addr, offsetForValue, data);
                    offsetForValue += 1;
                }
                break;
            default:
                throw std::runtime_error("Unsupported argument type: " +
                                         info->argumentName);
        }
    } else {
        assert(false && "Unsupported argument kind");
    }
}

void readArgs(size_t addr, const KernelArgument& arg) {
    if (!arg.value.has_value()) {
        logger.debug(std::string("Kernel argument ") + arg.info->argumentName +
                     " has no value to write into");
        return;
    }
    auto storage = Storage::get_instance();
    auto info = arg.info.get();
    if (std::holds_alternative<CLMem*>(arg.value.value().value)) {
        auto value = std::get<CLMem*>(arg.value.value().value);

        switch (info->kind) {
            case SCALAR: assert(false && "todo");
            case POINTER: {
                if (!value->kernelCanWrite) {
                    return;
                }
                auto realAddr = storage->read_8_bytes(addr);
                std::vector<uint8_t> data = storage->read_data(realAddr, 0, value->size);
                memcpy(value->address, data.data(), data.size());
                break;
            }
            default:
                throw std::runtime_error("Unsupported argument type: " +
                                         info->argumentName);
        }
    } else {
        assert(false && "Unsupported argument kind");
    }
}