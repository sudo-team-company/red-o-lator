#include <common/test/doctest.h>

#include "alu/alu.h"

TEST_SUITE("SOP2 format instructions test") {
    size_t sop2WfId = 0;
    size_t sop2WfSize = 1;
    size_t sop2Vgprnum = 0;

    TEST_CASE("s_absdiff_i32 - computes the absolute value of difference between two values.") {
        size_t sgprnum = 12;
        auto wavefront = Wavefront(nullptr, sop2WfSize, sop2WfId, sgprnum, sop2Vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_absdiff_i32"), "",
                        {Operand{REGISTER, S6, 1}, Operand{REGISTER, S10, 1},
                         Operand{REGISTER, S11, 1}});
        SUBCASE("src0 is 0x00000002, src1 is 0x00000005") {
            wavefront.scalarRegFile[10] = 0x00000002;
            wavefront.scalarRegFile[11] = 0x00000005;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[6] == 0x00000003);
            CHECK(wavefront.sccReg);
        }
        SUBCASE("src0 is 0x80000000, src1 is 0x00000001") {
            wavefront.scalarRegFile[10] = 0x80000000;
            wavefront.scalarRegFile[11] = 0x00000001;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[6] == 0x7fffffff);
            CHECK(wavefront.sccReg);
        }
        SUBCASE("src0 is 0x80000000, src1 is 0xffffffff") {
            wavefront.scalarRegFile[10] = 0x80000000;
            wavefront.scalarRegFile[11] = 0xffffffff;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[6] == 0x7fffffff);
            CHECK(wavefront.sccReg);
        }
        SUBCASE("src0 is 0x80000000, src1 is 0xfffffffe") {
            wavefront.scalarRegFile[10] = 0x80000000;
            wavefront.scalarRegFile[11] = 0xfffffffe;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[6] == 0x7ffffffe);
            CHECK(wavefront.sccReg);
        }
    }

    TEST_CASE("s_addc_u32") {
        size_t sgprnum = 12;
        auto wavefront = Wavefront(nullptr, sop2WfSize, sop2WfId, sgprnum, sop2Vgprnum);
        Instruction instruction(0, std::string("s_addc_u32"), "",
                                {Operand{REGISTER, S6, 1}, Operand{REGISTER, S10, 1},
                                 Operand{REGISTER, S11, 1}});

        SUBCASE("src0 is 0x00000002, src1 is 0x00000005") {
            wavefront.sccReg = true;
            wavefront.scalarRegFile[10] = 0x00000002;
            wavefront.scalarRegFile[11] = 0x00000005;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x00000008);
            CHECK(!wavefront.sccReg);
        }

        SUBCASE("src0 is 0x00000002, src1 is 0x00000005") {
            wavefront.sccReg = false;
            wavefront.scalarRegFile[10] = 0x80000000;
            wavefront.scalarRegFile[11] = 0x00000005;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x80000005);
            CHECK(!wavefront.sccReg);
        }
        SUBCASE("src0 is 0x7fffffff, src1 is 0x00000001 => overflow") {
            wavefront.scalarRegFile[10] = 0xffffffff;
            wavefront.scalarRegFile[11] = 0x00000001;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0);
            CHECK(wavefront.sccReg);
        }
    }

    TEST_CASE("s_add_u32") {
        size_t sgprnum = 12;
        auto wavefront = Wavefront(nullptr, sop2WfSize, sop2WfId, sgprnum, sop2Vgprnum);
        Instruction instruction(0, std::string("s_addc_u32"), "",
                                {Operand{REGISTER, S0, 1}, Operand{REGISTER, S10, 1},
                                 Operand{REGISTER, S11, 1}});

        SUBCASE("src0 is 0x00000002, src1 is 0x00000005") {
            wavefront.scalarRegFile[10] = 0x00000002;
            wavefront.scalarRegFile[11] = 0x00000005;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x00000007);
            CHECK(!wavefront.sccReg);
        }
        SUBCASE("src0 is 0x7fffffff, src1 is 0x00000001 => overflow") {
            wavefront.scalarRegFile[10] = 0xffffffff;
            wavefront.scalarRegFile[11] = 0x00000001;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0);
            CHECK(wavefront.sccReg);
        }
    }

    TEST_CASE("s_add_i32") {
        size_t sgprnum = 12;
        auto wavefront = Wavefront(nullptr, sop2WfSize, sop2WfId, sgprnum, sop2Vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_add_i32"), "",
                        {Operand{REGISTER, S6, 1}, Operand{REGISTER, S10, 1},
                         Operand{REGISTER, S11, 1}});
        SUBCASE("src0 is -2, src1 is 3") {
            wavefront.scalarRegFile[10] = -2;
            wavefront.scalarRegFile[11] = 3;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x00000001);
            CHECK(!wavefront.sccReg);
        }
        SUBCASE("src0 is -3, src1 is -2") {
            wavefront.scalarRegFile[10] = -2;
            wavefront.scalarRegFile[11] = -3;
            run_sop2(instruction, &wavefront);
            CHECK(static_cast<int32_t>(wavefront.scalarRegFile[0]) == -5);
            CHECK(wavefront.scalarRegFile[0] == 0xfffffffb);
            CHECK(!wavefront.sccReg);
        }
        SUBCASE("src0 is -2147483648, src1 is -1") {
            wavefront.scalarRegFile[10] = -2147483648;
            wavefront.scalarRegFile[11] = -1;
            run_sop2(instruction, &wavefront);
            CHECK(static_cast<int32_t>(wavefront.scalarRegFile[0]) == 2147483647);
            CHECK(wavefront.scalarRegFile[0] == 0x7fffffff);
            CHECK(wavefront.sccReg);
        }
    }

    TEST_CASE("s_sub_i32") {
        size_t sgprnum = 12;
        auto wavefront = Wavefront(nullptr, sop2WfSize, sop2WfId, sgprnum, sop2Vgprnum);
        Instruction instruction =
            Instruction(0, std::string("s_sub_i32"), "",
                        {Operand{REGISTER, S6, 1}, Operand{REGISTER, S10, 1},
                         Operand{REGISTER, S11, 1}});
        SUBCASE("src0 is -2, src1 is 3") {
            wavefront.scalarRegFile[10] = -2;
            wavefront.scalarRegFile[11] = 3;
            run_sop2(instruction, &wavefront);
            CHECK(static_cast<int32_t>(wavefront.scalarRegFile[0]) == -5);
            CHECK(wavefront.scalarRegFile[0] == 0xfffffffb);
            CHECK(!wavefront.sccReg);
        }
        SUBCASE("src0 is -3, src1 is -2") {
            wavefront.scalarRegFile[10] = -2;
            wavefront.scalarRegFile[11] = -3;
            run_sop2(instruction, &wavefront);
            CHECK(static_cast<int32_t>(wavefront.scalarRegFile[0]) == 1);
            CHECK(wavefront.scalarRegFile[0] == 0x00000001);
            CHECK(!wavefront.sccReg);
        }
        SUBCASE("src0 is -2147483648, src1 is -1") {
            wavefront.scalarRegFile[10] = -2147483648;
            wavefront.scalarRegFile[11] = 1;
            run_sop2(instruction, &wavefront);
            CHECK(static_cast<int32_t>(wavefront.scalarRegFile[0]) == 2147483647);
            CHECK(wavefront.scalarRegFile[0] == 0x7fffffff);
            CHECK(wavefront.sccReg);
        }
    }

    TEST_CASE("s_lshl_b32") {
        size_t sgprnum = 2;
        auto wavefront = Wavefront(nullptr, sop2WfSize, sop2WfId, sgprnum, sop2Vgprnum);
        Instruction instruction(0, std::string("s_lshl_b32"), "",
                                {Operand{REGISTER, S0, 1}, Operand{REGISTER, S1, 1},
                                 Operand{INT_CONST, 1, 0}});

        SUBCASE("SCC should be equal to 0") {
            wavefront.scalarRegFile[1] = 0x80000000;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0);
            CHECK(!wavefront.sccReg);
        }

        SUBCASE("SCC should be equal to 1") {
            wavefront.scalarRegFile[1] = 0x00000001;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x00000002);
            CHECK(wavefront.sccReg);
        }
    }

    TEST_CASE("s_and_b32") {
        size_t sgprnum = 3;
        auto wavefront = Wavefront(nullptr, sop2WfSize, sop2WfId, sgprnum, sop2Vgprnum);
        Instruction instruction(0, std::string("s_and_b32"), "",
                                {Operand{REGISTER, S0, 1}, Operand{REGISTER, S1, 1},
                                 Operand{REGISTER, S2, 1}});

        SUBCASE("SCC should be equal to 1") {
            wavefront.scalarRegFile[1] = 0xf0f0f0f0;
            wavefront.scalarRegFile[2] = 0x805f0bbb;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x805000b0);
            CHECK(wavefront.sccReg);
        }

        SUBCASE("SCC should be equal to 0") {
            wavefront.scalarRegFile[1] = 0xf0f0f00f;
            wavefront.scalarRegFile[2] = 0x0f0f0ff0;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0);
            CHECK(!wavefront.sccReg);
        }
    }

    TEST_CASE("s_bfe_u32") {
        size_t sgprnum = 3;
        auto wavefront = Wavefront(nullptr, sop2WfSize, sop2WfId, sgprnum, sop2Vgprnum);
        Instruction instruction(0, std::string("s_bfe_u32"), "",
                                {Operand{REGISTER, S0, 1}, Operand{REGISTER, S1, 1},
                                 Operand{REGISTER, S2, 1}});

        SUBCASE("SCC should be equal to 1") {
            wavefront.scalarRegFile[1] = 0xffff01ef; //1111111111 11111100000001111 01111
            wavefront.scalarRegFile[2] = 0x00110005;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x0001F80F);
            CHECK(wavefront.sccReg);
        }

        SUBCASE("SCC should be equal to 0") {
            wavefront.scalarRegFile[1] = 0xffffffff;
            wavefront.scalarRegFile[2] = 0xff00ff20;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0);
            CHECK(!wavefront.sccReg);
        }
    }

    TEST_CASE("s_mul_i32") {
        size_t sgprnum = 3;
        auto wavefront = Wavefront(nullptr, sop2WfSize, sop2WfId, sgprnum, sop2Vgprnum);
        Instruction instruction(0, std::string("s_mul_i32"), "",
                                {Operand{REGISTER, S0, 1}, Operand{REGISTER, S1, 1},
                                 Operand{REGISTER, S2, 1}});
        SUBCASE("2 * 5 = 10") {
            wavefront.scalarRegFile[1] = 0x00000002;
            wavefront.scalarRegFile[2] = 0x00000005;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x0000000a);
        }

        SUBCASE("INT_MAX * 1 = INT_MAX") {
            wavefront.scalarRegFile[1] = 0x7fffffff;
            wavefront.scalarRegFile[2] = 0x00000001;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x7fffffff);
        }

        SUBCASE("-1 * 100 = -100") {
            wavefront.scalarRegFile[1] = 0xffffffff;
            wavefront.scalarRegFile[2] = 0x00000064;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0xffffff9c);
        }

        SUBCASE("-1 * -2 = 2") {
            wavefront.scalarRegFile[1] = 0xffffffff;
            wavefront.scalarRegFile[2] = 0xfffffffe;
            run_sop2(instruction, &wavefront);
            CHECK(wavefront.scalarRegFile[0] == 0x00000002);
        }
    }
}