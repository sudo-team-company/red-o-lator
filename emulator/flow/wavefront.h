//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_WAVEFRONT_H
#define RED_O_LATOR_WAVEFRONT_H

#define DEFAULT_WAVEFRONT_SIZE 64

#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>
#include "../instr/instruction.h"
#include "../reg/register.h"
#include "kernel.h"
#include "wf_state.h"

struct Wavefront;
struct WorkItem;

struct WorkGroup {
    int IDX, IDY, IDZ;
    int sizeX, sizeY, sizeZ;
    size_t size;

   public:
    KernelCode* kernelCode;
    std::vector<std::unique_ptr<WorkItem>> workItems;
    std::vector<std::unique_ptr<Wavefront>> wavefronts;

    WorkGroup(int sizeX, int sizeY, int sizeZ)
        : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {}
    int actualSizeX, actualSizeY, actualSizeZ;

    void setIds(int idX, int idY, int idZ);
    void setActualSize(int sizeX, int sizeY, int sizeZ);
};

struct Wavefront {
    WorkGroup* workGroup;
    std::vector<uint32_t> scalarRegFile;
    std::vector<uint32_t> vectorRegFile;
    uint64_t programCounter;
    uint64_t execReg;
    uint64_t vccReg;
    std::unique_ptr<StatusReg> statusReg;
    std::unique_ptr<ModeReg> modeReg;
    uint32_t m0Reg;
    int vgprsnum;
    bool sccReg;

    std::vector<WorkItem*> workItems;
    bool atBarrier;
    bool completed;

    explicit Wavefront(WorkGroup* wg, int sgprsnum, int vgprsnum)
        : workGroup(wg),
          programCounter(0),
          execReg(0),
          vccReg(0),
          statusReg(std::make_unique<StatusReg>(0)),
          modeReg(std::make_unique<ModeReg>(0)),
          vgprsnum(vgprsnum),
          atBarrier(false),
          completed(false) {
        scalarRegFile = std::vector<uint32_t>(sgprsnum);
        vectorRegFile = std::vector<uint32_t>(DEFAULT_WAVEFRONT_SIZE * vgprsnum);
    }

    Instruction* get_cur_instr();
    void to_next_instr();

    void set_v_reg(size_t wiInd, size_t vInd, uint32_t value);

    WfStateSOP1 get_sop1_state(const Instruction&);

   private:
    std::vector<uint32_t> read_operand(const Operand&);
};

struct WorkItem {
    WorkGroup* workGroup;
    int localIdX, localIdY, localIdZ;
    int globalIdX, globalIdY, globalIdZ;

    WorkItem(WorkGroup* workGroup, int idX, int idY, int idZ)
        : workGroup(workGroup),
          localIdX(idX),
          localIdY(idY),
          localIdZ(idZ),
          globalIdX(workGroup->IDX * workGroup->sizeX + idX),
          globalIdY(workGroup->IDY * workGroup->sizeY + idY),
          globalIdZ(workGroup->IDZ * workGroup->sizeZ + idZ) {}
};

#endif  // RED_O_LATOR_WAVEFRONT_H
