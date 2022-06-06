#include "compute_unit.h"

void ComputeUnit::run_work_group(WorkGroup* wg, DebugContext& debug) {
    debug.setWorkGroup(wg); // TODO: come up with something smarter
    while (!wg->all_wf_completed()) {
        for (auto& wavefront : wg->wavefronts) {
            auto* wf = wavefront.get();
            run_wavefront(wf, debug);
        }
        resolve_barrier(wg);
    }
}

void ComputeUnit::run_wavefront(Wavefront* wavefront, DebugContext& debug) {
    ExecutionContext ctx = debug.createContext(0, wavefront);
    while (true) {
        Instruction* curInstr = wavefront->get_cur_instr();
        InstrKey instrKey = curInstr->get_key();
        ctx.address = curInstr->get_addr();

        debug.processInstruction(ctx);

        if (instrKey == S_ENDPGM) {
            wavefront->compete();
            break;
        }

        if (instrKey == S_BARRIER) {
            wavefront->atBarrier = true;
            break;
        }
        if (instrKey != S_WAITCNT) {  // S_WAITCNT is always ignored
            try {
                run_instr(*curInstr, wavefront);
            } catch(std::runtime_error& e) {
                logger.error(e.what());
                debug.processException(std::string{e.what()}, ctx);
            }
        }
        wavefront->to_next_instr();
    }
}

void ComputeUnit::resolve_barrier(WorkGroup* wg) {
    if (wg->all_wf_completed()) return;
    for (auto& wavefront : wg->wavefronts) {
        auto* wf = wavefront.get();
        if (!wf->atBarrier)assert(false && "Illegal wavefront state");
        wf->atBarrier = false;
    }
}
