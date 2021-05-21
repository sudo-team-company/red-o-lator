//
// Created by Diana Kudaiberdieva
//

#include "wavefront.h"

void WorkGroup::set_ids(int idX, int idY, int idZ) {
    IDX = idX;
    IDY = idY;
    IDZ = idZ;
}
void WorkGroup::set_actual_size(int x, int y, int z) {
    actualSizeX_ = x;
    actualSizeY_ = y;
    actualSizeZ_ = z;
}
bool WorkGroup::all_wf_completed() {
    for (auto& wavefront : wavefronts) {
        if (!wavefront->completed) {
            return false;
        }
    }
    return true;
}
Instruction* Wavefront::get_cur_instr() const {
    return workGroup->kernelCode->get_instr(this->programCounter->get_value());
}

void Wavefront::to_next_instr() {
    if (programCounter->was_used()) return;
    auto curInstrKey =  get_cur_instr()->get_key();
    programCounter->hidden_add(get_instr_width(curInstrKey) / 8);
}

void Wavefront::set_vgpr(size_t wiInd, size_t vInd, uint32_t value) {
    size_t ind = wiInd * vgprsnum + vInd;
    assert(ind >= 0 && ind < vectorRegFile.size() && "Vector register is out of range");
    vectorRegFile[ind] = value;
}

uint32_t Wavefront::read_vgpr(size_t wiInd, size_t vInd) {
    size_t ind = wiInd * vgprsnum + vInd;
    assert(ind >= 0 && ind < vectorRegFile.size() && "Vector register is out of range");
    return vectorRegFile[ind];
}

WfStateSOP1 Wavefront::get_sop1_state(const Instruction& instr) {
    assert(instr.get_operands_count() <= 2 && "Unexpected operands count for SOP1");

    auto state = WfStateSOP1{execReg,       programCounter.get(),  m0Reg,
                             modeReg.get(), statusReg.get(), sccReg};
    if (instr.get_operands_count() < 2) {
        return state;
    }

    state.SDST = to_uin64_t(read_operand(*instr[0]));
    state.SSRC0 = to_uin64_t(read_operand(*instr[1]));
    state.DEST_TYPE = std::get<RegisterType>(instr[0]->value);
    state.SRC0_TYPE = std::get<RegisterType>(instr[1]->value);
    return state;
}

void Wavefront::update_with_sop1_state(const Instruction& instr,
                                       const WfStateSOP1& state) {
    assert(instr.get_operands_count() <= 2 && "Unexpected operands count for SOP1");

    execReg = state.EXEC;
    m0Reg = state.M0;
    sccReg = state.SCC;

    if (instr.get_operands_count() < 2) {
        return;
    }

    write_operand_to_gpr(*instr[0], state.SDST);
}

WfStateSOP2 Wavefront::get_sop2_state(const Instruction& instruction) {
    assert(instruction.get_operands_count() <= 3 && "Unexpected operands count for SOP2");

    auto state =
        WfStateSOP2(execReg, programCounter.get(), modeReg.get(), statusReg.get(), sccReg);

    if (instruction.get_operands_count() < 3) {
        return state;
    }
    state.SDST = to_uin64_t(read_operand(*instruction[0]));
    state.SSRC0 = to_uin64_t(read_operand(*instruction[1]));
    state.SSRC1 = to_uin64_t(read_operand(*instruction[2]));
    return state;
}

void Wavefront::update_with_sop2_state(const Instruction& instruction,
                                       const WfStateSOP2& state) {
    assert(instruction.get_operands_count() <= 3 && "Unexpected operands count for SOP2");

    execReg = state.EXEC;
    sccReg = state.SCC;

    if (instruction.get_operands_count() < 3) {
        return;
    }

    write_operand_to_gpr(*instruction[0], state.SDST);
}

WfStateSOPK Wavefront::get_sopk_state() const {
    return WfStateSOPK(programCounter.get(), sccReg);
}

WfStateSOPK Wavefront::get_sopk_state(const Instruction& instruction) {
    assert(instruction.get_operands_count() == 2 && "Unexpected operands count for SOPK");

    auto state = WfStateSOPK(programCounter.get(), sccReg);

    state.SDST = to_uin64_t(read_operand(*instruction[0]));
    state.IMM16 = to_uin64_t(read_operand(*instruction[1]));
    return state;
}

void Wavefront::update_with_sopk_state(const Instruction& instruction,
                                       const WfStateSOPK& state) {
    sccReg = state.SCC;
    write_operand_to_gpr(*instruction[0], state.SDST);
}

WfStateSOPC Wavefront::get_sopc_state(const Instruction& instruction) {
    assert(instruction.get_operands_count() == 2 && "Unexpected operands count for SOP2");

    auto state = WfStateSOPC(execReg, modeReg.get(), sccReg);
    state.SSRC0 = to_uin64_t(read_operand(*instruction[0]));
    state.SSRC1 = to_uin64_t(read_operand(*instruction[1]));
    return state;
}

void Wavefront::update_with_sopc_state(const WfStateSOPC& state) {
    sccReg = state.SCC;
    m0Reg = state.M0;
}

WfStateSOPP Wavefront::get_common_sopp_state(const Instruction& instruction) const {
    return WfStateSOPP{programCounter.get(),  execReg,       vccReg, m0Reg,
                       statusReg.get(), modeReg.get(), sccReg};
}

void Wavefront::update_with_common_sopp_state(const Instruction& instruction,
                                              const WfStateSOPP& state) {
    execReg = state.EXEC;
    vccReg = state.VCC;
    sccReg = state.SCC;
}

WfStateSMEM Wavefront::get_smem_state(const Instruction& instruction) {
    assert(instruction.get_operands_count() == 3 &&
           "Unexpected amount of operands for SMEM instruction");

    uint64_t BASE = to_uin64_t(read_operand(*instruction[1]));
    uint64_t OFFSET = to_uin64_t(read_operand(*instruction[2]));

    return WfStateSMEM{BASE, OFFSET, read_operand(*instruction[0])};
}

void Wavefront::update_with_smem_state(const Instruction& instruction,
                                       const WfStateSMEM& state) {
    write_operand_to_gpr(*instruction[0], state.SDST);
}

WfStateVOP1 Wavefront::get_vop1_state(const Instruction& instruction) {
    assert(instruction.get_operands_count() == 2 &&
           "Unexpected amount of operands for VOP1 instruction");
    auto VDST = std::vector<uint64_t>();
    auto SRC0 = std::vector<uint64_t>();

    for (int i = 0; i < workItems.size(); ++i) {
        VDST.push_back(to_uin64_t(read_operand(*instruction[0], i)));
        SRC0.push_back(to_uin64_t(read_operand(*instruction[1], i)));
    }

    return WfStateVOP1(std::move(VDST), std::move(SRC0));
}

void Wavefront::update_with_vop1_state(const Instruction& instruction,
                                       const WfStateVOP1& state) {
    for (int i = 0; i < workItems.size(); ++i) {
        write_operand_to_gpr(*instruction[0], state.VDST[i], i);
    }
}

WfStateVOP2 Wavefront::get_vop2_state(const Instruction& instruction) {
    assert(instruction.get_operands_count() == 3 &&
           "Unexpected amount of operands for VOP2 instruction");
    auto VDST = std::vector<uint64_t>();
    auto SRC0 = std::vector<uint64_t>();
    auto SRC1 = std::vector<uint64_t>();

    for (int i = 0; i < workItems.size(); ++i) {
        VDST.push_back(to_uin64_t(read_operand(*instruction[0], i)));
        SRC0.push_back(to_uin64_t(read_operand(*instruction[1], i)));
        SRC1.push_back(to_uin64_t(read_operand(*instruction[2], i)));
    }

    return WfStateVOP2(std::move(VDST), std::move(SRC0), std::move(SRC1));
}

void Wavefront::update_with_vop2_state(const Instruction& instruction,
                                       const WfStateVOP2& state) {
    for (int i = 0; i < workItems.size(); ++i) {
        write_operand_to_gpr(*instruction[0], state.VDST[i], i);
    }
}

WfStateVOP3 Wavefront::get_vop3_state(const Instruction& instruction) {
    assert(instruction.get_operands_count() <= 4 &&
           "Unexpected amount of operands for VOP3 instruction");
    auto VDST = std::vector<uint64_t>();
    auto SRC0 = std::vector<uint64_t>();
    auto SRC1 = std::vector<uint64_t>();
    auto SRC2 = std::vector<uint64_t>();

    for (int i = 0; i < workItems.size(); ++i) {
        VDST.push_back(to_uin64_t(read_operand(*instruction[0], i)));
        SRC0.push_back(to_uin64_t(read_operand(*instruction[1], i)));
        SRC1.push_back(to_uin64_t(read_operand(*instruction[2], i)));
        if (instruction.get_operands_count() == 4) {
            SRC2.push_back(to_uin64_t(read_operand(*instruction[3], i)));
        }
    }

    return WfStateVOP3(std::move(VDST), std::move(SRC0), std::move(SRC1), std::move(SRC2));
}

void Wavefront::update_with_vop3_state(const Instruction& instruction,
                                       const WfStateVOP3& state) {
    for (int i = 0; i < workItems.size(); ++i) {
        write_operand_to_gpr(*instruction[0], state.VDST[i], i);
    }
}

std::vector<uint32_t> Wavefront::read_operand(const Operand& op) {
    return read_operand(op, -1);
}

std::vector<uint32_t> Wavefront::read_operand(const Operand& operand, int wiInd) {
    switch (operand.type) {
        case FLOAT:
            return {to_uint32_t(std::get<float>(operand.value))};
        case INT_CONST:
            return {static_cast<uint32_t>(std::get<int>(operand.value))};
        case LITERAL_CONST:
            return {std::get<uint32_t>(operand.value)};
        case REGISTER:
            return read_reg_operand(operand, wiInd);
    }
}

std::vector<uint32_t> Wavefront::read_reg_operand(const Operand& operand, int wiInd) {
    auto regType = std::get<RegisterType>(operand.value);

    bool isScalarReg = is_s_reg(regType);
    bool isVectorReg = is_v_reg(regType);

    if (operand.regAmount > 1 && !isScalarReg && !isVectorReg)
        throw std::runtime_error(
            "Only scalar or vector register can be multiple operands");

    if (isScalarReg) {
        std::vector<uint32_t> data(0);
        for (size_t i = 0; i < operand.regAmount; i++) {
            auto curSReg = static_cast<int>(regType) - S0 + i;
            assert(curSReg >= 0 && curSReg < scalarRegFile.size() &&
                   "Scalar register is out of range");
            data.push_back(scalarRegFile[curSReg]);
        }
        return data;
    }

    if (isVectorReg) {
        std::vector<uint32_t> data(0);
        assert(wiInd != -1 && "Illegal state");
        for (size_t i = 0; i < operand.regAmount; ++i) {
            auto vRegInd = static_cast<int>(regType) - V0 + i;
            data.push_back(read_vgpr(wiInd, vRegInd));
        }
        return data;
    }

    switch (regType) {
        case EXEC:
            return {static_cast<uint32_t>(execReg >> 32), static_cast<uint32_t>(execReg)};
        case SCC:
            return {static_cast<uint32_t>(sccReg)};
        case VCC:
            return {static_cast<uint32_t>(vccReg >> 32), static_cast<uint32_t>(vccReg)};
        case M0:
            return {m0Reg};
        default:
            assert(false && "Another register types are unsupported yet");
    }
}

void Wavefront::write_operand_to_gpr(const Operand& operand, uint64_t data) {
    write_operand_to_gpr(operand, data, -1);
}

void Wavefront::write_operand_to_gpr(const Operand& operand,
                                     const std::vector<uint32_t>& data) {
    write_operand_to_gpr(operand, data, -1);
}

void Wavefront::write_operand_to_gpr(const Operand& operand, uint64_t data, int wiInd) {
    assert(operand.regAmount <= 2);
    auto vData = std::vector<uint32_t>();
    if (operand.regAmount == 2) {
        vData.push_back(uint32_t(data >> 32));
    }
    vData.push_back(uint32_t(data));
    write_operand_to_gpr(operand, vData, wiInd);
}

void Wavefront::write_operand_to_gpr(const Operand& operand,
                                     const std::vector<uint32_t>& data,
                                     int wiInd) {
    if (operand.type != REGISTER) {
        // todo log
        return;
    }
    auto reg = std::get<RegisterType>(operand.value);

    if (is_s_reg(reg)) {
        assert(data.size() == operand.regAmount);
        RegisterType firstReg = S0;
        for (size_t i = 0; i < operand.regAmount; ++i) {
            auto curReg = static_cast<int>(reg) - firstReg + i;
            assert(curReg >= 0 && curReg < scalarRegFile.size() &&
                   "Scalar register is out of range");
            scalarRegFile[curReg] = data[i];
        }
    } else if (is_v_reg(reg)) {
        assert(data.size() == operand.regAmount);
        assert(wiInd != -1);

        for (size_t i = 0; i < operand.regAmount; ++i) {
            auto vRegInd = static_cast<int>(reg) - V0 + i;
            assert(vRegInd >= 0 && vRegInd < vectorRegFile.size() &&
                   "Vector register is out of range");
            set_vgpr(wiInd, vRegInd, data[i]);
        }
    } else {
        // todo log
    }
}
bool Wavefront::work_item_masked(size_t wiInd) const {
    return (execReg & (1 << wiInd)) != 0;
}

void Wavefront::set_sgpr_pair(size_t sgprInd, uint64_t data) {
    assert(sgprInd % 2 == 0 && "SGPR-pair should be aligned");
    scalarRegFile[sgprInd] = static_cast<uint32_t>(data >> 32);
    scalarRegFile[sgprInd + 1] = static_cast<uint32_t>(data);
}

uint64_t Wavefront::read_sgpr_pair(size_t sgprInd){
    assert(sgprInd % 2 == 0 && "SGPR-pair should be aligned");
    return static_cast<uint64_t>(scalarRegFile[sgprInd]) << 32 | static_cast<uint64_t>(scalarRegFile[sgprInd + 1]);
}
