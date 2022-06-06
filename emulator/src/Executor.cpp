#include "Executor.h"

#include <cassert>
#include <stdexcept>
#include <variant>
#include <vector>
#include "commons/commons.h"
#include "debugger/message/OnStop.h"
#include "flow/debug_context.h"
#include "flow/wavefront.h"
#include "gpu/compute_unit.h"
#include "gpu/dispatcher.h"
#include "gpu/storage.h"
#include "model/kernel/arg/KernelArgumentStore.h"

void Executor::executeKernel(Kernel& kernel,
                             Channel<std::shared_ptr<DebuggerMessage>>& outMessageChannel,
                             Channel<std::shared_ptr<DebuggerMessage>>& inMessageChannel,
                             const BreakpointStorage& breakpointStorage) {
    const KernelConfig& kernelConfig = kernel.config;
    auto storage = Storage::get_instance();

    storage->init(10000000);
    uint64_t addr = 0;
    storage->write_data(addr, 0, uint64_t(kernelConfig.get_X_offset()));
    storage->write_data(addr, 8, uint64_t(kernelConfig.get_Y_offset()));
    storage->write_data(addr, 16, uint64_t(kernelConfig.get_Z_offset()));
    addr = 8 * 6;
    size_t offsetForValues = evaluateArgSize(kernel.arguments);
    std::vector<size_t> argAddresses;
    for (const auto& arg : kernel.arguments) {
        argAddresses.push_back(addr);
        writeArg(addr, offsetForValues, arg);
    }
    Dispatcher dispatcher(&kernelConfig, &kernel.code);
    DebugContext debug{&outMessageChannel, &inMessageChannel, breakpointStorage};
    while (dispatcher.has_next_wg()) {
        std::unique_ptr<WorkGroup> workGroup(dispatcher.next_wg());
        ComputeUnit::run_work_group(workGroup.get(), debug);
    }
    for (size_t i = 0; i < kernel.arguments.size(); ++i) {
        readArgs(argAddresses[i], kernel.arguments[i]);
    }
    if (debug.executionMode == ExecutionMode::STOP) {
        outMessageChannel.write(std::make_shared<OnStop>());
    }
}

size_t Executor::evaluateArgSize(const std::vector<KernelArgumentStore>& args) {
    size_t result = 0;
    for (const auto& arg : args) {
        if (!arg.data.has_value()) continue;

        switch (arg.kind) {
            case POINTER: result += 8; break;
            case SCALAR: result += arg.size; break;
            default: throw std::runtime_error("Unsupported argument type: " + arg.name);
        }
    }
    return result;
}

void Executor::writeArg(uint64_t& addr,
                        size_t& offsetForValue,
                        const KernelArgumentStore& arg) {
    if (!arg.data.has_value()) {
        logger.debug(std::string("Kernel argument ") + arg.name +
                     " has no value to read");
        return;
    }
    auto storage = Storage::get_instance();
    const auto* bytes = reinterpret_cast<const std::byte*>(arg.data.value().data());
    switch (arg.kind) {
        case POINTER:
            storage->write_data(addr, 0, uint64_t(addr) + offsetForValue);
            if (arg.readPermission) {  // write to the gpu storage data that
                for (size_t i = 0; i < arg.size; ++i) {
                    auto data = uint8_t(bytes[i]);
                    storage->write_data(addr, offsetForValue, data);
                    offsetForValue += 1;
                }
            }
            addr += 8;
            break;
        case SCALAR:
            for (size_t i = 0; i < arg.size; ++i) {
                auto data = uint8_t(bytes[i]);
                storage->write_data(addr, offsetForValue, data);
                offsetForValue += 1;
            }
            break;
        default: throw std::runtime_error("Unsupported argument type: " + arg.name);
    }
}

void Executor::readArgs(size_t addr, KernelArgumentStore& arg) {
    if (!arg.data.has_value()) {
        logger.debug(std::string("Kernel argument ") + arg.name +
                     " has no value to write into");
        return;
    }
    auto storage = Storage::get_instance();
    switch (arg.kind) {
        case SCALAR: assert(false && "todo");
        case POINTER: {
            if (!arg.writePermission) {
                return;
            }
            auto realAddr = storage->read_8_bytes(addr);
            std::vector<uint8_t> data = storage->read_data(realAddr, 0, arg.size);
            arg.output = data;
            break;
        }
        default: throw std::runtime_error("Unsupported argument type: " + arg.name);
    }
}
