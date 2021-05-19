//
// Created by Diana Kudaiberdieva
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../alu/alu.h"
#include "../../doctest.h"
#include "../../../common/common/common.hpp"


//SOP1
TEST_CASE(
    "run_s_abs_i32 - stores absolute signed value of the SSRC0 into SDST.") {
    auto* wavefront = new Wavefront(nullptr, 2, 0);
    Instruction instruction = Instruction("", "s_abs_i32", {"s0", "s1"});

    SUBCASE("src0 is 0x80000000") {
        wavefront->scalarRegFile[1] = 0x80000000;
        run_sop1(instruction, wavefront);
        CHECK(wavefront->scalarRegFile[0] == 0x80000000);
        CHECK(wavefront->sccReg);
    }
//    state.clear();
//    SUBCASE("src0 is 0x80000001") {
//        state.SSRC0 = 0x80000001;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0x7fffffff);
//        CHECK(state.SCC);
//    }
//    state.clear();
//    SUBCASE("src0 is 0x80000002") {
//        state.SSRC0 = 0x80000002;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0x7ffffffe);
//        CHECK(state.SCC);
//    }
//    state.clear();
//    SUBCASE("src0 is 0xffffffff") {
//        state.SSRC0 = 0xffffffff;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0x00000001);
//        CHECK(state.SCC);
//    }
//    state.clear();
//    SUBCASE("src0 is 0x7fffffff") {
//        state.SSRC0 = 0x7fffffff;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0x7fffffff);
//        CHECK(state.SCC);
//    }
    delete wavefront;
}

TEST_CASE(
    "run_s_bcnt0_i32_{b32,b64}") {
//    InstrKey instr = S_BCNT0_I32_B32;
//    WfStateSOP1Test state = WfStateSOP1Test();
//
//    SUBCASE("S_BCNT0_I32_B32(0x00000000) => 32") {
//        state.SSRC0 = 0x00000000;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 32);
//        CHECK(state.SCC);
//    }
//    state.clear();
//    SUBCASE("S_BCNT0_I32_B32(0xcccccccc) => 16") {
//        state.SSRC0 = 0xcccccccc;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 16);
//        CHECK(state.SCC);
//    }
//    state.clear();
//    SUBCASE("S_BCNT0_I32_B32(0xffffffff) => 0") {
//        state.SSRC0 = 0xffffffff;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0);
//        CHECK(!state.SCC);
//    }
//    state.clear();
//
//    instr = S_BCNT0_I32_B64;
//
//    SUBCASE("S_BCNT0_I32_B64(0x00000000) => 64") {
//        state.SSRC0 = 0x00000000;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 64);
//        CHECK(state.SCC);
//    }
//    state.clear();
//    SUBCASE("S_BCNT0_I32_B64(0xcccccccc) => 48") {
//        state.SSRC0 = 0xcccccccc;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 48);
//        CHECK(state.SCC);
//    }
}

TEST_CASE(
    "run_s_bcnt1_i32_{b32,b64}") {
//    InstrKey instr = S_BCNT1_I32_B32;
//    WfStateSOP1Test state = WfStateSOP1Test();
//
//    SUBCASE("S_BCNT1_I32_B32(0x00000000) => 0") {
//        state.SSRC0 = 0x00000000;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0);
//        CHECK(!state.SCC);
//    }
//    state.clear();
//    SUBCASE("S_BCNT1_I32_B32(0xcccccccc) => 16") {
//        state.SSRC0 = 0xcccccccc;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 16);
//        CHECK(state.SCC);
//    }
//    state.clear();
//    SUBCASE("S_BCNT1_I32_B32(0xffffffff) => 32") {
//        state.SSRC0 = 0xffffffff;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 32);
//        CHECK(state.SCC);
//    }
//    state.clear();
//
//    instr = S_BCNT1_I32_B64;
//
//    SUBCASE("S_BCNT1_I32_B64(0x00000000) => 0") {
//        state.SSRC0 = 0x00000000;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0);
//        CHECK(!state.SCC);
//    }
//    state.clear();
//    SUBCASE("S_BCNT1_I32_B64(0xcccccccc) => 16") {
//        state.SSRC0 = 0xcccccccc;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 16);
//        CHECK(state.SCC);
//    }
}
TEST_CASE(
    "run_s_ff0_i32_{b32,b64}") {
//    InstrKey instr = S_FF0_I32_B32;
//    WfStateSOP1Test state = WfStateSOP1Test();
//
//    SUBCASE("S_FF0_I32_B32(0xaaaaaaaa) => 0") {
//        state.SSRC0 = 0xaaaaaaaa;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0);
//    }
//    state.clear();
//    SUBCASE("SS_FF0_I32_B32(0x00000000) => 0") {
//        state.SSRC0 = 0x00000000;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0);
//    }
//    state.clear();
//    SUBCASE("S_FF0_I32_B32(0xfffeffff) => 16") {
//        state.SSRC0 = 0xfffeffff;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 16);
//    }
//    state.clear();
//    SUBCASE("S_FF0_I32_B32(0x55555555) => 1") {
//        state.SSRC0 = 0x55555555;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 1);
//    }
//    state.clear();
//    SUBCASE("S_FF0_I32_B32(0xffffffff) => 0xffffffff") {
//        state.SSRC0 = 0xffffffff;
//        run_sop1(instr, state);
//        CHECK(int32_t (state.SDST) == -1);
//        CHECK(state.SDST == 0xffffffff);
//    }
//
//    instr = S_FF0_I32_B64;
//    SUBCASE("S_FF0_I32_B64(0xffffffffffffffff) => 0xffffffffffffffff") {
//        state.SSRC0 = 0xffffffffffffffff;
//        run_sop1(instr, state);
//        CHECK(int32_t (state.SDST) == -1);
//        CHECK(state.SDST == 0xffffffffffffffff);
//    }
}
TEST_CASE(
    "run_s_ff1_i32_b32") {
//    InstrKey instr = S_FF1_I32_B32;
//    WfStateSOP1Test state = WfStateSOP1Test();
//
//    SUBCASE("S_FF1_I32_B32(0xaaaaaaaa) => 1") {
//        state.SSRC0 = 0xaaaaaaaa;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 1);
//    }
//    state.clear();
//    SUBCASE("S_FF1_I32_B32(0x00000000) => 0xffffffff") {
//        state.SSRC0 = 0x00000000;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0xffffffff);
//    }
//    state.clear();
//    SUBCASE("S_FF1_I32_B32(0xffffffff) => 0") {
//        state.SSRC0 = 0xffffffff;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0);
//    }
//    state.clear();
//    SUBCASE("S_FF1_I32_B32(0x00010000) => 16") {
//        state.SSRC0 = 0x00010000;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 16);
//    }
}
TEST_CASE(
    "run_s_flbit_i32_b32") {
//    InstrKey instr = S_FLBIT_I32_B32;
//    WfStateSOP1Test state = WfStateSOP1Test();
//
//    SUBCASE("S_FLBIT_I32_B32(0x00000000) => 0xffffffff") {
//        state.SSRC0 = 0x00000000;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0xffffffff);
//    }
//    state.clear();
//    SUBCASE("S_FLBIT_I32_B32(0x0000cccc) => 16") {
//        state.SSRC0 = 0x0000cccc;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 16);
//    }
//    state.clear();
//    SUBCASE("S_FLBIT_I32_B32(0xffff3333) => 0") {
//        state.SSRC0 = 0xffff3333;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0);
//    }
//    state.clear();
//    SUBCASE("S_FLBIT_I32_B32(0x7fffffff) => 1") {
//        state.SSRC0 = 0x7fffffff;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 1);
//    }
//    state.clear();
//    SUBCASE("S_FLBIT_I32_B32(0x80000000) => 0") {
//        state.SSRC0 = 0x80000000;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0);
//    }
//    state.clear();
//    SUBCASE("S_FLBIT_I32_B32(0xffffffff) => 0") {
//        state.SSRC0 = 0xffffffff;
//        run_sop1(instr, state);
//        CHECK(state.SDST == 0);
//    }

}


//SOP2
TEST_CASE(
    "run_s_absdiff_i32 - omputes the absolute value of difference between two values.") {
//    InstrKey instr = S_ABSDIFF_I32;
//    WfStateSOP2Test state = WfStateSOP2Test();
//
//    SUBCASE("src0 is 0x00000002, src1 is 0x00000005") {
//        state.SSRC0 = 0x00000002;
//        state.SSRC1 = 0x00000005;
//        run_sop2(instr, state);
//        CHECK(state.SDST == 0x00000003);
//        CHECK(state.SCC);
//    }
//    state.clear();
//    SUBCASE("src0 is 0x80000000, src1 is 0x00000001") {
//        state.SSRC0 = 0x80000000;
//        state.SSRC1 = 0x00000001;
//        run_sop2(instr, state);
//        CHECK(state.SDST == 0x7fffffff);
//        CHECK(state.SCC);
//    }
//    state.clear();
//    SUBCASE("src0 is 0x80000000, src1 is 0xffffffff") {
//        state.SSRC0 = 0x80000000;
//        state.SSRC1 = 0xffffffff;
//        run_sop2(instr, state);
//        CHECK(state.SDST == 0x7fffffff);
//        CHECK(state.SCC);
//    }
//    state.clear();
//    SUBCASE("src0 is 0x80000000, src1 is 0xfffffffe") {
//        state.SSRC0 = 0x80000000;
//        state.SSRC1 = 0xfffffffe;
//        run_sop2(instr, state);
//        CHECK(state.SDST == 0x7ffffffe);
//        CHECK(state.SCC);
//    }
}