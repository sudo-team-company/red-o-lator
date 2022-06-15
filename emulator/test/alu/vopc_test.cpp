#include <common/test/doctest.h>

#include "alu/alu.h"

static inline void init_wf(Wavefront& wf, size_t wiAmount) {
    assert(wiAmount <= 64);

    for (size_t i = 0; i < wiAmount; ++i) {
        wf.execReg |= uint64_t(1) << i;
    }
}

TEST_SUITE("VOPC format instructions test") {
    size_t vopcWfId = 0;
    size_t vopcWfSize = 5;

    TEST_CASE("v_lshlrev_b64") {
        size_t sgprnum = 0;
        size_t vgprnum = 2;
        Instruction instruction =
            Instruction(0, std::string("v_lshlrev_b64"), "",
                        {Operand{REGISTER, V0, 2},
                         Operand{REGISTER, INT_CONST, 1},
                         Operand{REGISTER, V0, 2}});

        auto wavefront = Wavefront(nullptr, vopcWfSize, vopcWfId, sgprnum, vgprnum);
        init_wf(wavefront, vopcWfSize);

        SUBCASE("0x8000000000000000 << 2 = 0") {
            for (size_t wi = 0; wi < wavefront.get_size(); ++wi) {
                wavefront.set_vgpr(wi, 0, 0x00000000);
                wavefront.set_vgpr(wi, 1, 0x80000000);
            }

            run_vop3(instruction, &wavefront);

            for (size_t wi = 0; wi < wavefront.get_size(); ++wi) {
                CHECK(wavefront.read_vgpr(wi, 0) == 0);
                CHECK(wavefront.read_vgpr(wi, 1) == 0);
            }
        }

        SUBCASE("0x1 << 2 = 0x4") {
            for (size_t wi = 0; wi < wavefront.get_size(); ++wi) {
                wavefront.set_vgpr(wi, 0, 0x1);
                wavefront.set_vgpr(wi, 1, 0x0);
            }

            run_vop3(instruction, &wavefront);

            for (size_t wi = 0; wi < wavefront.get_size(); ++wi) {
                CHECK(wavefront.read_vgpr(wi, 0) == 4);
                CHECK(wavefront.read_vgpr(wi, 1) == 0);
            }
        }
    }
}