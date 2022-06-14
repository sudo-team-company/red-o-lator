#include <common/test/doctest.h>

#include "alu/alu.h"

TEST_SUITE("SOPP format instructions test") {
    size_t soppWfId = 0;
    size_t soppWfSize = 1;
    size_t soppVgprnum = 0;

    TEST_CASE("s_cbranch_scc1") {
        size_t sgprnum = 0;
        auto wavefront = Wavefront(nullptr, soppWfSize, soppWfId, sgprnum, soppVgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_cbranch_scc1"), "",{Operand{INT_CONST, 168, 0}});
        SUBCASE("Program counter is SRC0") {
            wavefront.sccReg = true;
            run_sopp(instruction, &wavefront);
            CHECK(wavefront.programCounter.get()->get_value() == 0xa8);
            CHECK(wavefront.sccReg); //no changes
        }

        SUBCASE("Program counter does not change") {
            wavefront.sccReg = false;
            run_sopp(instruction, &wavefront);
            CHECK(wavefront.programCounter.get()->get_value() == 0);
            CHECK(!wavefront.sccReg); //no changes
        }
    }

    TEST_CASE("s_cbranch_scc0") {
        size_t sgprnum = 2;
        auto wavefront = Wavefront(nullptr, soppWfSize, soppWfId, sgprnum, soppVgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_cbranch_scc0"), "",{Operand{INT_CONST, 168, 0}});

        SUBCASE("Program counter does not change") {
            wavefront.sccReg = true;
            run_sopp(instruction, &wavefront);
            CHECK(wavefront.programCounter.get()->get_value() == 0);
            CHECK(wavefront.sccReg); //no changes
        }

        SUBCASE("Program counter is SRC0") {
            wavefront.sccReg = false;
            run_sopp(instruction, &wavefront);
            CHECK(wavefront.programCounter.get()->get_value() == 0xa8);
            CHECK(!wavefront.sccReg); //no changes
        }
    }

    TEST_CASE("s_cbranch_execz") {
        size_t sgprnum = 2;
        auto wavefront = Wavefront(nullptr, soppWfSize, soppWfId, sgprnum, soppVgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_cbranch_execz"), "",{Operand{INT_CONST, 308, 0}});

        SUBCASE("Program counter does not change") {
            wavefront.execReg = 0xff;
            run_sopp(instruction, &wavefront);
            CHECK(wavefront.programCounter.get()->get_value() == 0x0);
            CHECK(wavefront.execReg == 0xff); //no changes
        }

        SUBCASE("Program counter is SRC0") {
            wavefront.execReg = 0;
            run_sopp(instruction, &wavefront);
            CHECK(wavefront.programCounter.get()->get_value() == 0x134);
            CHECK(wavefront.execReg == 0); //no changes
        }
    }
}