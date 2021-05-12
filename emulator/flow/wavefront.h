//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_WAVEFRONT_H
#define RED_O_LATOR_WAVEFRONT_H

#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>
#include "../instr/instruction.h"
#include "../reg/register.h"

struct WorkGroup {};

struct Wavefront {
    std::shared_ptr<WorkGroup> WG;
    uint64_t EXEC;
    uint64_t PC;
    uint64_t VCC;
    uint32_t M0;
    bool SCC;
    StatusReg STATUS_REG;
    ModeReg MODE_REG;

    std::vector<uint32_t> S_REG_FILE;

    explicit Wavefront(int sgprsnum = 16)
        : EXEC(0), PC(0), VCC(0), STATUS_REG(0), MODE_REG(0) {
        S_REG_FILE = std::vector<uint32_t>(sgprsnum);
    }

    std::vector<uint32_t> read_reg(const reg::RegisterType regType,
                                   size_t reg_amount) {
        std::vector<uint32_t> data(0);

        bool isScalarReg = is_s_reg(regType);
        bool isVectorReg = is_v_reg(regType);

        assert(reg_amount > 1 && (isScalarReg || isVectorReg) && "Only scalar or vector register can be multiple operands");

        if (isScalarReg) {
            for (size_t i = 0; i < reg_amount; i++) {
                auto cur_reg = static_cast<int>(regType) - reg::S0 + i;
                //todo there is clear instruction for this case in specification. Think later
                assert(is_valid_register_ind(cur_reg) && "Scalar register is out of range");
                data.push_back(S_REG_FILE[cur_reg]);
            }
        } else if (isVectorReg) {
            //todo
        } else if (regType == reg::EXEC) {
            data.push_back(uint32_t(EXEC));
            data.push_back(uint32_t(EXEC >> 32));
        } else if (regType == reg::SCC) {
            data.push_back((uint32_t) SCC);
        } else if (regType == reg::VCC) {
            data.push_back(uint32_t(VCC));
            data.push_back(uint32_t(VCC >> 32));
        } else if (regType == reg::M0) {
            data.push_back(M0);
        } else {
            assert(false && "Another register types are unsupported yet");
        }
        return data;
    };
};

#endif  // RED_O_LATOR_WAVEFRONT_H
