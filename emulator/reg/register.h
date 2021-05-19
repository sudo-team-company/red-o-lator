//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_REGISTER_H
#define RED_O_LATOR_REGISTER_H

#include <bitset>
#include <unordered_map>
#include "reg_info.h"

constexpr bool is_valid_register_ind(int i) {
    return i >= PC && i <= V255;
}

constexpr bool is_s_reg(RegisterType type) {
    return type >= S0 && type <= S101;
}

constexpr bool is_v_reg(RegisterType type) {
    return type >= V0 && type <= V255;
}

struct ModeReg {
    std::bitset<32> value;

    ModeReg(uint32_t);

    uint8_t fp_round();   //[3:0] bits
    uint8_t fp_denorm();  //[7:4]
    bool dx10_clamp();    // 8
    bool ieee();          // 9
    bool lod_clamped();   // 10
    bool debug();         // 11
    uint8_t excp_en();    //[18:12]
    bool fp16_ovfl();     // 23
    bool pops_packer0();  // 24
    bool pops_packer1();  // 25
    bool disable_perf();  // 26
    bool gpr_idx_en();    // 27
    bool vskip();         // 28
    uint8_t csp();        //[31:29]

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
};

struct StatusReg {
    std::bitset<32> value;

    StatusReg(uint32_t);

    /**
     * Scalar condition code. Used as a carry-out bit. For a comparison instruction,
     * this bit indicates failure or success. For logical operations, this is 1 if the
     * result was non-zero.
     */
    bool scc();            // 0 - in docs 1, mb it is mistake
    /**
     * Wavefront priority set by the shader processor interpolator (SPI) when the
     * wavefront is created. See the S_SETPRIO instruction for
     * details. 0 is lowest, 3 is highest priority
     */
    uint8_t spi_prio() const;    //[2:1]
    /**
     * Wavefront priority set by the shader program. See the S_SETPRIO
     * instruction (page 12-49) for details
     */
    uint8_t wave_prio() const;   //[4:3]
    /**
     * Privileged mode. Can only be active when in the trap handler. Gives write
     * access to the TTMP, TMA, and TBA registers
     */
    bool priv();           // 5
    void priv(bool);
    /**
     * Indicates that a trap handler is present. When set to zero, traps are not taken.
     */
    bool trap_en();        // 6
    /**
     * Indicates whether thread trace is enabled for this wavefront. If zero, also
     * ignore any shader-generated (instruction) thread-trace data
     */
    bool ttrace_en();      // 7
    /**
     * This status bit indicates if export buffer space has been allocated. The
     * shader stalls any export instruction until this bit becomes 1. It is set to 1
     * when export buffer space has been allocated. Before a Pixel or Vertex
     * shader can export, the hardware checks the state of this bit. If the bit is 1,
     * export can be issued. If the bit is zero, the wavefront sleeps until space
     * becomes available in the export buffer. Then, this bit is set to 1, and the
     * wavefront resumes.
     */
    bool export_rdy();     // 8
    /**
     * Exec mask is zero
     */
    bool execz();          // 9
    /**
     * Vector condition code is zero.
     */
    bool vccz();           // 10
    /**
     * Wavefront is a member of a work-group of more than one wavefront.
     */
    bool in_tg();          // 11
    /**
     * Wavefront is waiting at a barrier.
     */
    bool in_barrier();     // 12
    /**
     * Wavefront is halted or scheduled to halt. HALT can be set by the host
     * through wavefront-control messages, or by the shader. This bit is ignored
     * while in the trap handler (PRIV = 1); it also is ignored if a host-initiated trap
     * is received (request to enter the trap handler).
     */
    bool halt();           // 13

    bool halt(bool);
    /**
     * Wavefront is flagged to enter the trap handler as soon as possible
     */
    bool trap();           // 14
    /**
     * Enables/disables thread trace for this compute unit (CU). This bit allows
     * more than one CU to be outputting USERDATA (shader initiated writes to
     * the thread-trace buffer). Note that wavefront data is only traced from one
     * CU per shader array. Wavefront user data (instruction based) can be output
     * if this bit is zero.
     */
    bool ttrace_cu_en();   // 15
    /**
     * Wavefront is active (has been created and not yet ended)  .
     */
    bool valid();          // 16
    /**
     * An ECC error has occurred
     */
    bool ecc_err();        // 17
    /**
     * For Vertex Shaders only. 1 = this shader is not allocated export buffer
     * space; all export instructions are ignored (treated as NOPs). Formerly
     * called VS_NO_ALLOC. Used for stream-out of multiple streams (multiple
     * passes over the same VS), and for DS running in the VS stage for
     * wavefronts that produced no primitives.
     */
    bool skip_export();    // 18
    /**
     * Performance counters are enabled for this wavefront.
     */
    bool perf_en();        // 19
    /**
     * Conditional debug indicator for user mode.
     */
    bool cond_dbg_user();  // 20
    /**
     * Conditional debug indicator for system mode.
     */
    bool cond_dbg_sys();   // 21
    /**
     * Indicates that ATC replay is enabled.
     */
    bool allow_replay();   // 22
    /**
     * This wavefront is required to perform an export with Done=1 before
     * terminating.
     */
    bool must_export();    // 27
};

int get_register_size(RegisterType registerType);

#endif  // RED_O_LATOR_REGISTER_H
