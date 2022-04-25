#pragma once

#include <bitset>
#include <unordered_map>
#include "reg_info.h"

constexpr bool is_s_reg(RegisterType type) {
    return type >= S0 && type <= S103;
}

constexpr bool is_v_reg(RegisterType type) {
    return type >= V0 && type <= V255;
}

struct ProgramCounter {

    void set_value(uint64_t value) {
        value_ = value;
        used_ = true;
    }

    void add(uint64_t value) {
        value_ += value;
        used_ = true;
    }

    void hidden_add(uint64_t value) {
        value_ += value;
    }

    uint64_t get_value() const {
        return value_;
    }

    bool was_used() {
        bool temp = used_;
        used_ = false;
        return temp;
    }

    explicit ProgramCounter(uint64_t value) : value_(value), used_(false) {}

   private:
    uint64_t value_ = 0;
    bool used_ = false;
};

struct ModeReg {
public:
    explicit ModeReg(uint32_t);

    uint8_t fp_round() const;   //[3:0] bits
    uint8_t fp_denorm() const;  //[7:4]
    bool dx10_clamp() const;    // 8
    bool ieee() const;          // 9
    bool lod_clamped() const;   // 10
    bool debug() const;         // 11
    uint8_t excp_en() const;    //[18:12]
    bool fp16_ovfl() const;     // 23
    bool pops_packer0() const;  // 24
    bool pops_packer1() const;  // 25
    bool disable_perf() const;  // 26
    bool gpr_idx_en() const;    // 27
    bool vskip() const;         // 28
    uint8_t csp() const;        //[31:29]

    void fp_round(uint8_t);
    void fp_denorm(uint8_t);
    void dx10_clamp(bool);
    void ieee(bool);
    void lod_clamped(bool);
    void debug(bool);
    void excp_en(uint8_t);
    void fp16_ovfl(bool);
    void pops_packer0(bool);
    void pops_packer1(bool);
    void disable_perf(bool);
    void gpr_idx_en(bool);
    void vskip(bool);
    void csp(uint8_t);

private:
    std::bitset<32> value;
};

struct StatusReg {
public:
    explicit StatusReg(uint32_t);

    /**
     * Scalar condition code. Used as a carry-out bit. For a comparison instruction,
     * this bit indicates failure or success. For logical operations, this is 1 if the
     * result was non-zero.
     */
    bool scc() const;  //0
    void scc(bool);
    /**
     * Wavefront priority set by the shader processor interpolator (SPI) when the
     * wavefront is created. See the S_SETPRIO instruction for
     * details. 0 is lowest, 3 is highest priority
     */
    uint8_t spi_prio() const;  //[2:1]
    /**
     * Wavefront priority set by the shader program. See the S_SETPRIO
     * instruction (page 12-49) for details
     */
    uint8_t wave_prio() const;  //[4:3]
    /**
     * Privileged mode. Can only be active when in the trap handler. Gives write
     * access to the TTMP, TMA, and TBA registers
     */
    bool priv() const;  // 5
    void priv(bool);
    /**
     * Indicates that a trap handler is present. When set to zero, traps are not taken.
     */
    bool trap_en() const;  // 6
    /**
     * Indicates whether thread trace is enabled for this wavefront. If zero, also
     * ignore any shader-generated (instruction) thread-trace data
     */
    bool ttrace_en() const;  // 7
    /**
     * This status bit indicates if export buffer space has been allocated. The
     * shader stalls any export instruction until this bit becomes 1. It is set to 1
     * when export buffer space has been allocated. Before a Pixel or Vertex
     * shader can export, the hardware checks the state of this bit. If the bit is 1,
     * export can be issued. If the bit is zero, the wavefront sleeps until space
     * becomes available in the export buffer. Then, this bit is set to 1, and the
     * wavefront resumes.
     */
    bool export_rdy() const;  // 8
    /**
     * Exec mask is zero
     */
    bool execz() const;  // 9
    /**
     * Vector condition code is zero.
     */
    bool vccz() const;  // 10
    /**
     * Wavefront is a member of a work-group of more than one wavefront.
     */
    bool in_tg() const;  // 11
    /**
     * Wavefront is waiting at a barrier.
     */
    bool in_barrier() const;  // 12
    /**
     * Wavefront is halted or scheduled to halt. HALT can be set by the host
     * through wavefront-control messages, or by the shader. This bit is ignored
     * while in the trap handler (PRIV = 1); it also is ignored if a host-initiated trap
     * is received (request to enter the trap handler).
     */
    bool halt() const;  // 13

    bool halt(bool);
    /**
     * Wavefront is flagged to enter the trap handler as soon as possible
     */
    bool trap() const;  // 14
    /**
     * Enables/disables thread trace for this compute unit (CU). This bit allows
     * more than one CU to be outputting USERDATA (shader initiated writes to
     * the thread-trace buffer). Note that wavefront data is only traced from one
     * CU per shader array. Wavefront user data (instruction based) can be output
     * if this bit is zero.
     */
    bool ttrace_cu_en() const;  // 15
    /**
     * Wavefront is active (has been created and not yet ended)  .
     */
    bool valid() const;  // 16
    /**
     * An ECC error has occurred
     */
    bool ecc_err() const;  // 17
    /**
     * For Vertex Shaders only. 1 = this shader is not allocated export buffer
     * space; all export instructions are ignored (treated as NOPs). Formerly
     * called VS_NO_ALLOC. Used for stream-out of multiple streams (multiple
     * passes over the same VS), and for DS running in the VS stage for
     * wavefronts that produced no primitives.
     */
    bool skip_export() const;  // 18
    /**
     * Performance counters are enabled for this wavefront.
     */
    bool perf_en() const;  // 19
    /**
     * Conditional debug indicator for user mode.
     */
    bool cond_dbg_user() const;  // 20
    /**
     * Conditional debug indicator for system mode.
     */
    bool cond_dbg_sys() const;  // 21
    /**
     * Indicates that ATC replay is enabled.
     */
    bool allow_replay() const;  // 22
    /**
     * This wavefront is required to perform an export with Done=1 before
     * terminating.
     */
    bool must_export() const;  // 27

private:
    std::bitset<32> value;
};
