#include "Debugger.h"
#include <iostream>
#include "common/logger/Logger.h"
#include "core/Launch.h"
#include "debugger/message/OnException.h"
#include "debugger/message/OnPause.h"
#include "debugger/message/OnRespondMemory.h"
#include "debugger/message/OnRespondRegisters.h"
#include "debugger/message/RequestMemory.h"
#include "debugger/message/RequestRegisters.h"
#include "debugger/message/Run.h"
#include "debugger/message/SetBreakpoint.h"
#include "debugger/message/Stop.h"
#include "gui/EmulatorApp.h"
#include "ipc/IPCWrapper.h"
#include "src/commons/parser.h"

Debugger::Debugger(EmulatorApp* app) {
    this->app = app;
    messageReaderThread = std::make_unique<std::thread>(&Debugger::processDebuggerMessages, this);
    currentExecutionContext = ExecutionContext();

    auto p = [this](const std::string& s) -> std::string {
        auto proto = IPCWrapper::receiveKernelForExecution(s);
        KernelCode kernelCode = KernelParser::parseKernelCode(proto.instructions);
        KernelConfig config = KernelParser::parseKernelConfig(
            proto.config.dims,
            proto.config.globalWorkOffset,
            proto.config.globalWorkSize,
            proto.config.localWorkSize,
            proto.config.parameters);
        std::vector<KernelArgumentStore> args;
        for (const auto& a : proto.args) {
            KernelArgumentStore ks;
            ks.name = a.name;
            ks.writePermission = a.writePermission;
            ks.readPermission = a.readPermission;
            if (a.kind == ArgumentProto::POINTER) {
                ks.kind = POINTER;
            } else {
                ks.kind = SCALAR;
            }
            ks.size = a.size;
            std::vector<uint8_t> data{a.data.begin(), a.data.end()};
            ks.data = data;
            args.push_back(ks);
        }
        Kernel kernel{
           proto.name,
            std::move(kernelCode),
            std::move(config),
            args
        };
        addKernel(std::move(kernel));
        return s;
    };

    ipcServer = std::make_unique<IpcServer>(p);
    ipcServer->listen();

    app->stopExecution();
}

Debugger::~Debugger() {
    if (emulatorThread) {
        emulatorThread->join();
    }
    if (messageReaderThread) {
        inMessageChannel.close();
        outMessageChannel.close();
        messageReaderThread->join();
    }
};

void Debugger::onSetBreakpoint(uint64_t address) {
    std::unique_lock<std::mutex> lock(mutex);
    auto bp = Breakpoint(address);
    breakpointStorage.addBreakpoint(bp);
    if (!hasKernelRuntime) return;
    outMessageChannel.write(std::make_shared<SetBreakpoint>(bp));
}

void Debugger::onRemoveBreakpoint(uint64_t address) {
    std::unique_lock<std::mutex> lock(mutex);
    auto bp = Breakpoint(address);
    breakpointStorage.removeBreakpoint(bp);
    if (!hasKernelRuntime) return;
    outMessageChannel.write(std::make_shared<SetBreakpoint>(bp, true));
}

void Debugger::onSelectExecutionContext(const ExecutionContext& context) {
    std::unique_lock<std::mutex> lock(mutex);
    changeExecutionContext(context);
}

void Debugger::onSelectKernel(size_t index) {
    if (index == currentKernelIdx)
        return;
    selectKernel(index);
}

void Debugger::onAttach() {
    if (currentKernelIdx == KERNEL_SELECT_NONE)
        return;
    app->startExecution();
    runKernel();
}

void Debugger::onPause(uint64_t address, int workGroupId) {
    // unused
}

void Debugger::onResume() {
    if (!hasKernelRuntime) return;
    outMessageChannel.write(std::make_shared<Run>());
    app->startExecution();
}

void Debugger::onStep() {
    if (!hasKernelRuntime) return;
    outMessageChannel.write(std::make_shared<Run>(true));
    app->startExecution();
}

void Debugger::onStop() {
    if (!hasKernelRuntime) return;
    outMessageChannel.write(std::make_shared<Stop>());
}

void Debugger::onRequestMemory(uint64_t address) {
    if (!hasKernelRuntime) return;
    outMessageChannel.write(std::make_shared<RequestMemory>(address));
}

void Debugger::addKernel(Kernel kernel) {
    programs.emplace_back(std::make_unique<Kernel>(std::move(kernel)));
    selectKernel(currentKernelIdx);
}

void Debugger::runKernel() {
    if (currentKernelIdx == KERNEL_SELECT_NONE) return;

    hasKernelRuntime = true;

    if (emulatorThread) {
        emulatorThread->join();
    }
    emulatorThread = std::make_unique<std::thread>(Executor::executeKernel,
                                                   std::ref(*programs[currentKernelIdx]),
                                                   std::ref(inMessageChannel),
                                                   std::ref(outMessageChannel),
                                                   std::ref(breakpointStorage));
}

void Debugger::processDebuggerMessages() {
    while (!inMessageChannel.isExhausted()) {
        auto messageOptional = inMessageChannel.read();
        if (messageOptional.has_value()) {
            std::shared_ptr<DebuggerMessage> msg = messageOptional.value();
            switch (msg->type) {
                case DebuggerMessageType::ON_STOP:
                    app->stopExecution();
                    break;
                case DebuggerMessageType::ON_PAUSE: {
                    const auto& onPause = std::dynamic_pointer_cast<OnPause>(msg);
                    this->pauseExecution(onPause->context);
                    break;
                }
                case DebuggerMessageType::ON_RES_MEM: {
                    const auto& onResMem = std::dynamic_pointer_cast<OnRespondMemory>(msg);
                    app->setMemoryView(onResMem->data, onResMem->address);
                    break;
                }
                case DebuggerMessageType::ON_RES_REG: {
                    const auto& onResReg =
                        std::dynamic_pointer_cast<OnRespondRegisters>(msg);
                    app->setRegisters(onResReg->data);
                    break;
                }
                case DebuggerMessageType::ON_EXC: {
                    const auto& onExc =
                        std::dynamic_pointer_cast<OnException>(msg);
                    app->setException(onExc->what);
                    break;
                }
                default:
                    throw std::runtime_error("unknown message type");
            }
        }
    }
}

std::string vec_to_str(const std::vector<size_t>& vec) {
    if (vec.empty())
        return "";

    std::ostringstream oss;
    std::copy(vec.begin(), vec.end() - 1,
              std::ostream_iterator<int>(oss, ","));
    oss << vec.back();

    return oss.str();
}

void Debugger::selectKernel(int kernelIdx) {
    std::unique_lock<std::mutex> lock(mutex);
    currentKernelIdx = kernelIdx;
    std::vector<std::string> kernels;
    for (const auto& p : programs) {
        kernels.push_back(p->name);
    }
    app->setKernelList(kernels, currentKernelIdx);

    if (kernelIdx == KERNEL_SELECT_NONE)
        return;

    auto kernel = programs[currentKernelIdx].get();

    std::vector<InstructionView> instructions;
    for (const auto& i : kernel->code.code) {
        instructions.emplace_back(i.first, i.second->get_original_line(), false);
    }
    for (const auto& i : kernel->code.labels) {
        instructions.emplace_back(i.first, i.second->name, true);
    }
    std::sort(instructions.begin(), instructions.end(),
              [](const InstructionView& a, const InstructionView& b) {
                  return a.address < b.address || a.address == b.address && a.isLabel;
              });

    app->setInstructions(instructions);

    std::vector<Parameter> parameters;
    auto config = kernel->config;
    parameters.emplace_back("dims", std::to_string(config.dims));

    if (!config.globalWorkOffset.empty()) {
        parameters.emplace_back("globalWorkOffset", vec_to_str(config.globalWorkOffset));
    }
    if (!config.globalWorkSize.empty()) {
        parameters.emplace_back("globalWorkSize", vec_to_str(config.globalWorkSize));
    }
    if (!config.localWorkSize.empty()) {
        parameters.emplace_back("localWorkSize", vec_to_str(config.localWorkSize));
    }

    parameters.emplace_back("sgprsnum", std::to_string(config.sgprsnum));
    parameters.emplace_back("vgprsnum", std::to_string(config.vgprsnum));
    parameters.emplace_back("pgmrsrc2", std::to_string(config.pgmrsrc2));
    parameters.emplace_back("priority", std::to_string(config.priority));

    if (config.dx10clamp) {
        parameters.emplace_back("dx10clamp", "true");
    }
    if (config.ieeemode) {
        parameters.emplace_back("ieeemode", "true");
    }
    if (config.useargs) {
        parameters.emplace_back("useargs", "true");
    }
    if (config.usesetup) {
        parameters.emplace_back("usesetup", "true");
    }
    if (config.usegeneric) {
        parameters.emplace_back("usegeneric", "true");
    }

    app->setKernelParameters(parameters);
}

void Debugger::pauseExecution(const ExecutionContext& context) {
    changeExecutionContext(context);
    app->pauseExecution(context);
}

void Debugger::changeExecutionContext(const ExecutionContext& context) {
    currentExecutionContext = context;
    this->requestRegisters(context);
}

void Debugger::requestRegisters(const ExecutionContext& context) {
    if (!hasKernelRuntime) return;
    outMessageChannel.write(std::make_shared<RequestRegisters>(context));
}
