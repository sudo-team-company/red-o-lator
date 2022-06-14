#include <common/test/doctest.h>

#include "alu/alu.h"

static inline void init_wf(Wavefront& wf, size_t wiAmount) {
    assert(wiAmount <= 64);

    for (size_t i = 0; i < wiAmount; ++i) {
        wf.execReg |= uint64_t(1) << i;
    }
}

TEST_SUITE("VOP1 format instructions test") {
    size_t vop1WfId = 0;
    size_t vop1WfSize = 64;

    TEST_CASE("v_mov_b32") {
        size_t sgprnum = 2;
        size_t vgprnum = 4;
        Instruction instruction =
            Instruction(0, std::string("v_mov_b32"), "",
                        {Operand{REGISTER, V3, 1},
                         Operand{REGISTER, S1, 1}});
        auto wavefront = Wavefront(nullptr, vop1WfSize, vop1WfId, sgprnum, vgprnum);
        init_wf(wavefront, vop1WfSize);

        SUBCASE("value from s1 should be in all v0") {
            wavefront.scalarRegFile[1] = 0xf2345678;
            run_vop1(instruction, &wavefront);
            for (size_t wi = 0; wi < wavefront.get_size(); ++wi) {
                CHECK(wavefront.read_vgpr(wi, 3) == 0xf2345678);
            }
        }
    }
}