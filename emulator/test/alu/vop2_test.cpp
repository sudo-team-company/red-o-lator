#include <common/test/doctest.h>

#include "alu/alu.h"

static inline void init_wf(Wavefront& wf, size_t wiAmount) {
    assert(wiAmount <= 64);

    for (size_t i = 0; i < wiAmount; ++i) {
        wf.execReg |= uint64_t(1) << i;
    }
}

TEST_SUITE("VOP2 format instructions test") {
    size_t vop2WfId = 0;
    size_t vop2WfSize = 64;

    TEST_CASE("v_add_u32") {
        size_t sgprnum = 2;
        size_t vgprnum = 4;
        Instruction instruction =
            Instruction(0, std::string("v_add_u32"), "",
                        {Operand{REGISTER, V0, 1},
                         Operand{REGISTER, S0, 1},
                         Operand{REGISTER, V0, 1}});

        auto wavefront = Wavefront(nullptr, vop2WfSize, vop2WfId, sgprnum, vgprnum);
        init_wf(wavefront, vop2WfSize);

        SUBCASE("0xf0000000 + {0x1, ..., 0x64} = {0xf0000001, ... , 0xf0000064}") {
            wavefront.scalarRegFile[0] = 0xf0000000;
            for (size_t wi = 0; wi < wavefront.get_size(); ++wi) {
                wavefront.set_vgpr(wi, 0, wi);
            }

            run_vop2(instruction, &wavefront);

            for (size_t wi = 0; wi < wavefront.get_size(); ++wi) {
                CHECK(wavefront.read_vgpr(wi, 0) == uint32_t(0xf0000000 + wi));
            }
        }
    }
}