//
// Created by Diana Kudaiberdieva
//

#include "wavefront.h"

void WorkGroup::setIds(int idX, int idY, int idZ) {
    IDX = idX;
    IDY = idY;
    IDZ = idZ;
}
void WorkGroup::setActualSize(int x, int y, int z) {
    actualSizeX = x;
    actualSizeY = y;
    actualSizeZ = z;
}

std::vector<uint32_t> Wavefront::read_operand(const Operand& op) {
    std::vector<uint32_t> data(0);

    if (op.type == FLOAT) {
        float copyingValue = std::get<float>(op.value);
        uint32_t value = 0;
        memcpy(&value, &copyingValue, sizeof value);
        data.push_back(value);
        return data;
    } else if (op.type == FLOAT) {
        data.push_back((uint32_t) std::get<int16_t>(op.value));
        return data;
    } else if (op.type == FLOAT) {
        data.push_back(std::get<uint32_t>(op.value));
        return data;
    }
    auto regType = std::get<RegisterType>(op.value);

    bool isScalarReg = is_s_reg(regType);
    bool isVectorReg = is_v_reg(regType);

    assert(op.regAmount > 1 && (isScalarReg || isVectorReg) &&
           "Only scalar or vector register can be multiple operands");

    if (isScalarReg) {
        for (size_t i = 0; i < op.regAmount; i++) {
            auto cur_reg = static_cast<int>(regType) - S0 + i;
            // todo there is clear instruction for this case in specification. Think later
            assert(is_valid_register_ind(cur_reg) && "Scalar register is out of range");
            data.push_back(scalarRegFile[cur_reg]);
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

Instruction* Wavefront::get_cur_instr() {
    return workGroup->kernelCode->get_instr(this->programCounter);
}

void Wavefront::to_next_instr() {
    programCounter += 4;
}

void Wavefront::set_v_reg(size_t wiInd, size_t vInd, uint32_t value) {
    vectorRegFile[wiInd * vgprsnum + vInd] = value;
}

WfStateSOP1 Wavefront::get_sop1_state(const Instruction& instr) {
    assert(instr.get_operands_count() <= 2 && "Unexpected operands count for SOP1");

    if (instr.get_operands_count() < 2) {
        return WfStateSOP1();
    }

//    uint64_t dest = read_operand(*instr[0]);
    return WfStateSOP1();
}
