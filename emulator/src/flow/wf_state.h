#pragma once

#include "reg/register.h"

struct WfStateSOP1 {
    uint64_t SDST = 0;
    uint64_t SSRC0 = 0;
    uint64_t EXEC = 0;
    RegisterType DEST_TYPE;
    RegisterType SRC0_TYPE;
    uint32_t M0 = 0;
    ProgramCounter* PC = nullptr;
    ModeReg* MODE = nullptr;
    StatusReg* STATUS = nullptr;
    bool SCC = false;

    WfStateSOP1(uint64_t EXEC,
                ProgramCounter* PC,
                uint32_t M0,
                ModeReg* modeReg,
                StatusReg* statusReg,
                bool SCC)
        : EXEC(EXEC), M0(M0), PC(PC), MODE(modeReg), STATUS(statusReg), SCC(SCC) {}
};

struct WfStateSOP2 {
    uint64_t SDST = 0;
    uint64_t SSRC0 = 0;
    uint64_t SSRC1 = 0;
    uint64_t EXEC = 0;
    ProgramCounter* PC = nullptr;
    ModeReg* MODE = nullptr;
    StatusReg* STATUS = nullptr;
    bool SCC = false;

    WfStateSOP2(uint64_t EXEC,
                ProgramCounter* PC,
                ModeReg* modeReg,
                StatusReg* statusReg,
                bool SCC)
        : EXEC(EXEC), PC(PC), MODE(modeReg), STATUS(statusReg), SCC(SCC) {}
};

struct WfStateSOPP {
    uint64_t RELADDR = 0;
    uint64_t EXEC = 0;
    uint64_t VCC = 0;
    uint32_t M0 = 0;
    uint32_t SIMM16 = 0;
    ProgramCounter* PC = nullptr;
    StatusReg* STATUS = nullptr;
    ModeReg* MODE = nullptr;
    bool SCC = false;

    WfStateSOPP(ProgramCounter* PC,
                uint64_t EXEC,
                uint64_t VCC,
                uint64_t M0,
                StatusReg* statusReg,
                ModeReg* modeReg,
                bool SCC)
        : EXEC(EXEC),
          VCC(VCC),
          M0(M0),
          PC(PC),
          STATUS(statusReg),
          MODE(modeReg),
          SCC(SCC) {}
};

struct WfStateSOPC {
    uint64_t SSRC0 = 0;
    uint64_t SSRC1 = 0;
    uint32_t M0 = 0;
    ModeReg* MODE = nullptr;
    bool SCC = false;

    WfStateSOPC(uint32_t M0, ModeReg* modeReg, bool SCC)
        : M0(M0), MODE(modeReg), SCC(SCC) {}
};

struct WfStateSOPK {
    uint64_t SDST = 0;
    uint64_t SSRC0 = 0;
    uint64_t RELADDR = 0;
    ProgramCounter* PC = nullptr;
    uint32_t IMM16 = 0;
    bool SCC = false;

    WfStateSOPK(ProgramCounter* PC, bool SCC) : PC(PC), SCC(SCC) {}
};

struct WfStateSMEM {
    uint64_t BASE;
    uint64_t OFFSET;
    std::vector<uint32_t> SDST;

    WfStateSMEM(uint64_t BASE, uint64_t OFFSET, std::vector<uint32_t> SDST)
        : BASE(BASE), OFFSET(OFFSET), SDST(std::move(SDST)) {}
};

struct WfStateVOP1 {
public:
    std::vector<uint64_t> VDST;
    std::vector<uint64_t> SRC0;
    WfStateVOP1(std::vector<uint64_t> VDST, std::vector<uint64_t> SRC0)
        : VDST(std::move(VDST)), SRC0(std::move(SRC0)) {}
};

struct WfStateVOP2 {
    bool CLAMP = false;
    std::vector<uint64_t> VDST;
    std::vector<uint64_t> SRC0;
    std::vector<uint64_t> SRC1;
    uint64_t VCC;

    WfStateVOP2(std::vector<uint64_t> VDST,
                std::vector<uint64_t> SRC0,
                std::vector<uint64_t> SRC1,
                uint64_t VCC,
                bool CLAMP)
        : VDST(std::move(VDST)), SRC0(std::move(SRC0)), SRC1(std::move(SRC1)), VCC(VCC), CLAMP(CLAMP) {}
};

struct WfStateVOP3 {
    bool CLAMP = false;
    std::vector<uint64_t> VDST;
    std::vector<uint64_t> SRC0;
    std::vector<uint64_t> SRC1;
    std::vector<uint64_t> SRC2;
    uint64_t VCC = 0;

    WfStateVOP3(std::vector<uint64_t> VDST,
                std::vector<uint64_t> SRC0,
                std::vector<uint64_t> SRC1,
                std::vector<uint64_t> SRC2,
                uint64_t VCC,
                bool CLAMP)
        : VDST(std::move(VDST)),
          SRC0(std::move(SRC0)),
          SRC1(std::move(SRC1)),
          SRC2(std::move(SRC2)), VCC(VCC), CLAMP(CLAMP) {}
};

struct WfStateFLATStore {
    std::vector<uint64_t> VADDR;
    std::vector<uint32_t> VDATA;
    size_t vdataAmountPerWi = 0;
    WfStateFLATStore(std::vector<uint64_t> VADDR, std::vector<uint32_t> VDATA, size_t vdataAmountPerWi)
        : VADDR(std::move(VADDR)), VDATA(std::move(VDATA)), vdataAmountPerWi(vdataAmountPerWi) {}
};

struct WfStateFLATLoad {
    std::vector<uint64_t> VADDR;
    std::vector<uint32_t> VDST;
    size_t vdstAmountPerWi = 0;
    WfStateFLATLoad(std::vector<uint64_t> VADDR, size_t vdstAmount, size_t vdstAmountPerWi)
        : VADDR(std::move(VADDR)), VDST(std::vector<uint32_t>(vdstAmount)), vdstAmountPerWi(vdstAmountPerWi) {}
};

struct WfStateVOPC {
    uint64_t EXEC;
    uint64_t SDST;
    std::vector<uint64_t> SRC0;
    std::vector<uint64_t> SRC1;

    WfStateVOPC( uint64_t EXEC, uint64_t SDST, std::vector<uint64_t> SRC0, std::vector<uint64_t> SRC1):
        EXEC(EXEC), SDST(SDST), SRC0(std::move(SRC0)), SRC1(std::move(SRC1)) {}
};