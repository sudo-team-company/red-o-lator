//
// Created by Diana Kudaiberdieva
//

#include "alu.h"

void set_reladdr_and_run(const Instruction& instruction,
                         Wavefront* wf,
                         WfStateSOPP& state,
                         void (*exec)(WfStateSOPP&));

void run_s_branch(WfStateSOPP& state) {
    state.PC = state.RELADDR;
}

void run_s_cbranch_cdbgsys(WfStateSOPP& state) {
    if (state.STATUS->cond_dbg_sys()) {
        state.PC = state.RELADDR;
    }
}

void run_s_cbranch_cdbgsys_and_user(WfStateSOPP& state) {
    if (state.STATUS->cond_dbg_sys() && state.STATUS->cond_dbg_user()) {
        state.PC = state.RELADDR;
    }
}

void run_s_cbranch_cdbgsys_or_user(WfStateSOPP& state) {
    if (state.STATUS->cond_dbg_sys() || state.STATUS->cond_dbg_user()) {
        state.PC = state.RELADDR;
    }
}

void run_s_cbranch_cdbguser(WfStateSOPP& state) {
    if (state.STATUS->cond_dbg_user()) {
        state.PC = state.RELADDR;
    }
}

void run_s_cbranch_execnz(WfStateSOPP& state) {
    if (state.EXEC != 0) state.PC = state.RELADDR;
}

void run_s_cbranch_execz(WfStateSOPP& state) {
    if (state.EXEC == 0) state.PC = state.RELADDR;
}

void run_s_cbranch_scc0(WfStateSOPP& state) {
    if (state.SCC == 0) state.PC = state.RELADDR;
}

void run_s_cbranch_scc1(WfStateSOPP& state) {
    state.PC = (state.SCC == 1) ? state.RELADDR : state.PC;
}
void run_s_cbranch_vccnz(WfStateSOPP& state) {
    state.PC = state.VCC != 0 ? state.RELADDR : state.PC;
}
void run_s_cbranch_vccz(WfStateSOPP& state) {
    state.PC = state.VCC == 0 ? state.RELADDR : state.PC;
}

void run_s_decperflevel(WfStateSOPP& state) {
    // todo
}

void run_s_endpgm_ordered_ps_done(WfStateSOPP& state) {
    // todo
}

void run_s_endpgm_saved(WfStateSOPP& state) {
    // todo
}

void run_s_icache_inv(WfStateSOPP& state) {
    // todo
}

void run_s_incperflevel(WfStateSOPP& state) {
    // todo
}

void run_s_nop(WfStateSOPP& state) {
    //do nothing
}

void run_s_sendmsg(WfStateSOPP& state) {
    // todo
}

void run_s_sendmsghalt(WfStateSOPP& state) {
    // todo
}

void run_s_set_gpr_idx_mode(WfStateSOPP& state) {
    state.M0 = (state.M0 & 0xffff0fff) | ((state.SIMM16 & 15) << 12);
}

void run_s_set_gpr_idx_off(WfStateSOPP& state) {
    state.MODE->gpr_idx_en(0);
}

void run_s_sethalt(WfStateSOPP& state) {
    state.STATUS->halt(state.SIMM16 & 1);
}

void run_s_setkill(WfStateSOPP& state) {
    // todo
}

void run_s_setprio(WfStateSOPP& state) {
    // todo
}

void run_s_sleep(WfStateSOPP& state) {
    // todo
}
void run_s_trap(WfStateSOPP& state) {
    // todo
}

void run_s_ttracedata(WfStateSOPP& state) {
    // todo
}

void run_s_waitcnt(WfStateSOPP& state) {
    // todo
}

void run_sopp(const Instruction& instruction, Wavefront* wavefront) {
    auto instrKey = instruction.get_key();

    auto state = wavefront->get_common_sopp_state(instruction);

    switch (instrKey) {
        case S_BARRIER:
        case S_ENDPGM:
            assert("unexpected instruction in SOPP");
            break;

        case S_BRANCH:
            set_reladdr_and_run(instruction, wavefront, state, run_s_branch);
            break;
        case S_CBRANCH_CDBGSYS:
            set_reladdr_and_run(instruction, wavefront, state, run_s_cbranch_cdbgsys);
            break;
        case S_CBRANCH_CDBGSYS_AND_USER:
            set_reladdr_and_run(instruction, wavefront, state,
                                run_s_cbranch_cdbgsys_and_user);
            break;
        case S_CBRANCH_CDBGSYS_OR_USER:
            set_reladdr_and_run(instruction, wavefront, state,
                                run_s_cbranch_cdbgsys_or_user);
            break;
        case S_CBRANCH_CDBGUSER:
            set_reladdr_and_run(instruction, wavefront, state, run_s_cbranch_cdbguser);
            break;
        case S_CBRANCH_EXECNZ:
            set_reladdr_and_run(instruction, wavefront, state, run_s_cbranch_execnz);
            break;
        case S_CBRANCH_EXECZ:
            set_reladdr_and_run(instruction, wavefront, state, run_s_cbranch_execz);
            break;
        case S_CBRANCH_SCC0:
            set_reladdr_and_run(instruction, wavefront, state, run_s_cbranch_scc0);
            break;
        case S_CBRANCH_SCC1:
            set_reladdr_and_run(instruction, wavefront, state, run_s_cbranch_scc1);
            break;
        case S_CBRANCH_VCCNZ:
            set_reladdr_and_run(instruction, wavefront, state, run_s_cbranch_vccnz);
            break;
        case S_CBRANCH_VCCZ:
            set_reladdr_and_run(instruction, wavefront, state, run_s_cbranch_vccz);
            break;

        case S_DECPERFLEVEL:
            run_s_decperflevel(state);
            break;
        case S_ENDPGM_ORDERED_PS_DONE:
            run_s_endpgm_ordered_ps_done(state);
            break;
        case S_ENDPGM_SAVED:
            run_s_endpgm_saved(state);
            break;
        case S_ICACHE_INV:
            run_s_icache_inv(state);
            break;
        case S_INCPERFLEVEL:
            run_s_incperflevel(state);
            break;
        case S_NOP:
            run_s_nop(state);
            break;
        case S_SENDMSG:
            run_s_sendmsg(state);
            break;
        case S_SENDMSGHALT:
            run_s_sendmsghalt(state);
            break;
        case S_SET_GPR_IDX_MODE:
            run_s_set_gpr_idx_mode(state);
            break;
        case S_SET_GPR_IDX_OFF:
            run_s_set_gpr_idx_off(state);
            break;
        case S_SETHALT:
            run_s_sethalt(state);
            break;
        case S_SETKILL:
            run_s_setkill(state);
            break;
        case S_SETPRIO:
            run_s_setprio(state);
            break;
        case S_SLEEP:
            run_s_sleep(state);
            break;
        case S_TRAP:
            run_s_trap(state);
            break;
        case S_TTRACEDATA:
            run_s_ttracedata(state);
            break;
        case S_WAITCNT:
            run_s_waitcnt(state);
            break;
        default:
            assert(false && "Unknown instruction met!");
            throw std::runtime_error(std::string("Unexpected instruction key: ") +
                                     get_instr_str(instrKey));
    }

    wavefront->update_with_common_sopp_state(instruction, state);
}

void set_reladdr_and_run(const Instruction& instruction,
                         Wavefront* wf,
                         WfStateSOPP& state,
                         void (*exec)(WfStateSOPP&)) {
    state.RELADDR = to_uin64_t(wf->read_operand(*instruction[0]));
    exec(state);
}
