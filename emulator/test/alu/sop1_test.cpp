#include <common/test/doctest.h>

#include "alu/alu.h"

TEST_SUITE("SOP1 format instructions test") {
    size_t sop1WfId = 0;
    size_t wfSize = 1;
    size_t vgprnum = 0;

    TEST_CASE("s_abs_i32 - stores absolute signed value of the SSRC0 into SDST.") {
        size_t sgprnum = 2;
        auto wavefront = Wavefront(nullptr, wfSize, sop1WfId, sgprnum, vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_abs_i32"), "",
                        {Operand{REGISTER, S0, 1}, Operand{REGISTER, S1, 1}});

        SUBCASE("src0 is 0x80000000") {
            wavefront.scalarRegFile[1] = 0x80000000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x80000000);
            CHECK(wavefront.sccReg);
        }
        SUBCASE("src0 is 0x80000001") {
            wavefront.scalarRegFile[1] = 0x80000001;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x7fffffff);
            CHECK(wavefront.sccReg);
        }
        SUBCASE("src0 is 0x80000002") {
            wavefront.scalarRegFile[1] = 0x80000002;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x7ffffffe);
            CHECK(wavefront.sccReg);
        }
        SUBCASE("src0 is 0xffffffff") {
            wavefront.scalarRegFile[1] = 0xffffffff;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x00000001);
            CHECK(wavefront.sccReg);
        }
        SUBCASE("src0 is 0x7fffffff") {
            wavefront.scalarRegFile[1] = 0x7fffffff;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x7fffffff);
            CHECK(wavefront.sccReg);
        }
    }

    TEST_CASE("s_bcnt0_i32_b32") {
        size_t sgprnum = 5;
        auto wavefront = Wavefront(nullptr, wfSize, sop1WfId, sgprnum, vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_bcnt0_i32_b32"), "",
                        {Operand{REGISTER, S2, 1}, Operand{REGISTER, S4, 1}});
        SUBCASE("S_BCNT0_I32_B32(0x00000000) => 32") {
            wavefront.scalarRegFile[4] = 0x00000000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[2] == 32);
            CHECK(wavefront.sccReg);
        }
        SUBCASE("S_BCNT0_I32_B32(0xcccccccc) => 16") {
            wavefront.scalarRegFile[4] = 0xcccccccc;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[2] == 16);
            CHECK(wavefront.sccReg);
        }
        SUBCASE("S_BCNT0_I32_B32(0xffffffff) => 0") {
            wavefront.scalarRegFile[4] = 0xffffffff;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[2] == 0);
            CHECK(!wavefront.sccReg);
        }
    }

    TEST_CASE("s_bcnt0_i32_b64") {
        size_t sgprnum = 6;
        auto wavefront = Wavefront(nullptr, wfSize, sop1WfId, sgprnum, vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_bcnt0_i32_b64"), "",
                        {Operand{REGISTER, S2, 1}, Operand{REGISTER, S4, 2}});

        SUBCASE("S_BCNT0_I32_B64(0x00000000) => 64") {
            wavefront.scalarRegFile[4] = 0x00000000;
            wavefront.scalarRegFile[5] = 0x00000000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[2] == 64);
            CHECK(wavefront.sccReg);
        }
        SUBCASE("S_BCNT0_I32_B64(0xcccccccc) => 48") {
            wavefront.scalarRegFile[4] = 0x00000000;
            wavefront.scalarRegFile[5] = 0xcccccccc;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[2] == 48);
            CHECK(wavefront.sccReg);
        }
    }

    TEST_CASE("s_bcnt1_i32_b32}") {
        size_t sgprnum = 7;
        auto wavefront = Wavefront(nullptr, wfSize, sop1WfId, sgprnum, vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_bcnt1_i32_b32"), "",
                        {Operand{REGISTER, S6, 1}, Operand{REGISTER, S1, 1}});

        SUBCASE("S_BCNT1_I32_B32(0x00000000) => 0") {
            wavefront.scalarRegFile[1] = 0x00000000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[6] == 0);
            CHECK(!wavefront.sccReg);
        }
        SUBCASE("S_BCNT1_I32_B32(0xcccccccc) => 16") {
            wavefront.scalarRegFile[1] = 0xcccccccc;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[6] == 16);
            CHECK(wavefront.sccReg);
        }
        SUBCASE("S_BCNT1_I32_B32(0xffffffff) => 32") {
            wavefront.scalarRegFile[1] = 0xffffffff;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[6] == 32);
            CHECK(wavefront.sccReg);
        }
    }

    TEST_CASE("s_bcnt1_i32_b64}") {
        size_t sgprnum = 4;
        auto wavefront = Wavefront(nullptr, wfSize, sop1WfId, sgprnum, vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_bcnt1_i32_b64"), "",
                        {Operand{REGISTER, S2, 1}, Operand{REGISTER, S2, 2}});

        SUBCASE("S_BCNT1_I32_B64(0x00000000) => 0") {
            wavefront.scalarRegFile[2] = 0x00000000;
            wavefront.scalarRegFile[3] = 0x00000000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[2] == 0);
            CHECK(!wavefront.sccReg);
        }
        SUBCASE("S_BCNT1_I32_B64(0xcccccccc) => 16") {
            wavefront.scalarRegFile[2] = 0x00000000;
            wavefront.scalarRegFile[3] = 0xcccccccc;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[2] == 16);
            CHECK(wavefront.sccReg);
        }
    }

    TEST_CASE("s_ff0_i32_b32") {
        size_t sgprnum = 1;
        auto wavefront = Wavefront(nullptr, wfSize, sop1WfId, sgprnum, vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_ff0_i32_b32"), "",
                        {Operand{REGISTER, S0, 1}, Operand{REGISTER, S0, 1}});
        SUBCASE("S_FF0_I32_B32(0xaaaaaaaa) => 0") {
            wavefront.scalarRegFile[0] = 0xaaaaaaaa;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0);
        }
        SUBCASE("SS_FF0_I32_B32(0x00000000) => 0") {
            wavefront.scalarRegFile[0] = 0x00000000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0);
        }
        SUBCASE("S_FF0_I32_B32(0xfffeffff) => 16") {
            wavefront.scalarRegFile[0] = 0xfffeffff;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 16);
        }
        SUBCASE("S_FF0_I32_B32(0x55555555) => 1") {
            wavefront.scalarRegFile[0] = 0x55555555;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 1);
        }
        SUBCASE("S_FF0_I32_B32(0xffffffff) => 0xffffffff") {
            wavefront.scalarRegFile[0] = 0xffffffff;
            run_sop1(instruction, &wavefront);
            CHECK(int32_t(wavefront.scalarRegFile[0]) == -1);
            CHECK(wavefront.scalarRegFile[0] == 0xffffffff);
        }
    }

    TEST_CASE("s_ff0_i32_b64") {
        size_t sgprnum = 101;
        auto wavefront = Wavefront(nullptr, wfSize, sop1WfId, sgprnum, vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_ff0_i32_b64"), "",
                        {Operand{REGISTER, S0, 1}, Operand{REGISTER, S99, 2}});
        SUBCASE("S_FF0_I32_B64(0xffffffffffffffff) => 0xffffffff") {
            wavefront.scalarRegFile[99] = 0xffffffff;
            wavefront.scalarRegFile[100] = 0xffffffff;
            run_sop1(instruction, &wavefront);
            CHECK(int32_t(wavefront.scalarRegFile[0]) == -1);
            CHECK(wavefront.scalarRegFile[0] == 0xffffffff);
        }
    }

    TEST_CASE("s_ff1_i32_b32") {
        size_t sgprnum = 101;
        auto wavefront = Wavefront(nullptr, wfSize, sop1WfId, sgprnum, vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_ff1_i32_b32"), "",
                        {Operand{REGISTER, S99, 1}, Operand{REGISTER, S100, 1}});

        SUBCASE("S_FF1_I32_B32(0xaaaaaaaa) => 1") {
            wavefront.scalarRegFile[100] = 0xaaaaaaaa;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[99] == 1);
        }
        SUBCASE("S_FF1_I32_B32(0x00000000) => 0xffffffff") {
            wavefront.scalarRegFile[100] = 0x00000000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[99] == 0xffffffff);
        }
        SUBCASE("S_FF1_I32_B32(0xffffffff) => 0") {
            wavefront.scalarRegFile[100] = 0xffffffff;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[99] == 0);
        }
        SUBCASE("S_FF1_I32_B32(0x00010000) => 16") {
            wavefront.scalarRegFile[100] = 0x00010000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[99] == 16);
        }
    }

    TEST_CASE("s_flbit_i32_b32") {
        size_t sgprnum = 1;
        auto wavefront = Wavefront(nullptr, wfSize, sop1WfId, sgprnum, vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_flbit_i32_b32"), "",
                        {Operand{REGISTER, S0, 1}, Operand{REGISTER, S0, 1}});

        SUBCASE("s_flbit_i32_b32(0x00000000) => 0xffffffff") {
            wavefront.scalarRegFile[0] = 0x00000000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0xffffffff);
        }
        SUBCASE("s_flbit_i32_b32(0x0000cccc) => 16") {
            wavefront.scalarRegFile[0] = 0x0000cccc;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 16);
        }
        SUBCASE("s_flbit_i32_b32(0xffff3333) => 0") {
            wavefront.scalarRegFile[0] = 0xffff3333;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0);
        }
        SUBCASE("s_flbit_i32_b32(0x7fffffff) => 1") {
            wavefront.scalarRegFile[0] = 0x7fffffff;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 1);
        }
        SUBCASE("s_flbit_i32_b32(0x80000000) => 0") {
            wavefront.scalarRegFile[0] = 0x80000000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0);
        }
        SUBCASE("s_flbit_i32_b32(0xffffffff) => 0") {
            wavefront.scalarRegFile[0] = 0xffffffff;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0);
        }
    }

    TEST_CASE("s_and_saveexec_b64") {
        size_t sgprnum = 2;
        auto wavefront = Wavefront(nullptr, wfSize, sop1WfId, sgprnum, vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_bcnt1_i32_b32"), "",
                        {Operand{REGISTER, S0, 2}, Operand{REGISTER, VCC, 2}});

        SUBCASE("EXEC != 1") {
            wavefront.execReg = 0xfffffffffffff321;
            wavefront.vccReg = 0xffffffff80000fff;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0xfffff321);  // lsb
            CHECK(wavefront.scalarRegFile[1] == 0xffffffff);  // msb
            CHECK(wavefront.execReg == 0xffffffff80000321);
            CHECK(wavefront.sccReg);
        }

        SUBCASE("EXEC = 0") {
            wavefront.execReg = 0x1234567890abcdef;
            wavefront.vccReg = 0x0000000001000000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x90abcdef);  // lsb
            CHECK(wavefront.scalarRegFile[1] == 0x12345678);  // msb
            CHECK(wavefront.execReg == 0);
            CHECK(!wavefront.sccReg);
        }
    }
    TEST_CASE("s_andn1_saveexec_b64") {
        size_t sgprnum = 2;
        auto wavefront = Wavefront(nullptr, wfSize, sop1WfId, sgprnum, vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_andn1_saveexec_b64"), "",
                        {Operand{REGISTER, S0, 2}, Operand{REGISTER, VCC, 2}});

        SUBCASE("EXEC != 1") {
            wavefront.execReg = 0xfffffffffffff321;
            wavefront.vccReg =  0x000000007ffff000;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0xfffff321);  // lsb
            CHECK(wavefront.scalarRegFile[1] == 0xffffffff);  // msb
            CHECK(wavefront.execReg == 0xffffffff80000321);
            CHECK(wavefront.sccReg);
        }

        SUBCASE("EXEC = 0") {
            wavefront.execReg = 0x1234567890abcdef;
            wavefront.vccReg = 0xfffffffffeffffff;
            run_sop1(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x90abcdef);  // lsb
            CHECK(wavefront.scalarRegFile[1] == 0x12345678);  // msb
            CHECK(wavefront.execReg == 0);
            CHECK(!wavefront.sccReg);
        }
    }
}