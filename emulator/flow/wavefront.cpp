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
    actualSizeX = sizeX;
    actualSizeY = sizeY;
    actualSizeZ = sizeZ;
}
bool WorkGroup::all_wf_completed() {
    for (auto & wavefront : wavefronts) {
        if (!wavefront->completed) {
            return false;
        }
    }
    return true;
}
Instruction* Wavefront::get_cur_instr() const {
    return workGroup->kernelCode->get_instr(this->programCounter);
}

void Wavefront::to_next_instr() {
    //todo неправда
    programCounter += 4;
}

void Wavefront::set_v_reg(size_t wiInd, size_t vInd, uint32_t value) {
    vectorRegFile[wiInd * vgprsnum + vInd] = value;
}

WfStateSOP1 Wavefront::get_sop1_state(const Instruction& instr) {
    assert(instr.get_operands_count() <= 2 && "Unexpected operands count for SOP1");

    auto state = WfStateSOP1{execReg,       programCounter,  m0Reg,
                             modeReg.get(), statusReg.get(), sccReg};
    if (instr.get_operands_count() < 2) {
        return state;
    }

    state.SDST = to_uin64_t(read_operand(*instr[0]));
    state.SSRC0 = to_uin64_t(read_operand(*instr[1]));
    state.DEST_TYPE = std::get<RegisterType>(instr[0]->value);
    return state;
}

void Wavefront::update_with_sop1_state(const Instruction& instr,
                                       const WfStateSOP1& state) {
    assert(instr.get_operands_count() <= 2 && "Unexpected operands count for SOP1");

    execReg = state.EXEC;
    programCounter = state.PC;
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
        WfStateSOP2(execReg, programCounter, modeReg.get(), statusReg.get(), sccReg);

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
    programCounter = state.PC;
    sccReg = state.SCC;

    if (instruction.get_operands_count() < 3) {
        return;
    }

    write_operand_to_gpr(*instruction[0], state.SDST);
}

WfStateSOPK Wavefront::get_sopk_state() const {
    return WfStateSOPK(programCounter, sccReg);
}

WfStateSOPK Wavefront::get_sopk_state(const Instruction& instruction) {
    assert(instruction.get_operands_count() == 2 && "Unexpected operands count for SOPK");

    auto state = WfStateSOPK(programCounter, sccReg);

    state.SDST = to_uin64_t(read_operand(*instruction[0]));
    state.IMM16 = to_uin64_t(read_operand(*instruction[1]));
    return state;
}

void Wavefront::update_with_sopk_state(const Instruction& instruction,
                                       const WfStateSOPK& state) {
    programCounter = state.PC;
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
    return WfStateSOPP{programCounter,  execReg,       vccReg, m0Reg,
                       statusReg.get(), modeReg.get(), sccReg};
}

void Wavefront::update_with_common_sopp_state(const Instruction& instruction,
                                              const WfStateSOPP& state) {
    programCounter = state.PC;
    execReg = state.EXEC;
    vccReg = state.VCC;
    sccReg = state.SCC;
}

std::vector<uint32_t> Wavefront::read_operand(const Operand& op) {
    std::vector<uint32_t> data(0);

    if (op.type == FLOAT) {
        float copyingValue = std::get<float>(op.value);
        uint32_t value = 0;
        memcpy(&value, &copyingValue, sizeof value);
        data.push_back(value);
        return data;
    } else if (op.type == INT_CONST) {
        data.push_back((uint32_t) std::get<int>(op.value));
        return data;
    } else if (op.type == LITERAL_CONST) {
        data.push_back(std::get<uint32_t>(op.value));
        return data;
    }
    auto regType = std::get<RegisterType>(op.value);

    bool isScalarReg = is_s_reg(regType);
    bool isVectorReg = is_v_reg(regType);

    if (op.regAmount > 1 && !(isScalarReg || isVectorReg)) {
        throw std::runtime_error(
            "Only scalar or vector register can be multiple operands");
    }

    if (isScalarReg) {
        for (size_t i = 0; i < op.regAmount; i++) {
            auto curReg = static_cast<int>(regType) - S0 + i;
            // todo there is clear instruction for this case in specification. Think later
            assert(curReg >= 0 && curReg < scalarRegFile.size() &&
                   "Scalar register is out of range");
            data.push_back(scalarRegFile[curReg]);
        }
    } else if (isVectorReg) {
        // todo
    } else if (regType == EXEC) {
        data.push_back(uint32_t(execReg));
        data.push_back(uint32_t(execReg >> 32));
    } else if (regType == SCC) {
        data.push_back((uint32_t) sccReg);
    } else if (regType == VCC) {
        data.push_back(uint32_t(vccReg));
        data.push_back(uint32_t(vccReg >> 32));
    } else if (regType == m0Reg) {
        data.push_back(m0Reg);
    } else {
        assert(false && "Another register types are unsupported yet");
    }
    return data;
}

void Wavefront::write_operand_to_gpr(const Operand& operand, uint64_t data) {
    assert(operand.regAmount <= 2);
    auto vData = std::vector<uint32_t>();
    if (operand.regAmount > 1) {
        vData.push_back(uint32_t(data >> 32));
    }
    vData.push_back(uint32_t(data));
    write_operand_to_gpr(operand, vData);
}

void Wavefront::write_operand_to_gpr(const Operand& operand,
                                     const std::vector<uint32_t> &data) {
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
        RegisterType firstReg = V0;
        for (size_t i = 0; i < operand.regAmount; ++i) {

            auto curReg = static_cast<int>(reg) - firstReg + i;
            assert(curReg >= 0 && curReg < vectorRegFile.size() &&
                   "Vector register is out of range");
            vectorRegFile[curReg] = data[i];

        }
    } else {
        // todo log
        return;
    }
}
