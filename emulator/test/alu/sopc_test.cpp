#include <common/test/doctest.h>

#include "alu/alu.h"

TEST_SUITE("SOPC format instructions test") {
    size_t sopcWfId = 0;
    size_t sopcWfSize = 1;
    size_t sopcVgprnum = 0;

    TEST_CASE("s_cmp_eq_u64") {
        size_t sgprnum = 2;
        auto wavefront = Wavefront(nullptr, sopcWfSize, sopcWfId, sgprnum, sopcVgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_cmp_eq_u64"), "",{Operand{REGISTER, S0, 2}, Operand{INT_CONST, 0, 0}});

        SUBCASE("not equal") {
            wavefront.scalarRegFile[0] = 0xffffffff;
            wavefront.scalarRegFile[1] = 0xffffffff;
            run_sopc(instruction, &wavefront);
            CHECK(!wavefront.sccReg);
        }

        SUBCASE("equal") {
            wavefront.scalarRegFile[0] = 0x00000000;
            wavefront.scalarRegFile[1] = 0x00000000;
            run_sopc(instruction, &wavefront);
            CHECK(wavefront.sccReg);
        }
    }

    TEST_CASE("s_cmp_eq_i32") {
        size_t sgprnum = 1;
        auto wavefront = Wavefront(nullptr, sopcWfSize, sopcWfId, sgprnum, sopcVgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_cmp_eq_u64"), "",{Operand{REGISTER, S0, 1}, Operand{INT_CONST, -1, 0}});

        SUBCASE("equal") {
            wavefront.scalarRegFile[0] = 0xffffffff;
            run_sopc(instruction, &wavefront);
            CHECK(wavefront.sccReg);
        }

        SUBCASE("not equal") {
            wavefront.scalarRegFile[0] = 0x00000001;
            run_sopc(instruction, &wavefront);
            CHECK(!wavefront.sccReg);
        }
    }
}