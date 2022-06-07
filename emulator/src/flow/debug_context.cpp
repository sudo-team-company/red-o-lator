#include "debug_context.h"

#include <utility>
#include "debugger/message/OnException.h"
#include "debugger/message/OnPause.h"
#include "debugger/message/OnRespondMemory.h"
#include "debugger/message/OnRespondRegisters.h"
#include "debugger/message/OnStop.h"
#include "debugger/message/RequestMemory.h"
#include "debugger/message/RequestRegisters.h"
#include "debugger/message/Run.h"
#include "debugger/message/SetBreakpoint.h"
#include "gpu/storage.h"

DebugContext::DebugContext(Channel<std::shared_ptr<DebuggerMessage>>* outMessageChannel,
                           Channel<std::shared_ptr<DebuggerMessage>>* inMessageChannel,
                           BreakpointStorage breakpoints)
    : outMessageChannel{outMessageChannel},
      inMessageChannel{inMessageChannel},
      breakpoints{std::move(breakpoints)} {}


void DebugContext::setWorkGroup(WorkGroup* wg) {
    currentWorkGroup = wg;
}

void DebugContext::readMessages() {
    while (!inMessageChannel->isExhausted()) {
        std::optional<std::shared_ptr<DebuggerMessage>> msg;
        msg = inMessageChannel->tryRead();
        if (!msg.has_value()) return;
        processMessage(msg.value());
    }
}

void DebugContext::waitForResume() {
    while (!inMessageChannel->isExhausted() &&
           executionMode == ExecutionMode::PAUSE) {
        std::optional<std::shared_ptr<DebuggerMessage>> msg;
        msg = inMessageChannel->read();
        if (!msg.has_value()) return;
        processMessage(msg.value());
    }
    readMessages();
}

void DebugContext::processMessage(const std::shared_ptr<DebuggerMessage>& msg) {
    switch (msg->type) {
        case DebuggerMessageType::REQ_MEM: {
            const auto& req = std::dynamic_pointer_cast<RequestMemory>(msg);
            std::vector<uint8_t> mem =
                Storage::get_instance()->read_data(req->address, 0, 512, true);
            outMessageChannel->write(std::make_shared<OnRespondMemory>(req->address, mem));
            break;
        }
        case DebuggerMessageType::REQ_REG: {
            const auto& req = std::dynamic_pointer_cast<RequestRegisters>(msg);
            RegData data = getRegisters(req->context);
            outMessageChannel->write(std::make_shared<OnRespondRegisters>(data));
            break;
        }
        case DebuggerMessageType::SET_BREAKPOINT: {
            if (isTerminal())
                break;
            const auto& bp = std::dynamic_pointer_cast<SetBreakpoint>(msg);
            if (bp->remove) {
                breakpoints.removeBreakpoint(bp->breakpoint);
            } else {
                breakpoints.addBreakpoint(bp->breakpoint);
            }
            break;
        }
        case DebuggerMessageType::RUN: {
            if (isTerminal())
                break;
            const auto& run = std::dynamic_pointer_cast<Run>(msg);
            if (run->stepping) {
                executionMode = ExecutionMode::STEP;
            } else {
                executionMode = ExecutionMode::RUN;
            }
            break;
        }
        case DebuggerMessageType::STOP:
            executionMode = ExecutionMode::STOP;
            break;
    }
}

void DebugContext::processInstruction(const ExecutionContext& context) {
    if (isTerminal())
        return;

    if (executionMode == ExecutionMode::STEP ||
        breakpoints.hasBreakpoint(context.address)) {
        executionMode = ExecutionMode::PAUSE;
        outMessageChannel->write(std::make_shared<OnPause>(context));
        waitForResume();
    }
}

void DebugContext::processException(const std::string& what, const ExecutionContext& context) {
    executionMode = ExecutionMode::EXC;
    outMessageChannel->write(std::make_shared<OnException>(what));
}

RegData DebugContext::getRegisters(const ExecutionContext& context) {
    const auto& wf = currentWorkGroup->wavefronts[context.wavefrontId];

    const auto& sopp = wf->get_common_sopp_state();

    size_t wiInd = 0;
    const auto& wis = currentWorkGroup->workItems;
    for (size_t i = 0; i < wis.size(); i++) { // TODO: optimize
        const auto& wi = wis[i];
        if (context.workItemX == wi->get_local_id_x() &&
            context.workItemY == wi->get_local_id_y() &&
            context.workItemZ == wi->get_local_id_z()) {
            wiInd = i;
            break;
        }
    }
    size_t vStart = wiInd * wf->vgprsnum;
    size_t vEnd = vStart + wf->vgprsnum;

    RegData result;
    result.sFile = wf->scalarRegFile;
    result.vFile = {wf->vectorRegFile.begin() + vStart, wf->vectorRegFile.begin() + vEnd};

    result.RELADDR = sopp.RELADDR;
    result.EXEC = sopp.EXEC;
    result.VCC = sopp.VCC;
    result.M0 = sopp.M0;
    result.SIMM16 = sopp.SIMM16;
    result.SCC = sopp.SCC;

    return result;
}

ExecutionContext DebugContext::createContext(uint32_t address, Wavefront* wf) {
    const WorkGroup* wg = wf->workGroup;

    ExecutionContext context;
    context.info.wfSize = wg->wavefronts.size();
    context.info.sizeX = wg->sizeX;
    context.info.sizeY = wg->sizeY;
    context.info.sizeZ = wg->sizeZ;

    context.wavefrontId = wf->get_local_id();
    context.address = address;

    WorkItem& wi = wg->get_workitem_for_wf(0, wf->get_local_id());
    context.workItemX = wi.get_local_id_x();
    context.workItemY = wi.get_local_id_y();
    context.workItemZ = wi.get_local_id_z();

    return context;

}

bool DebugContext::isTerminal() {
    return executionMode == ExecutionMode::STOP || executionMode == ExecutionMode::EXC;
}
