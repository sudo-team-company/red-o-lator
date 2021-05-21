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
#include "kernel_config.h"
#include "wf_state.h"

struct Wavefront;
struct WorkItem;

struct WorkGroup {
    int IDX, IDY, IDZ;
    int sizeX, sizeY, sizeZ;

   public:
    KernelCode* kernelCode;
    std::vector<std::unique_ptr<WorkItem>> workItems;
    std::vector<std::unique_ptr<Wavefront>> wavefronts;

    int actualSizeX_, actualSizeY_, actualSizeZ_;

    WorkGroup(int sizeX, int sizeY, int sizeZ)
        : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {}

    void set_ids(int idX, int idY, int idZ);
    void set_actual_size(int sizeX, int sizeY, int sizeZ);
    bool all_wf_completed();
};

struct Wavefront {
    WorkGroup* workGroup = nullptr;
    std::vector<uint32_t> scalarRegFile;
    std::vector<uint32_t> vectorRegFile;
    uint64_t execReg;
    uint64_t vccReg;
    std::unique_ptr<ProgramCounter> programCounter;
    std::unique_ptr<StatusReg> statusReg;
    std::unique_ptr<ModeReg> modeReg;
    uint32_t m0Reg;
    int sgprsnum;
    int vgprsnum;
    bool sccReg;

    std::vector<WorkItem*> workItems;
    bool atBarrier;
    bool completed;

    explicit Wavefront(WorkGroup* wg, int sgprsnum, int vgprsnum)
        : workGroup(wg),
          execReg(0),
          vccReg(0),
          programCounter(std::make_unique<ProgramCounter>(0)),
          statusReg(std::make_unique<StatusReg>(0)),
          modeReg(std::make_unique<ModeReg>(0)),
          sgprsnum(sgprsnum),
          vgprsnum(vgprsnum),
          atBarrier(false),
          completed(false) {
        scalarRegFile = std::vector<uint32_t>(sgprsnum, uint32_t(0));
        vectorRegFile =
            std::vector<uint32_t>(DEFAULT_WAVEFRONT_SIZE * vgprsnum, uint32_t(0));
        workItems = std::vector<WorkItem*>();
    }

    Instruction* get_cur_instr() const;
    void to_next_instr();

    void set_sgpr_pair(size_t,uint64_t);
    uint64_t read_sgpr_pair(size_t);

    void set_vgpr(size_t wiInd, size_t vInd, uint32_t value);
    uint32_t read_vgpr(size_t wiInd, size_t vInd);

    WfStateSOP1 get_sop1_state(const Instruction&);
    void update_with_sop1_state(const Instruction&, const WfStateSOP1&);

    WfStateSOP2 get_sop2_state(const Instruction&);
    void update_with_sop2_state(const Instruction&, const WfStateSOP2&);

    WfStateSOPK get_sopk_state() const;
    WfStateSOPK get_sopk_state(const Instruction&);
    void update_with_sopk_state(const Instruction&, const WfStateSOPK&);

    WfStateSOPC get_sopc_state(const Instruction&);
    void update_with_sopc_state(const WfStateSOPC&);

    WfStateSOPP get_common_sopp_state(const Instruction&) const;
    void update_with_common_sopp_state(const Instruction&,
                                       const WfStateSOPP&);

    WfStateSMEM get_smem_state(const Instruction&);
    void update_with_smem_state(const Instruction&, const WfStateSMEM&);

    WfStateVOP1 get_vop1_state(const Instruction&);
    void update_with_vop1_state(const Instruction&, const WfStateVOP1&);

    WfStateVOP2 get_vop2_state(const Instruction&);
    void update_with_vop2_state(const Instruction&, const WfStateVOP2&);

    WfStateVOP3 get_vop3_state(const Instruction&);
    void update_with_vop3_state(const Instruction&, const WfStateVOP3&);

    std::vector<uint32_t> read_operand(const Operand&);
    std::vector<uint32_t> read_operand(const Operand&, int);
    void write_operand_to_gpr(const Operand&, const std::vector<uint32_t>&);
    void write_operand_to_gpr(const Operand&, const std::vector<uint32_t>&, int);
    void write_operand_to_gpr(const Operand&, uint64_t);
    void write_operand_to_gpr(const Operand&, uint64_t, int);

    bool work_item_masked(size_t wiInd) const;

   private:
    std::vector<uint32_t> read_reg_operand(const Operand&, int);
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
