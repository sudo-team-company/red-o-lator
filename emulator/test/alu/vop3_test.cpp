#include <common/test/doctest.h>

#include "alu/alu.h"

static inline void init_wf(Wavefront& wf, size_t wiAmount) {
    assert(wiAmount <= 64);

    for (size_t i = 0; i < wiAmount; ++i) {
        wf.execReg |= uint64_t(1) << i;
    }
}

TEST_SUITE("VOP3 format instructions test") {
    size_t vop3WfId = 0;
    size_t vop3WfSize = 5;

    TEST_CASE("v_lshlrev_b64") {
        size_t sgprnum = 0;
        size_t vgprnum = 2;
        Instruction instruction =
            Instruction(0, std::string("v_lshlrev_b64"), "",
                        {Operand{REGISTER, V0, 2},
                         Operand{INT_CONST, 2, 1},
                         Operand{REGISTER, V0, 2}});

        auto wavefront = Wavefront(nullptr, vop3WfSize, vop3WfId, sgprnum, vgprnum);
        init_wf(wavefront, vop3WfSize);

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

    TEST_CASE("v_mul_lo_u32") {
        size_t sgprnum = 3;
        size_t vgprnum = 2;
        Instruction instruction =
            Instruction(0, std::string("v_mul_lo_u32"), "",
                        {Operand{REGISTER, V1, 1},
                         Operand{REGISTER, V1, 1},
                         Operand{REGISTER, S2, 1}});

        auto wavefront = Wavefront(nullptr, vop3WfSize, vop3WfId, sgprnum, vgprnum);
        init_wf(wavefront, vop3WfSize);

        SUBCASE("0x8fffffff * 2 = 0x1ffffffe") {
            wavefront.scalarRegFile[2] = 2;
            for (size_t wi = 0; wi < wavefront.get_size(); ++wi) {
                wavefront.set_vgpr(wi, 1, 0x8fffffff);
            }

            run_vop3(instruction, &wavefront);

            for (size_t wi = 0; wi < wavefront.get_size(); ++wi) {
                CHECK(wavefront.read_vgpr(wi, 1) == 0x1ffffffe);
            }
        }

        SUBCASE("0xffffffff * 2 = 0xfffffffe") {
            wavefront.scalarRegFile[2] = 2;
            for (size_t wi = 0; wi < wavefront.get_size(); ++wi) {
                wavefront.set_vgpr(wi, 1, 0xffffffff);
            }

            run_vop3(instruction, &wavefront);

            for (size_t wi = 0; wi < wavefront.get_size(); ++wi) {
                CHECK(wavefront.read_vgpr(wi, 1) == 0xfffffffe);
            }
        }
    }
}