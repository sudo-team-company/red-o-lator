//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_WF_STATE_H
#define RED_O_LATOR_WF_STATE_H

#include "../reg/register.h"

struct WfStateSOP1 {
    uint64_t SDST = 0;
    uint64_t SSRC0 = 0;
    uint64_t EXEC = 0;
    uint64_t PC = 0;
    RegisterType DEST_TYPE;
    uint32_t M0 = 0;
    ModeReg* MODE = nullptr;
    StatusReg* STATUS = nullptr;
    bool SCC = false;

    WfStateSOP1(uint64_t SDST,
                uint64_t SSRC0,
                uint64_t EXEC,
                uint64_t PC,
                uint32_t M0,
                ModeReg* modeReg,
                StatusReg* statusReg,
                bool SCC)
        : SDST(SDST),
          SSRC0(SSRC0),
          EXEC(EXEC),
          PC(PC),
          M0(M0),
          MODE(modeReg),
          STATUS(statusReg),
          SCC(SCC) {}

    WfStateSOP1(uint64_t EXEC,
                uint64_t PC,
                uint32_t M0,
                ModeReg* modeReg,
                StatusReg* statusReg,
                bool SCC)
        : EXEC(EXEC), PC(PC), M0(M0), MODE(modeReg), STATUS(statusReg), SCC(SCC) {}
};

struct WfStateSOP2 {
    uint64_t SDST = 0;
    uint64_t SSRC0 = 0;
    uint64_t SSRC1 = 0;
    uint64_t EXEC = 0;
    uint64_t PC = 0;
    ModeReg* MODE = nullptr;
    StatusReg* STATUS = nullptr;
    bool SCC = false;

    WfStateSOP2(uint64_t SDST,
                uint64_t SSRC0,
                uint64_t SSRC1,
                uint64_t EXEC,
                uint64_t PC,
                ModeReg* modeReg,
                StatusReg* statusReg,
                bool SCC)
        : SDST(SDST),
          SSRC0(SSRC0),
          SSRC1(SSRC1),
          EXEC(EXEC),
          PC(PC),
          MODE(modeReg),
          STATUS(statusReg),
          SCC(SCC) {}

    WfStateSOP2(
        uint64_t EXEC, uint64_t PC, ModeReg* modeReg, StatusReg* statusReg, bool SCC)
        : EXEC(EXEC), PC(PC), MODE(modeReg), STATUS(statusReg), SCC(SCC) {}
};

struct WfStateSOPP {
    uint64_t RELADDR = 0;
    uint64_t PC = 0;
    uint64_t EXEC = 0;
    uint64_t VCC = 0;
    uint32_t M0 = 0;
    uint32_t SIMM16 = 0;
    StatusReg* STATUS = nullptr;
    ModeReg* MODE = nullptr;
    bool SCC = false;

    WfStateSOPP(uint64_t PC,
                uint64_t EXEC,
                uint64_t VCC,
                uint64_t M0,
                StatusReg* statusReg,
                ModeReg* modeReg,
                bool SCC)
        : PC(PC),
          EXEC(EXEC),
          VCC(VCC),
          M0(M0),
          STATUS(statusReg),
          MODE(modeReg),
          SCC(SCC) {}
};

struct WfStateSOPC {
    uint64_t SSRC0 = 0;
    uint64_t SSRC1 = 0;
    uint32_t M0 = 0;
    ModeReg* MODE = 0;
    bool SCC = false;

    WfStateSOPC(uint32_t M0, ModeReg* modeReg, bool SCC)
        : M0(M0), MODE(modeReg), SCC(SCC) {}
};

struct WfStateSOPK {
    uint64_t SDST = 0;
    uint64_t SSRC0 = 0;
    uint64_t RELADDR = 0;
    uint64_t PC = 0;
    uint32_t IMM16 = 0;
    bool SCC = false;

    WfStateSOPK(uint64_t PC, bool SCC) : PC(PC), SCC(SCC) {}
};

struct WfStateSMEM {
    uint64_t BASE;
    std::vector<uint32_t> SDST;
    uint64_t OFFSET;
};

#endif  // RED_O_LATOR_WF_STATE_H
