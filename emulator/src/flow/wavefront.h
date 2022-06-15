#pragma once

#include <common/logger/Logger.h>
#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>
#include "commons/commons.h"
#include "model/instr/instruction.h"
#include "model/kernel/KernelConfig.h"
#include "register.h"
#include "wf_state.h"

struct Wavefront;
struct WorkItem;

struct WorkGroup {
    friend class DebugContext;

   public:
    const KernelCode *const kernelCode;
    size_t sizeX, sizeY, sizeZ;
    size_t idX, idY, idZ;
    std::vector<std::unique_ptr<Wavefront>> wavefronts;

    WorkGroup(const KernelCode *const kernelCode,
              size_t sizeX, size_t sizeY, size_t sizeZ,
              size_t idX, size_t idY, size_t idZ)
        : kernelCode(kernelCode), sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ),
          idX(idX), idY(idY), idZ(idZ) {}

    void init_workitems();

    void init_wavefronts(const KernelConfig &);

    bool all_wf_completed();

    WorkItem &get_workitem_for_wf(size_t wiId, size_t wfId) const;

   private:
    std::vector<std::unique_ptr<WorkItem>> workItems;

    void init_wf_regs(Wavefront &, const KernelConfig &) const ;
};

struct Wavefront {
    friend class DebugContext;

    constexpr static auto WAVEFRONT_SIZE = 64u;

public:
    std::vector<uint32_t> scalarRegFile;
    std::vector<uint32_t> vectorRegFile;
    std::unique_ptr<StatusReg> statusReg;
    std::unique_ptr<ModeReg> modeReg;
    std::unique_ptr<ProgramCounter> programCounter;
    uint64_t execReg;
    uint64_t vccReg;
    uint32_t m0Reg = 0;
    bool sccReg = false;
    bool atBarrier = false;

    explicit Wavefront(const WorkGroup *, size_t size, size_t id, size_t sgprnum, size_t vgprnum);

    Instruction *get_cur_instr() const;

    void to_next_instr() const;

    void set_sgpr_pair(size_t, uint64_t);

    uint64_t read_sgpr_pair(size_t);

    void set_vgpr(size_t wiInd, size_t vInd, uint32_t value);

    uint32_t read_vgpr(size_t wiInd, size_t vInd);

    WfStateSOP1 get_sop1_state(const Instruction &);

    void update_with_sop1_state(const Instruction &, const WfStateSOP1 &);

    WfStateSOP2 get_sop2_state(const Instruction &);

    void update_with_sop2_state(const Instruction &, const WfStateSOP2 &);

    WfStateSOPK get_sopk_state() const;

    WfStateSOPK get_sopk_state(const Instruction &);

    void update_with_sopk_state(const Instruction &, const WfStateSOPK &);

    WfStateSOPC get_sopc_state(const Instruction &);

    void update_with_sopc_state(const WfStateSOPC &);

    WfStateSOPP get_common_sopp_state() const;

    void update_with_common_sopp_state(const Instruction &, const WfStateSOPP &);

    WfStateSMEM get_smem_state(const Instruction &);

    void update_with_smem_state(const Instruction &, const WfStateSMEM &);

    WfStateVOP1 get_vop1_state(const Instruction &);

    void update_with_vop1_state(const Instruction &, const WfStateVOP1 &);

    WfStateVOP2 get_vop2_state(const Instruction &);

    void update_with_vop2_state(const Instruction &, const WfStateVOP2 &);

    WfStateVOP3 get_vop3_state(const Instruction &);

    void update_with_vop3_state(const Instruction &, const WfStateVOP3 &);

    WfStateVOPC get_vopc_state(const Instruction &);

    void update_with_vopc_state(const Instruction &, const WfStateVOPC &);

    WfStateFLATStore get_flat_store_state(const Instruction &);

    WfStateFLATLoad get_flat_load_state(const Instruction &);

    void update_with_flat_load_state(const Instruction &, const WfStateFLATLoad &);

    std::vector<uint32_t> read_operand(const Operand &);

    std::vector<uint32_t> read_operand(const Operand &, int);

    void write_data_to_gpr(const Operand &operand, const std::vector<uint32_t> &data);

    void write_operand_to_gpr(const Operand &, uint64_t);

    void write_data_to_gpr(const Operand &operand, uint64_t data, int wiInd);

    void write_data_to_gpr(const Operand &operand, const std::vector<uint32_t> &data, int wiInd);

    bool work_item_masked(size_t wiInd) const;

    size_t get_size() const { return size; }

    void compete() { completed = true; }

    bool is_completed() const { return completed; }

    void set_exec_reg(uint64_t regValue) { execReg = regValue; }

    size_t get_local_id() const { return id; }

private:
    const WorkGroup *const workGroup = nullptr;
    size_t id, size;
    size_t sgprsnum, vgprsnum;
    bool completed = false;

    std::vector<uint32_t> read_reg_operand(const Operand &, int);
};

struct WorkItem {
public:
    WorkItem(size_t idX, size_t idY, size_t idZ)
        : localIdX(idX),
          localIdY(idY),
          localIdZ(idZ) {}

    size_t get_local_id_x() const {
        return localIdX;
    }

    size_t get_local_id_y() const {
        return localIdY;
    }

    size_t get_local_id_z() const {
        return localIdZ;
    }

private:
    size_t localIdX, localIdY, localIdZ;
};

constexpr uint64_t evaluate_exec_reg_value(size_t wfSize) {
    if (wfSize > 64) {
        assert(false && "Wavefront size can not be greater than 64");
    } else if (wfSize == 64) {
        return std::numeric_limits<uint64_t>::max();
    } else {
        return uint64_t(1ull << wfSize) - 1;
    }
}