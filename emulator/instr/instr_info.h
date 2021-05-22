//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_INSTR_INFO_H
#define RED_O_LATOR_INSTR_INFO_H

#include <unordered_map>
#include <cassert>
#include <string>

enum InstrKey {

    // SMEM
    /**
     * Syntax: S_ATOMIC_ADD SDATA, SBASE(2), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM;
     * *VM = *VM + SDATA;
     * SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_ADD,
    /**
     * Syntax: S_ATOMIC_ADD_X2 SDATA(2), SBASE(2), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = *VM + SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_ADD_X2,
    /**
     * Syntax: S_ATOMIC_AND SDATA, SBASE(2), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = *VM & SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_AND,
    /**
     * Syntax: S_ATOMIC_AND_X2 SDATA(2), SBASE(2), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = *VM & SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_AND_X2,
    /**
     *  Syntax: S_ATOMIC_CMPSWAP SDATA(2), SBASE(2), OFFSET
     *  Operation:
     *  UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     *  UINT32 P = *VM; *VM = *VM = *VM==(SDATA>>32) ? SDATA&0xffffffff : *VM
     * //atomic SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_CMPSWAP,
    /**
     * Syntax: S_ATOMIC_CMPSWAP_X2 SDATA(4), SBASE(2), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = *VM = *VM==(SDATA[2:3]) ? SDATA[0:1] : *VM //
     * atomic SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_CMPSWAP_X2,
    /**
     *  Syntax: S_ATOMIC_DEC SDATA, SBASE(2), OFFSET
     *  Operation:
     *  UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     *  UINT32 P = *VM; *VM = (*VM <= VDATA && *VM!=0) ? *VM-1 : VDATA; //
     * atomic SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_DEC,
    /**
     * Syntax: S_ATOMIC_DEC_X2 SDATA, SBASE(2), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = (*VM <= VDATA && *VM!=0) ? *VM-1 : VDATA; // atomic
     * SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_DEC_X2,
    /**
     * Syntax: S_ATOMIC_INC SDATA, SBASE(2), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = (*VM < SDATA) ? *VM+1 : 0; SDATA = (GLC) ? P :
     * SDATA // atomic
     */
    S_ATOMIC_INC,
    /**
     *  Syntax: S_ATOMIC_INC_X2 SDATA(2), SBASE(2), OFFSET
     *  Operation:
     *  UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     *  UINT64 P = *VM; *VM = (*VM < SDATA) ? *VM+1 : 0; SDATA = (GLC) ? P :
     * SDATA // atomic
     */
    S_ATOMIC_INC_X2,
    /**
     * Syntax: S_ATOMIC_OR SDATA, SBASE(2), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = *VM | SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_OR,
    /**
     * Syntax: S_ATOMIC_OR_X2 SDATA(2), SBASE(2), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = *VM | SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_OR_X2,
    /**
     * Syntax: S_ATOMIC_SMAX SDATA, SBASE(2), OFFSET
     * Operation:
     * INT32* VM = (INT32*)((SMEM + (OFFSET & ~3))
     * INT32 P = *VM; *VM = MAX(*VM, (INT32)SDATA); SDATA = (GLC) ? P : SDATA
     * //atomic
     */
    S_ATOMIC_SMAX,
    /**
     * Syntax: S_ATOMIC_SMAX_X2 SDATA(2), SBASE(2), OFFSET
     * Operation:
     * INT64* VM = (INT64*)((SMEM + (OFFSET & ~3))
     * INT64 P = *VM; *VM = MAX(*VM, (INT64)SDATA); SDATA = (GLC) ? P : SDATA
     * //atomic
     */
    S_ATOMIC_SMAX_X2,
    /**
     * Syntax: S_ATOMIC_SMIN SDATA, SBASE(2), OFFSET
     * Operation:
     * INT32* VM = (INT32*)((SMEM + (OFFSET & ~3))
     * INT32 P = *VM; *VM = MIN(*VM, (INT32)SDATA); SDATA = (GLC) ? P : SDATA
     * //atomic
     */
    S_ATOMIC_SMIN,
    /**
     * Syntax: S_ATOMIC_SMIN_X2 SDATA(2), SBASE(2), OFFSET
     * Operation:
     * INT64* VM = (INT64*)((SMEM + (OFFSET & ~3))
     * INT64 P = *VM; *VM = MIN(*VM, (INT64)SDATA); SDATA = (GLC) ? P : SDATA //
     * atomic
     */
    S_ATOMIC_SMIN_X2,
    /**
     * Syntax: S_ATOMIC_SUB SDATA, SBASE(2), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = *VM - SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_SUB,
    /**
     * Syntax: S_ATOMIC_SUB_X2 SDATA(2), SBASE(2), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = *VM - SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_SUB_X2,
    /**
     * Syntax: S_ATOMIC_SWAP SDATA, SBASE(2), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_SWAP,
    /**
     * Syntax: S_ATOMIC_SWAP_X2 SDATA(2), SBASE(2), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_SWAP_X2,
    /**
     * Syntax: S_ATOMIC_UMAX SDATA, SBASE(2), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = MAX(*VM, SDATA); SDATA = (GLC) ? P : SDATA //
     * atomic
     */
    S_ATOMIC_UMAX,
    /**
     * Syntax: S_ATOMIC_UMAX_X2 SDATA(2), SBASE(2), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = MAX(*VM, SDATA); SDATA = (GLC) ? P : SDATA //atomic
     */
    S_ATOMIC_UMAX_X2,
    /**
     * Syntax: S_ATOMIC_UMIN SDATA, SBASE(2), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = MIN(*VM, SDATA); SDATA = (GLC) ? P : SDATA //
     * atomic
     */
    S_ATOMIC_UMIN,
    /**
     * Syntax: S_ATOMIC_UMIN_X2 SDATA(2), SBASE(2), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = MIN(*VM, SDATA); SDATA = (GLC) ? P : SDATA //
     * atomic
     */
    S_ATOMIC_UMIN_X2,
    /**
     * Syntax: S_ATOMIC_XOR SDATA, SBASE(2), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = *VM ^ SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_XOR,
    /**
     * Syntax: S_ATOMIC_XOR_X2 SDATA(2), SBASE(2), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = *VM ^ SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_ATOMIC_XOR_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_ADD SDATA, SBASE(4), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = *VM + SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_ADD,
    /**
     * Syntax: S_BUFFER_ATOMIC_ADD_X2 SDATA(2), SBASE(4), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = *VM + SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_ADD_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_AND SDATA, SBASE(4), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = *VM & SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_AND,
    /**
     *  Syntax: S_BUFFER_ATOMIC_AND_X2 SDATA(2), SBASE(4), OFFSET
     *  Operation:
     *  UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     *  UINT64 P = *VM; *VM = *VM & SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_AND_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_CMPSWAP SDATA(2), SBASE(4), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = *VM = *VM==(SDATA>>32) ? SDATA&0xffffffff : *VM
     * //atomic SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_CMPSWAP,
    /**
     * Syntax: S_BUFFER_ATOMIC_CMPSWAP_X2 SDATA(4), SBASE(4), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = *VM = *VM==(SDATA[2:3]) ? SDATA[0:1] : *VM //
     * atomic SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_CMPSWAP_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_DEC SDATA, SBASE(4), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = (*VM <= VDATA && *VM!=0) ? *VM-1 : VDATA; // atomic
     * SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_DEC,
    /**
     * Syntax: S_BUFFER_ATOMIC_DEC_X2 SDATA, SBASE(4), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = (*VM <= VDATA && *VM!=0) ? *VM-1 : VDATA; // atomic
     * SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_DEC_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_INC SDATA, SBASE(4), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = (*VM < SDATA) ? *VM+1 : 0; SDATA = (GLC) ? P :
     * SDATA // atomic
     */
    S_BUFFER_ATOMIC_INC,
    /**
     * Syntax: S_BUFFER_ATOMIC_INC_X2 SDATA(2), SBASE(4), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = (*VM < SDATA) ? *VM+1 : 0; SDATA = (GLC) ? P :
     * SDATA // atomic
     */
    S_BUFFER_ATOMIC_INC_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_OR SDATA, SBASE(4), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = *VM | SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_OR,
    /**
     * Syntax: S_BUFFER_ATOMIC_OR_X2 SDATA(2), SBASE(4), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = *VM | SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_OR_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_SMAX SDATA, SBASE(4), OFFSET
     * Operation:
     * INT32* VM = (INT32*)((SMEM + (OFFSET & ~3))
     * INT32 P = *VM; *VM = MAX(*VM, (INT32)SDATA); SDATA = (GLC) ? P : SDATA
     * //atomic
     */
    S_BUFFER_ATOMIC_SMAX,
    /**
     * Syntax: S_BUFFER_ATOMIC_SMAX_X2 SDATA(2), SBASE(4), OFFSET
     * Operation:
     * INT64* VM = (INT64*)((SMEM + (OFFSET & ~3))
     * INT64 P = *VM; *VM = MAX(*VM, (INT64)SDATA); SDATA = (GLC) ? P : SDATA
     * //atomic
     */
    S_BUFFER_ATOMIC_SMAX_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_SMIN SDATA, SBASE(4), OFFSET
     * Operation:
     * INT32* VM = (INT32*)((SMEM + (OFFSET & ~3))
     * INT32 P = *VM; *VM = MIN(*VM, (INT32)SDATA); SDATA = (GLC) ? P : SDATA
     * //atomic
     */
    S_BUFFER_ATOMIC_SMIN,
    /**
     * Syntax: S_BUFFER_ATOMIC_SMIN_X2 SDATA(2), SBASE(4), OFFSET
     * Operation:
     * INT64* VM = (INT64*)((SMEM + (OFFSET & ~3))
     * INT64 P = *VM; *VM = MIN(*VM, (INT64)SDATA); SDATA = (GLC) ? P : SDATA
     * //atomic
     */
    S_BUFFER_ATOMIC_SMIN_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_SUB SDATA, SBASE(4), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = *VM - SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_SUB,
    /**
     * Syntax: S_BUFFER_ATOMIC_SUB_X2 SDATA(2), SBASE(4), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = *VM - SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_SUB_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_SWAP SDATA, SBASE(4), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_SWAP,
    /**
     * Syntax: S_BUFFER_ATOMIC_SWAP_X2 SDATA(2), SBASE(4), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_SWAP_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_UMAX SDATA, SBASE(4), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = MAX(*VM, SDATA); SDATA = (GLC) ? P : SDATA //
     * atomic
     */
    S_BUFFER_ATOMIC_UMAX,
    /**
     * Syntax: S_BUFFER_ATOMIC_UMAX_X2 SDATA(2), SBASE(4), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = MAX(*VM, SDATA); SDATA = (GLC) ? P : SDATA //
     * atomic
     */
    S_BUFFER_ATOMIC_UMAX_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_UMIN SDATA, SBASE(4), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = MIN(*VM, SDATA); SDATA = (GLC) ? P : SDATA //
     * atomic
     */
    S_BUFFER_ATOMIC_UMIN,
    /**
     * Syntax: S_BUFFER_ATOMIC_UMIN_X2 SDATA(2), SBASE(4), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = MIN(*VM, SDATA); SDATA = (GLC) ? P : SDATA //
     * atomic
     */
    S_BUFFER_ATOMIC_UMIN_X2,
    /**
     * Syntax: S_BUFFER_ATOMIC_XOR SDATA, SBASE(4), OFFSET
     * Operation:
     * UINT32* VM = (UINT32*)((SMEM + (OFFSET & ~3))
     * UINT32 P = *VM; *VM = *VM ^ SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_XOR,
    /**
     * Syntax: S_BUFFER_ATOMIC_XOR_X2 SDATA(2), SBASE(4), OFFSET
     * Operation:
     * UINT64* VM = (UINT64*)((SMEM + (OFFSET & ~3))
     * UINT64 P = *VM; *VM = *VM ^ SDATA; SDATA = (GLC) ? P : SDATA // atomic
     */
    S_BUFFER_ATOMIC_XOR_X2,
    /**
     * Syntax: S_BUFFER_LOAD_DWORD SDATA, SBASE(4), OFFSET
     * Operation:
     * SDATA = *(UINT32*)(SMEM + (OFFSET & ~3))
     */
    S_BUFFER_LOAD_DWORD,
    /**
     * Syntax: S_BUFFER_LOAD_DWORDX16 SDATA(16), SBASE(4), OFFSET
     * Operation:
     * for (BYTE i = 0; i < 16; i++)
     * SDATA[i] = *(UINT32*)(SMEM + i*4 + (OFFSET & ~3))
     */
    S_BUFFER_LOAD_DWORDX16,
    /**
     * Syntax: S_BUFFER_LOAD_DWORDX2 SDATA(2), SBASE(4), OFFSET
     * Operation:
     * SDATA = *(UINT64*)(SMEM + (OFFSET & ~3))
     */
    S_BUFFER_LOAD_DWORDX2,
    /**
     * Syntax: S_BUFFER_LOAD_DWORDX4 SDATA(4), SBASE(4), OFFSET
     * Operation:
     * for (BYTE i = 0; i < 4; i++)
     * SDATA[i] = *(UINT32*)(SMEM + i*4 + (OFFSET & ~3))
     */
    S_BUFFER_LOAD_DWORDX4,
    /**
     * Syntax: S_BUFFER_LOAD_DWORDX8 SDATA(8), SBASE(4), OFFSET
     * Operation:
     * for (BYTE i = 0; i < 8; i++)
     * SDATA[i] = *(UINT32*)(SMEM + i*4 + (OFFSET & ~3))
     */
    S_BUFFER_LOAD_DWORDX8,
    /**
     * Syntax: S_BUFFER_STORE_DWORD SDATA, SBASE(4), OFFSET
     * Operation:
     * *(UINT32*)(SMEM + (OFFSET & ~3)) = SDATA
     */
    S_BUFFER_STORE_DWORD,
    /**
     *  Syntax: S_BUFFER_STORE_DWORDX2 SDATA(2), SBASE(4), OFFSET
     *  Operation:
     *  *(UINT64*)(SMEM + (OFFSET & ~3)) = SDATA
     */
    S_BUFFER_STORE_DWORDX2,
    /**
     * Syntax: S_BUFFER_STORE_DWORDX4 SDATA(4), SBASE(4), OFFSET
     * Operation:
     * for (BYTE i = 0; i < 4; i++)
     * *(UINT32*)(SMEM + i*4 + (OFFSET & ~3)) = SDATA[i]
     */
    S_BUFFER_STORE_DWORDX4,
    /**
     * Syntax: S_DCACHE_DISCARD SBASE(2), SOFFSET1
     * Description: Discard one dirty scalar data cache line. A cache line is 64
     * bytes. Address calculated as S_STORE_DWORD with alignment to 64-byte
     * boundary. LGKM count is incremented by 1 for this opcode.
     */
    S_DCACHE_DISCARD,
    /**
     * Syntax: S_DCACHE_DISCARD_X2 SBASE(2), SOFFSET1
     * Description: Discard two dirty scalar data cache lines. A cache line is
     * 64 bytes. Address calculated as S_STORE_DWORD with alignment to 64-byte
     * boundary. LGKM count is incremented by 1 for this opcode.
     */
    S_DCACHE_DISCARD_X2,
    /**
     * Syntax: S_DCACHE_INV
     * Description: Invalidate entire L1 K cache.
     */
    S_DCACHE_INV,
    /**
     * Syntax: S_DCACHE_INV_VOL
     * Description: Invalidate all volatile lines in L1 K cache.
     */
    S_DCACHE_INV_VOL,
    /**
     * Syntax: S_LOAD_DWORD SDATA, SBASE(2), OFFSET
     * Operation:
     * SDATA = *(UINT32*)(SMEM + (OFFSET & ~3))
     */
    S_LOAD_DWORD,
    /**
     * Syntax: S_LOAD_DWORDX16 SDATA(16), SBASE(2), OFFSET
     * Operation:
     * for (BYTE i = 0; i < 16; i++)
     * SDATA[i] = *(UINT32*)(SMEM + i*4 + (OFFSET & ~3))
     */
    S_LOAD_DWORDX16,
    /**
     *  Syntax: S_LOAD_DWORDX2 SDATA(2), SBASE(2), OFFSET
     *  SDATA = *(UINT64*)(SMEM + (OFFSET & ~3))
     */
    S_LOAD_DWORDX2,
    /**
     * Syntax: S_LOAD_DWORDX4 SDATA(4), SBASE(2), OFFSET
     * Operation:
     * for (BYTE i = 0; i < 4; i++)
     *      SDATA[i] = *(UINT32*)(SMEM + i*4 + (OFFSET & ~3))
     */
    S_LOAD_DWORDX4,
    /**
     * Syntax: S_LOAD_DWORDX8 SDATA(8), SBASE(2), OFFSET
     * Operation:
     * for (BYTE i = 0; i < 8; i++)
     *      SDATA[i] = *(UINT32*)(SMEM + i*4 + (OFFSET & ~3))
     */
    S_LOAD_DWORDX8,
    /**
     * Syntax: S_MEMREALTIME SDATA(2)
     * Operation:
     * SDATA = CLOCKCNT
     */
    S_MEMREALTIME,
    /**
     * Syntax: S_MEMTIME SDATA(2)
     * Operation:
     * SDATA = CLOCKCNT
     */
    S_MEMTIME,
    /**
     * Syntax: S_SCRATCH_LOAD_DWORD SDATA, SBASE(2), SGPROFFSET OFFSET:OFFSET
     * Operation:
     * SDATA = *(UINT32*)(SMEM + (OFFSET & ~3) + (SGPROFFSET & ~3)*64)
     */
    S_SCRATCH_LOAD_DWORD,
    /**
     * Syntax: S_SCRATCH_LOAD_DWORDX2 SDATA, SBASE(2), SGPROFFSET OFFSET:OFFSET
     * Operation:
     * SDATA = *(UINT64*)(SMEM + (OFFSET & ~3) + (SGPROFFSET & ~3)*64)
     */
    S_SCRATCH_LOAD_DWORDX2,
    /**
     * Syntax: S_SCRATCH_LOAD_DWORDX4 SDATA, SBASE(2), SGPROFFSET OFFSET:OFFSET
     * Operation:
     * for (BYTE i = 0; i < 4; i++)
     * SDATA[i] = *(UINT32*)(SMEM + i*4 + (OFFSET & ~3) + (SGPROFFSET & ~3)*64)
     */
    S_SCRATCH_LOAD_DWORDX4,
    /**
     * Syntax: S_SCRATCH_STORE_DWORD SDATA, SBASE(2), SGPROFFSET OFFSET:OFFSET
     * Operation:
     * *(UINT32*)(SMEM + (OFFSET & ~3) + (SGPROFFSET & ~3)*64) = SDATA
     */
    S_SCRATCH_STORE_DWORD,
    /**
     * Syntax: S_SCRATCH_STORE_DWORDX2 SDATA(2), SBASE(2), SGPROFFSET
     * OFFSET:OFFSET Operation:
     * *(UINT64*)(SMEM + (OFFSET & ~3) + (SGPROFFSET & ~3)*64) = SDATA
     */
    S_SCRATCH_STORE_DWORDX2,
    /**
     * Syntax: S_SCRATCH_STORE_DWORDX4 SDATA(4), SBASE(2), SGPROFFSET
     * OFFSET:OFFSET Operation: for (BYTE i = 0; i < 4; i++)
     * *(UINT32*)(SMEM + i*4 + (OFFSET & ~3) + (SGPROFFSET & ~3)*64) = SDATA[i]
     */
    S_SCRATCH_STORE_DWORDX4,
    /**
     * Syntax: S_STORE_DWORD SDATA, SBASE(2), OFFSET
     * Operation:
     * *(UINT32*)(SMEM + (OFFSET & ~3)) = SDATA
     */
    S_STORE_DWORD,
    /**
     *  Syntax: S_STORE_DWORDX2 SDATA(2), SBASE(2), OFFSET
     *  Operation:
     *  *(UINT64*)(SMEM + (OFFSET & ~3)) = SDATA
     */
    S_STORE_DWORDX2,

    /**
     * Syntax: S_STORE_DWORDX4 SDATA(4), SBASE(2), OFFSET
     * Operation:
     * for (BYTE i = 0; i < 4; i++)
     * *(UINT32*)(SMEM + i*4 + (OFFSET & ~3)) = SDATA[i]
     */
    S_STORE_DWORDX4,
    // END SMEM

    // SOPC
    /**
     * Syntax: S_BITCMP0_B32 SSRC0, SSRC1
     * Operation:
     * SCC = (SSRC0 & (1U << (SSRC1&31))) == 0
     */
    S_BITCMP0_B32,
    /**
     * Syntax: S_BITCMP0_B64 SSRC0(2), SSRC1
     *Operation:
     * SCC = (SSRC0 & (1ULL << (SSRC1&63))) == 0
     */
    S_BITCMP0_B64,
    /**
     * Syntax: S_BITCMP1_B32 SSRC0, SSRC1
     * Operation:
     * SCC = (SSRC0 & (1U << (SSRC1&31))) != 0
     */
    S_BITCMP1_B32,
    /**
     * Syntax: S_BITCMP1_B64 SSRC0(2), SSRC1
     * Operation:
     * SCC = (SSRC0 & (1ULL << (SSRC1&63))) != 0
     */
    S_BITCMP1_B64,
    /**
     * Syntax: S_CMP_EQ_I32 SSRC0, SSRC1
     * Operation:
     * SCC = SSRC0==SSRC1
     */
    S_CMP_EQ_I32,
    /**
     * Syntax: S_CMP_EQ_U32 SSRC0, SSRC1
     * Operation:
     * SCC = SSRC0==SSRC1
     */
    S_CMP_EQ_U32,
    /**
     * Syntax: S_CMP_EQ_U64 SSRC0(2), SSRC1(2)
     * Operation:
     * SCC = SSRC0==SSRC1
     */
    S_CMP_EQ_U64,
    /**
     * Syntax: S_CMP_GE_I32 SSRC0, SSRC1
     * Operation:
     * SCC = (INT32)SSRC0 >= (INT32)SSRC1
     */
    S_CMP_GE_I32,
    /**
     * Syntax: S_CMP_GE_U32 SSRC0, SSRC1
     * Operation:
     * SCC = SSRC0 >= SSRC1
     */
    S_CMP_GE_U32,
    /**
     * Syntax: S_CMP_GT_I32 SSRC0, SSRC1
     * Operation:
     * SCC = (INT32)SSRC0 > (INT32)SSRC1
     */
    S_CMP_GT_I32,
    /**
     * Syntax: S_CMP_GT_U32 SSRC0, SSRC1
     * Operation:
     * SCC = SSRC0 > SSRC1
     */
    S_CMP_GT_U32,
    /**
     * Syntax: S_CMP_LE_I32 SSRC0, SSRC1
     * Operation:
     * SCC = (INT32)SSRC0 <= (INT32)SSRC1
     */
    S_CMP_LE_I32,
    /**
     * Syntax: S_CMP_LE_U32 SSRC0, SSRC1
     * Operation:
     * SCC = SSRC0 <= SSRC1
     */
    S_CMP_LE_U32,
    /**
     * Syntax: S_CMP_LG_I32 SSRC0, SSRC1
     * Operation:
     * SCC = SSRC0!=SSRC1
     */
    S_CMP_LG_I32,
    /**
     *   Syntax: S_CMP_LG_I32 SSRC0, SSRC1
     *   Operation:
     *   SCC = SSRC0!=SSRC1
     */
    S_CMP_LG_U32,
    /**
     * Syntax: S_CMP_LG_U64 SSRC0(2), SSRC1(2)
     * Operation:
     * SCC = SSRC0!=SSRC1
     */
    S_CMP_LG_U64,
    /**
     * Syntax: S_CMP_NE_U64 SSRC0(2), SSRC1(2)
     * Operation:
     * SCC = SSRC0!=SSRC1
     */
    S_CMP_NE_U64,
    /**
     * Syntax: S_CMP_LT_I32 SSRC0, SSRC1
     * Operation:
     * SCC = (INT32)SSRC0 < (INT32)SSRC1
     */
    S_CMP_LT_I32,
    /**
     * Syntax: S_CMP_LT_U32 SSRC0, SSRC1
     * Operation:
     * SCC = SSRC0 < SSRC1
     */
    S_CMP_LT_U32,
    /**
     * Syntax:S_SET_GPR_IDX_ON SSRC0(0), IMM8
     * Description:  Enable GPR indexing mode. Vector operations after this will
     * perform relative GPR addressing based on the contents of m0Reg_. The
     * structure SQ_M0_GPR_IDX_WORD may be used to decode m0Reg_. The raw
     * contents of the S1 field are read and used to set the enable bits.
     * S1[0] = VSRC0_REL, S1[1] = VSRC1_REL, S1[2] = VSRC2_REL and S1[3]
     * = VDST_REL
     * Operation:
     * MODE = (MODE & ~(1U<<27)) | (1U<<27)
     * m0Reg_ = (m0Reg_ & 0xffff0f00) | ((IMM8 & 15)<<12) | (SSRC0 & 0xff)
     */
    S_SET_GPR_IDX_ON,
    /**
     * Syntax: S_SETVSKIP SSRC0, SSRC1
     * Operation:
     * VSKIP = (SSRC0 & 1<<(SSRC1&31)) != 0
     */
    S_SETVSKIP,
    // END SOPC

    // SOPP
    /**
     * Syntax: S_BARRIER
     * Description: Synchronize waves within workgroup.
     */
    S_BARRIER,
    /**
     * Syntax: S_BRANCH RELADDR
     * Description: Jump to address RELADDR (store RELADDR to PC).
     * Operation:
     * PC = RELADDR
     */
    S_BRANCH,
    /**
     * Syntax: S_CBRANCH_CDBGSYS RELADDR
     * Description: Jump to address RELADDR if COND_DBG_SYS status bit is set.
     */
    S_CBRANCH_CDBGSYS,
    /**
     *  Syntax: S_CBRANCH_CDBGSYS_AND_USER RELADDR
     *  Description: Jump to address RELADDR if COND_DBG_SYS and COND_DBG_USER
     * status bit is set.
     */
    S_CBRANCH_CDBGSYS_AND_USER,
    /**
     * Syntax: S_CBRANCH_CDBGSYS_OR_USER RELADDR
     * Description: Jump to address RELADDR if COND_DBG_SYS or COND_DBG_USER
     * status bit is set.
     */
    S_CBRANCH_CDBGSYS_OR_USER,
    /**
     * Syntax: S_CBRANCH_CDBGUSER RELADDR
     * Description: Jump to address RELADDR if COND_DBG_USER status bit is set.
     */
    S_CBRANCH_CDBGUSER,
    /**
     *  Syntax: S_CBRANCH_EXECNZ RELADDR
     *  Description: If EXEC is not zero then jump to RELADDR, otherwise jump to
     *  next instruction.
     *  Operation:
     *  PC = EXEC!=0 ? RELADDR : PC+4
     */
    S_CBRANCH_EXECNZ,
    /**
     * Syntax: S_CBRANCH_EXECZ RELADDR
     * Description: If EXEC is zero then jump to RELADDR, otherwise jump to
     * next instruction.
     * Operation:
     * PC = EXEC == 0 ? RELADDR : PC+4
     */
    S_CBRANCH_EXECZ,
    /**
     * Syntax: S_CBRANCH_SCC0 RELADDR
     * Description: If SCC is zero then jump to RELADDR, otherwise jump to next
     * instruction.
     * Operation:
     * PC = SCC0==0 ? RELADDR : PC+4
     */
    S_CBRANCH_SCC0,
    /**
     * Syntax: S_CBRANCH_SCC1 RELADDR
     * Description: If SCC is one then jump to RELADDR, otherwise jump to next
     * instruction.
     * Operation:
     * PC = SCC0==1 ? RELADDR : PC+4
     */
    S_CBRANCH_SCC1,
    /**
     * Syntax: S_CBRANCH_VCCNZ RELADDR
     * Description: If VCC is not zero then jump to RELADDR, otherwise jump to
     * next instruction.
     * Operation:
     * PC = VCC!=0 ? RELADDR : PC+4
     */
    S_CBRANCH_VCCNZ,
    /**
     * Syntax: S_CBRANCH_VCCZ RELADDR
     * Description: If VCC is zero then jump to RELADDR, otherwise jump to next
     * instruction.
     * Operation:
     * PC = VCC==0 ? RELADDR : PC+4
     */
    S_CBRANCH_VCCZ,
    /**
     * Syntax: S_DECPERFLEVEL SIMM16
     * Description: Decrement performance counter specified in SIMM16&15 by 1.
     * Operation:
     * PERFCNT[SIMM16 & 15]--
     */
    S_DECPERFLEVEL,
    /**
     * Syntax: S_ENDPGM
     * Description: End program.
     */
    S_ENDPGM,
    /**
     *     Description: End of program; signal that a wave has exited its POPS
     * critical section and terminate wavefront. The hardware implicitly
     * executes S_WAITCNT 0 before executing this instruction. This instruction
     * is an optimization that combines S_SENDMSG(MSG_ORDERED_PS_DONE) and
     * S_ENDPGM. (from ISA manual)
     */
    S_ENDPGM_ORDERED_PS_DONE,
    /**
     * Syntax: S_ENDPGM_SAVED
     * Description: End of program; signal that a wave has been saved by the
     * context-switch trap handler, and terminate wavefront. The hardware
     * implicitly executes S_WAITCNT 0 before executing this instruction. Use
     * S_ENDPGM in all cases unless you are executing the context-switch save
     * handler. (from ISA manual)
     */
    S_ENDPGM_SAVED,
    /**
     * Syntax: S_ICACHE_INV
     * Description: Invalidate entire L1 instruction cache.
     */
    S_ICACHE_INV,
    /**
     * Syntax: S_INCPERFLEVEL SIMM16
     * Description: Increment performance counter specified in SIMM16&15 by 1.
     * Operation:
     * PERFCNT[SIMM16 & 15]++
     */
    S_INCPERFLEVEL,
    /**
     * Syntax: S_NOP SIMM16
     * Description: Do nothing by (SIMM16&7) + 1 cycles.
     * Operation: nothing
     */
    S_NOP,
    /**
     * Syntax: S_SENDMSG SENDMSG(MSG, GS_OP, STREAMID)
     * Description: Send message. List of messages:
     *      INTERRUPT, MSG_INTERRUPT - interrupt. m0Reg_&0xff - carries user data,
     * IDs also sent (wave_id, cu_id, ...) GS, MSG_GS GS_DONE, MSG_GS_DONE
     *      SYSMSG, MSG_SYSMSG, SYSTEM, MSG_SYSTEM
     * List of the GSOP's:
     *      NOP, GS_NOP - m0Reg_&0xff defines wave id. only GS_DONE
     *      CUT, GS_CUT - (SIMM16 & 0x300)>>8 - streamid, EXEC also sent,
     * m0Reg_&0xff - gs waveID EMIT, GS_EMIT - (SIMM16 & 0x300)>>8 - streamid, EXEC
     * also sent, m0Reg_&0xff - gs waveID EMIT_CUT, GS_EMIT_CUT, EMIT-CUT - (SIMM16
     * & 0x300)>>8 - streamid, EXEC also sent, m0Reg_&0xff - gs waveID
     */
    S_SENDMSG,
    /**
     * Syntax: S_SENDMSGHALT SENDMSG(MSG, GS_OP, STREAMID)
     * Description: Send message and halt.
     */
    S_SENDMSGHALT,
    /**
     * Syntax: S_SET_GPR_IDX_MODE SIMM16
     * Description: Set GPR indexing mode (12-15 bits in MO).
     * Operation:
     * m0Reg_ = (m0Reg_ & 0xffff0fff) | ((SIMM16 & 15)<<12)
     */
    S_SET_GPR_IDX_MODE,
    /**
     * Syntax: S_SET_GPR_IDX_OFF
     * Description: Disables GPR indexing.
     * Operation:
     * MODE = (MODE & ~(1U<<27))
     */
    S_SET_GPR_IDX_OFF,
    /**
     * Syntax: S_SETHALT SIMM16
     * Description: Set HALT bit to value SIMM16&1. 1 - halt, 0 - resume. Halt
     * is ignored while PRIV is 1. Operation: HALT = SIMM16&1
     */
    S_SETHALT,
    /**
     * Syntax: S_SETKILL SIMM16
     * Description: Store SIMM16&1 to KILL.
     * Operation:
     * KILL = SIMM16&1
     */
    S_SETKILL,
    /**
     * Syntax: S_SETPRIO SIMM16
     * Description: Set priority to SIMM16&3.
     * Operation:
     * PRIORITY = SIMM16&3
     */
    S_SETPRIO,
    /**
     * Syntax: S_SLEEP SIMM16
     * Description: Sleep approximately by (SIMM16&0x7)*64 cycles.
     */
    S_SLEEP,
    /**
     * Syntax: S_TRAP SIMM16
     * Description: Enter the trap handler.
     */
    S_TRAP,
    /**
     *  Syntax: S_TTRACEDATA
     *  Description: Send m0Reg_ as user data to thread-trace.
     */
    S_TTRACEDATA,
    /**
     * Wait for the counts of outstanding lds, vector-memory and
     * export/vmem-write-data to be at or below the specified levels.
     * SIMM16[3:0] = vmcount (vector memory operations) lower bits [3:0],
     * SIMM16[6:4] = export/mem-write-data count,
     * SIMM16[11:8] = LGKM_cnt (scalar-mem/GDS/LDS count),
     * SIMM16[15:14] = vmcount (vector memory operations) upper bits [5:4]
     * Waits for vector memory operations (VMCNT);
     * LDS, GDS, memory operations (LGKMCNT);
     * export memory-write-data operations (EXPCNT).
     * (SIMM16&15) specifies how many VMCNT operations can be unfinished
     * (GCN 1.0/1.1/1.2), value 0xf - no wait for VMCNT operations.
     * (SIMM16&15)|((SIMM16>>10)&0x30) specifies how many VMCNT operations can
     * be unfinished (GCN 1.4), value 0x3f - no wait for VMCNT operations.
     * (SIMM16>>4)&7 specifies how many EXPCNT operations can be unfinished, 0x7
     * - no wait for EXPCNT operations. (SIMM16>>8)&31 specifies how many
     * LGKMCNT operations can be unfinished, 0x1f - no wait for LGKM operations.
     */
    S_WAITCNT,
    // END SOPP

    // SOP1_FORMAT
    /**
     * Syntax: S_ABS_B32 SDST, SSRC0
     * Operation:
     * SDST = ABS(SSRC0)
     * SCC = SDST!=0
     */
    S_ABS_I32,
    /**
     * Syntax: S_AND_SAVEEXEC_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = EXEC
     * EXEC = SSRC0 & EXEC
     * SCC = EXEC!=0
     */
    S_AND_SAVEEXEC_B64,
    /**
     * Syntax: S_ANDN2_SAVEEXEC_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = EXEC
     * EXEC = ~SSRC0 & EXEC
     * SCC = EXEC!=0
     */
    S_ANDN1_SAVEEXEC_B64,
    /**
     * Syntax: S_ANDN1_WREXEC_B64 SDST(2), SSRC0(2)
     * Operation:
     * EXEC = ~SSRC0 & EXEC
     * SDST = EXEC
     * SCC = EXEC!=0
     */
    S_ANDN1_WREXEC_B64,
    /**
     * Syntax: S_ANDN2_SAVEEXEC_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = EXEC
     * EXEC = SSRC0 & ~EXEC
     * SCC = EXEC!=0
     */
    S_ANDN2_SAVEEXEC_B64,
    /**
     * Syntax: S_ANDN2_WREXEC_B64 SDST(2), SSRC0(2)
     * Operation:
     * EXEC = SSRC0 & ~EXEC
     * SDST = EXEC
     * SCC = EXEC!=0
     */
    S_ANDN2_WREXEC_B64,
    /**
     * Syntax: S_BCNT0_I32_B32 SDST, SSRC0
     * Operation:
     * SDST = BITCOUNT(~SSRC0)
     * SCC = SDST!=0
     */
    S_BCNT0_I32_B32,
    /**
     * Syntax: S_BCNT0_I32_B64 SDST, SSRC0(2)
     * Operation:
     * SDST = BITCOUNT(~SSRC0)
     * SCC = SDST!=0
     */
    S_BCNT0_I32_B64,
    /**
     * Syntax: S_BCNT1_I32_B64 SDST, SSRC0
     * Operation:
     * SDST = BITCOUNT(SSRC0)
     * SCC = SDST!=0
     */
    S_BCNT1_I32_B32,
    /**
     * Syntax: S_BCNT1_I32_B64 SDST, SSRC0(2)
     * Operation:
     * SDST = BITCOUNT(SSRC0)
     * SCC = SDST!=0
     */
    S_BCNT1_I32_B64,
    /**
     * Syntax: S_BITREPLICATE_B64_B32 SDST(2), SSRC0
     * Operation:
     * SDST = 0
     * for (BYTE I=0; I<32; I++)
     *      SDST |= (((SSRC0>>I)&1)*3)<<(I<<1)
     */
    S_BITREPLICATE_B64_B32,
    /**
     * Syntax: S_BITSET0_B32 SDST, SSRC0
     * Operation:
     * SDST &= ~(1U << (SSRC0&31))
     */
    S_BITSET0_B32,
    /**
     * Syntax: S_BITSET0_B64 SDST(2), SSRC0
     * Operation:
     * SDST &= ~(1ULL << (SSRC0&63))
     */
    S_BITSET0_B64,
    /**
     * Syntax: S_BITSET1_B32 SDST, SSRC0
     * Operation:
     * SDST |= 1U << (SSRC0&31)
     */
    S_BITSET1_B32,
    /**
     * Syntax: S_BITSET1_B64 SDST(2), SSRC0
     * Operation:
     * SDST |= 1ULL << (SSRC0&63)
     */
    S_BITSET1_B64,
    /**
     * Syntax: S_BREV_B32 SDST, SSRC0
     * Operation:
     * SDST = REVBIT(SSRC0)
     */
    S_BREV_B32,
    /**
     * Syntax: S_BREV_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = REVBIT(SSRC0)
     */
    S_BREV_B64,
    /**
     * Syntax: S_CBRANCH_JOIN SSRC0
     * Operation:
     * if (CSP==SSRC0)
     *   PC += 4
     * else {
     *   CSP--
     *   EXEC = SGPR[CSP*4:CSP*4+1]
     *   PC = SGPRS[CSP*4+2:CSP*4+3]
     * }
     */
    S_CBRANCH_JOIN,
    /**
     * Syntax: S_CMOV_B32 SDST, SSRC0
     * Operation:
     * SDST = SCC ? SSRC0 : SDST
     */
    S_CMOV_B32,
    /**
     * Syntax: S_CMOV_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = SCC ? SSRC0 : SDST
     */
    S_CMOV_B64,
    /**
     * Syntax: S_FF0_I32_B32 SDST, SSRC0
     * Operation:
     * SDST = -1
     * for (UINT8 i = 0; i < 32; i++)
     *      if ((1U<<i) & SSRC0) == 0)
     *       { SDST = i; break; }
     */
    S_FF0_I32_B32,
    /**
     * Syntax: S_FF0_I32_B64 SDST, SSRC0(2)
     * Operation:
     * SDST = -1
     * for (UINT8 i = 0; i < 64; i++)
     * if ((1ULL<<i) & SSRC0) == 0)
     * { SDST = i; break; }
     */
    S_FF0_I32_B64,
    /**
     * Syntax: S_FF1_I32_B32 SDST, SSRC0
     * Operation:
     * SDST = -1
     * for (UINT8 i = 0; i < 32; i++)
     * if ((1U<<i) & SSRC0) != 0)
     * { SDST = i; break; }
     */
    S_FF1_I32_B32,
    /**
     * Syntax: S_FF0_I32_B64 SDST, SSRC0(2)
     * Operation:
     * SDST = -1
     * for (UINT8 i = 0; i < 64; i++)
     * if ((1ULL<<i) & SSRC0) != 0)
     * { SDST = i; break; }
     */
    S_FF1_I32_B64,
    /**
     * Syntax: S_FLBIT_I32_B32 SDST, SSRC0
     * Operation:
     * SDST = -1
     * for (INT8 i = 31; i >= 0; i--)
     * if ((1U<<i) & SSRC0) != 0)
     * { SDST = 31-i; break; }
     */
    S_FLBIT_I32_B32,
    /**
     * Syntax: S_FLBIT_I32_B64 SDST, SSRC0(2)
     * Operation:
     * SDST = -1
     * for (INT8 i = 63; i >= 0; i--)
     * if ((1ULL<<i) & SSRC0) != 0)
     * { SDST = 63-i; break; }
     */
    S_FLBIT_I32_B64,
    /**
     * Syntax: S_FLBIT_I32 SDST, SSRC0
     * Operation:
     * SDST = -1
     * UINT32 bitval = (INT32)SSRC0>=0 ? 1 : 0
     * for (INT8 i = 31; i >= 0; i--)
     * if ((1U<<i) & SSRC0) == (bitval<<i))
     *   { SDST = 31-i; break; }
     */
    S_FLBIT_I32,
    /**
     * Syntax: S_FLBIT_I32_I64 SDST, SSRC0(2)
     * Operation:
     * SDST = -1
     * UINT64 bitval = (INT64)SSRC0>=0 ? 1 : 0
     * for (INT8 i = 63; i >= 0; i--)
     * if ((1U<<i) & SSRC0) == (bitval<<i))
     * { SDST = 63-i; break; }
     */
    S_FLBIT_I32_I64,
    /**
     * Syntax: S_GETPC_B64 SDST(2)
     * Operation:
     * SDST = PC + 4
     */
    S_GETPC_B64,
    /**
     * Syntax: S_MOV_B32 SDST, SSRC0
     * Operation:
     * SDST = SSRC0
     */
    S_MOV_B32,
    /**
     * Syntax: S_MOV_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = SSRC0
     */
    S_MOV_B64,
    /**
     * Syntax: S_MOVRELD_B32 SDST, SSRC0
     * Operation:
     * SGPR[SDST_NUMBER + m0Reg_] = SSRC0
     */
    S_MOVRELD_B32,
    /**
     * Syntax: S_MOVRELD_B64 SDST, SSRC0
     * Operation:
     * SGPR[SDST_NUMBER + m0Reg_ : SDST_NUMBER + m0Reg_ + 1] = SSRC0
     */
    S_MOVRELD_B64,
    /**
     * Syntax: S_MOVRELS_B32 SDST, SSRC0
     * Operation:
     * SDST = SGPR[SSRC0_NUMBER + m0Reg_]
     */
    S_MOVRELS_B32,
    /**
     * Syntax: S_MOVRELS_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = SGPR[SSRC0_NUMBER + m0Reg_ : SSRC0_NUMBER + m0Reg_ + 1]
     */
    S_MOVRELS_B64,
    /**
     * Syntax: S_NAND_SAVEEXEC_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = EXEC
     * EXEC = ~(SSRC0 & EXEC)
     * SCC = EXEC!=0
     */
    S_NAND_SAVEEXEC_B64,
    /**
     * Syntax: S_NOR_SAVEEXEC_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = EXEC
     * EXEC = ~(SSRC0 | EXEC)
     * SCC = EXEC!=0
     */
    S_NOR_SAVEEXEC_B64,
    /**
     * Syntax: S_NOT_B32 SDST, SSRC0
     * Operation:
     * SDST = ~SSRC0
     * SCC = SDST!=0
     */
    S_NOT_B32,
    /**
     * Syntax: S_NOT_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = ~SSRC0
     * SCC = SDST!=0
     */
    S_NOT_B64,
    /**
     * Syntax: S_OR_SAVEEXEC_B64 SDST(2), SDST(2)
     * Operation:
     * SDST = EXEC
     * EXEC = SSRC0 | EXEC
     * SCC = EXEC!=0
     */
    S_OR_SAVEEXEC_B64,
    /**
     * Syntax: S_ORN2_SAVEEXEC_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = EXEC
     * EXEC = ~SSRC0 & EXEC
     * SCC = EXEC!=0
     */
    S_ORN2_SAVEEXEC_B64,
    /**
     * Syntax: S_QUADMASK_B32 SDST, SSRC0
     * Operation:
     * UINT32 temp = 0
     * for (UINT8 i = 0; i < 8; i++)
     *      temp |= ((SSRC0>>(i<<2)) & 15)!=0 ? (1U<<i) : 0
     * SDST = temp
     * SCC = SDST!=0
     */
    S_QUADMASK_B32,
    /**
     * Syntax: S_QUADMASK_B64 SDST(2), SSRC0(2)
     * Operation:
     * UINT64 temp = 0
     * for (UINT8 i = 0; i < 16; i++)
     * temp |= ((SSRC0>>(i<<2)) & 15)!=0 ? (1U<<i) : 0
     * SDST = temp
     * SCC = SDST!=0
     */
    S_QUADMASK_B64,
    /**
     * Syntax: S_RFE_B64 SSRC0(2)
     * Operation: ???
     * PC = TTMP[0:1]
     */
    S_RFE_B64,
    /**
     * Syntax S_SET_GPR_IDX_IDX SSRC0(1)
     * Operation:
     * m0Reg_ = (m0Reg_ & 0xffffff00) | (SSRC0 & 0xff)
     */
    S_SET_GPR_IDX_IDX,
    /**
     * Syntax: S_SETPC_B64 SSRC0(2)
     * Operation:
     * PC = SSRC0
     */
    S_SETPC_B64,
    /**
     * Syntax: S_SEXT_I32_I8 SDST, SSRC0
     * Operation:
     * SDST = SEXT((INT8)SSRC0)
     */
    S_SEXT_I32_I8,
    /**
     * Syntax: S_SEXT_I32_I16 SDST, SSRC0
     * Operation:
     * SDST = SEXT((INT16)SSRC0)
     */
    S_SEXT_I32_I16,
    /**
     * Syntax: S_SWAPPC_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = PC + 4
     * PC = SSRC0
     */
    S_SWAPPC_B64,
    /**
     * Syntax: S_WQM_B32 SDST, SSRC0
     * Operation:
     * UINT32 temp = 0
     * for (UINT8 i = 0; i < 32; i+=4)
     *      temp |= ((SSRC0>>i) & 15)!=0 ? (15<<i) : 0
     * SDST = temp
     * SCC = SDST!=0
     */
    S_WQM_B32,
    /**
     * Syntax: S_WQM_B64 SDST(2), SSRC0(2)
     * Operation:
     * UINT64 temp = 0
     * for (UINT8 i = 0; i < 64; i+=4)
     * temp |= ((SSRC0>>i) & 15)!=0 ? (15ULL<<i) : 0
     * SDST = temp
     * SCC = SDST!=0
     */
    S_WQM_B64,
    /**
     * Syntax: S_XNOR_SAVEEXEC_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = EXEC
     * EXEC = ~(SSRC0 ^ EXEC)
     * SCC = EXEC!=0
     */
    S_XNOR_SAVEEXEC_B64,
    /**
     * Syntax: S_XOR_SAVEEXEC_B64 SDST(2), SSRC0(2)
     * Operation:
     * SDST = EXEC
     * EXEC = SSRC0 ^ EXEC
     * SCC = EXEC!=0
     */
    S_XOR_SAVEEXEC_B64,
    // END SOP1_FORMAT

    // SOP2_FORMAT
    /**
     * Syntax: S_ABSDIFF_I32 SDST, SSRC0, SSRC1
     * Operation:
     * SDST = ABS(SSRC0 - SSRC1)
     * SCC = SDST!=0
     */
    S_ABSDIFF_I32,
    /**
     * UINT64 temp = (UINT64)SSRC0 + (UINT64)SSRC1 + SCC
     * SDST = temp
     * SCC = temp >> 32
     */
    S_ADDC_U32,
    /**
     * Syntax: S_ADD_I32 SDST, SSRC0, SSRC1
     * Operation:
     *   SDST = SSRC0 + SSRC1
     *   INT64 temp = SEXT64(SSRC0) + SEXT64(SSRC1)
     *   SCC = temp > ((1LL<<31)-1) || temp < (-1LL<<31)
     */
    S_ADD_I32,
    /**
     * Syntax: S_ADD_U32 SDST, SSRC0, SSRC1
     * Description: Add SSRC0 to SSRC1 and store result into SDST and store
     * carry-out into SCC. Operation: UINT64 temp = (UINT64)SSRC0 +
     * (UINT64)SSRC1 SDST = temp SCC = temp >> 32
     */
    S_ADD_U32,
    /**
     * Syntax: S_AND_B32 SDST, SSRC0, SSRC1
     * Operation:
     * SDST = SSRC0 & SSRC1
     * SCC = SDST!=0
     */
    S_AND_B32,
    /**
     * SDST = SSRC0 & SSRC1
     * SCC = SDST!=0
     */
    S_AND_B64,
    /**
     * SDST = SSRC0 & ~SSRC1
     * SCC = SDST!=0
     */
    S_ANDN2_B32,
    /**
     *  SDST = SSRC0 & ~SSRC1
     *  SCC = SDST!=0
     */
    S_ANDN2_B64,
    /**
     * SDST = (INT32)SSRC0 >> (SSRC1 & 31)
     * SCC = SDST!=0
     */
    S_ASHR_I32,
    /**
     * SDST = (INT64)SSRC0 >> (SSRC1 & 63)
     * SCC = SDST!=0
     */
    S_ASHR_I64,
    /**
     * Syntax: S_BFE_I32 SDST, SSRC0, SSRC1
     * Description: Extracts bits in SSRC0 from range (SSRC1&31) with length
     * ((SSRC1>>16)&0x7f) and extend sign from last bit of extracted value, and
     * store result to SDST. If result is non-zero store 1 to SCC, otherwise
     * store 0 to SCC. Operation: UINT8 shift = SSRC1 & 31 UINT8 length =
     * (SSRC1>>16) & 0x7f if (length==0) SDST = 0 if (shift+length < 32) SDST =
     * (INT32)(SSRC0 << (32 - shift - length)) >> (32 - length) else SDST =
     * (INT32)SSRC0 >> shift SCC = SDST!=0
     */
    S_BFE_I32,
    /**
     * UINT8 shift = SSRC1 & 63
     * UINT8 length = (SSRC1>>16) & 0x7f
     * if (length==0)
     *  SDST = 0
     * if (shift+length < 64)
     *  SDST = (INT64)(SSRC0 << (64 - shift - length)) >> (64 - length)
     * else
     *  SDST = (INT64)SSRC0 >> shift
     *  SCC = SDST!=0
     */
    S_BFE_I64,
    /**
     * UINT8 shift = SSRC1 & 31
     * UINT8 length = (SSRC1>>16) & 0x7f
     * if (length==0)
     *   SDST = 0
     * if (shift+length < 32)
     *   SDST = SSRC0 << (32 - shift - length) >> (32 - length)
     * else
     *   SDST = SSRC0 >> shift
     * SCC = SDST!=0
     */
    S_BFE_U32,
    /**
     * UINT8 shift = SSRC1 & 63
     * UINT8 length = (SSRC1>>16) & 0x7f
     * if (length==0)
     *   SDST = 0
     * if (shift+length < 64)
     *   SDST = SSRC0 << (64 - shift - length) >> (64 - length)
     * else
     *   SDST = SSRC0 >> shift
     * SCC = SDST!=0
     */
    S_BFE_U64,
    /**
     * SDST = ((1U << (SSRC0&31))-1) << (SSRC1&31)
     */
    S_BFM_B32,
    /**
     * SDST = ((1ULL << (SSRC0&63))-1) << (SSRC1&63)
     */
    S_BFM_B64,
    /**
     * UINT64 passes = (EXEC & SSRC0)
     * UINT64 failures = (EXEC & ~SSRC0)
     * if (passes == EXEC)
     *   PC = SSRC1
     * else if (failures == EXEC)
     *   PC += 4
     * else if (BITCOUNT(failures) < BITCOUNT(passes)) {
     *   EXEC = failures
     *   SGPR[CSP*4:CSP*4+1] = passes
     *   SGPR[CSP*4+2:CSP*4+3] = SSRC1
     *   CSP++
     *   PC += 4 //jump to failure
     * } else {
     *   EXEC = passes
     *   SGPR[CSP*4:CSP*4+1] = failures
     *   SGPR[CSP*4+2:CSP*4+3] = PC+4
     *   CSP++
     *   PC = SSRC1  //jump to passes
     * }
     */
    S_CBRANCH_G_FORK,
    /**
     * SDST = SCC ? SSRC0 : SSRC1
     */
    S_CSELECT_B32,
    /**
     * SDST = SCC ? SSRC0 : SSRC1
     */
    S_CSELECT_B64,
    /**
     * SDST = SSRC0 << (SSRC1 & 31)
     * SCC = SDST != 0
     */
    S_LSHL_B32,
    /**
     * SDST = SSRC0 << (SSRC1 & 63)
     * SCC = SDST != 0
     */
    S_LSHL_B64,
    /**
     * UINT64 TMP = (SSRC0 << 1) + SSRC1
     * SDST = TMP & 0xffffffff
     * SCC = TMP >= (1ULL << 32)
     */
    S_LSHL1_ADD_U32,
    /**
     * UINT64 TMP = (SSRC0 << 2) + SSRC1
     * SDST = TMP & 0xffffffff
     * SCC = TMP >= (1ULL << 32)
     */
    S_LSHL2_ADD_U32,
    /**
     * UINT64 TMP = (SSRC0 << 3) + SSRC1
     * SDST = TMP & 0xffffffff
     * SCC = TMP >= (1ULL << 32)
     */
    S_LSHL3_ADD_U32,
    /**
     * UINT64 TMP = (SSRC0 << 4) + SSRC1
     * SDST = TMP & 0xffffffff
     * SCC = TMP >= (1ULL << 32)
     */
    S_LSHL4_ADD_U32,
    /**
     * SDST = SSRC0 >> (SSRC1 & 31)
     * SCC = SDST != 0
     */
    S_LSHR_B32,
    /**
     * SDST = SSRC0 >> (SSRC1 & 63)
     * SCC = SDST != 0
     */
    S_LSHR_B64,
    /**
     * SDST = MAX((INT32) SSRC0, (INT32) SSRC1)
     * SCC = (INT32) SSRC0 > (INT32) SSRC1
     */
    S_MAX_I32,
    /**
     * SDST = MAX(SSRC0, SSRC1)
     * SCC = SSRC0 > SSRC1
     */
    S_MAX_U32,
    /**
     * SDST = MIN((INT32) SSRC0, (INT32) SSRC1)
     * SCC = (INT32) SSRC0 < (INT32) SSRC1
     */
    S_MIN_I32,
    /**
     * SDST = MIN(SSRC0, SSRC1)
     * SCC = SSRC0 < SSRC1
     */
    S_MIN_U32,
    /**
     * SDST = ((INT64) SSRC0 * (INT32) SSRC1) >> 32
     */
    S_MUL_HI_I32,
    /**
     * SDST = ((UINT64) SSRC0 * SSRC1) >> 32
     */
    S_MUL_HI_U32,
    /**
     * SDST = SSRC0 * SSRC1
     */
    S_MUL_I32,
    /**
     * SDST = ~(SSRC0 & SSRC1)
     * SCC = SDST != 0
     */
    S_NAND_B32,
    /**
     * SDST = ~(SSRC0 & SSRC1)
     * SCC = SDST != 0
     */
    S_NAND_B64,
    /**
     * SDST = ~(SSRC0 | SSRC1)
     * SCC = SDST != 0
     */
    S_NOR_B32,
    /**
     * SDST = ~(SSRC0 | SSRC1)
     * SCC = SDST != 0
     */
    S_NOR_B64,
    /**
     * SDST = SSRC0 | SSRC1
     * SCC = SDST != 0
     */
    S_OR_B32,
    /**
     * SDST = SSRC0 | SSRC1
     * SCC = SDST != 0
     */
    S_OR_B64,
    /**
     * SDST = SSRC0 | ~SSRC1
     * SCC = SDST != 0
     */
    S_ORN2_B32,
    /**
     * SDST = SSRC0 | ~SSRC1
     * SCC = SDST != 0
     */
    S_ORN2_B64,
    /**
     * SDST = (SSRC0 >> 16) | (SSRC1 & 0xffff0000)
     */
    S_PACK_HH_B32_B16,
    /**
     * SDST = (SSRC0 & 0xffff) | (SSRC1 & 0xffff0000)
     */
    S_PACK_LH_B32_B16,
    /**
     * Operation:
     * SDST = (SSRC0 & 0xffff) | ((SSRC1 & 0xffff) << 16)
     */
    S_PACK_LL_B32_B16,
    /**
     * Syntax: S_RFE_RESTORE_B64 SDST(2), SSRC0(1)
     * Description: Return from exception handler and set: INST_ATC =
     * SSRC1.U32[0] ???
     */
    S_RFE_RESTORE_B64,
    /**
     * Syntax: S_SUBB_U32 SDST, SSRC0, SSRC1
     * Operation:
     * UINT64 temp = (UINT64)SSRC0 - (UINT64)SSRC1 - SCC
     * SDST = temp
     * SCC = (temp>>32) & 1
     */
    S_SUBB_U32,
    /**
     * Syntax: S_SUB_I32 SDST, SSRC0, SSRC1
     * Operation:
     * SDST = SSRC0 - SSRC1
     * INT64 temp = SEXT64(SSRC0) - SEXT64(SSRC1)
     * SCC = temp > ((1LL << 31) - 1) || temp < (-1LL << 31)
     */
    S_SUB_I32,
    /**
     * Syntax: S_SUB_U32 SDST, SSRC0, SSRC1
     * Operation:
     * UINT64 temp = (UINT64)SSRC0 - (UINT64)SSRC1
     * SDST = temp
     * SCC = (temp>>32)!=0
     */
    S_SUB_U32,
    /**
     * Syntax: S_XNOR_B32 SDST, SSRC0, SSRC1
     * Operation:
     * SDST = ~(SSRC0 ^ SSRC1)
     * SCC = SDST != 0
     */
    S_XNOR_B32,
    /**
     * Syntax: S_XNOR_B64 SDST(2), SSRC0(2), SSRC1(2)
     * Operation:
     * SDST = ~(SSRC0 ^ SSRC1)
     * SCC = SDST != 0
     */
    S_XNOR_B64,
    /**
     * Syntax: S_XOR_B32 SDST, SSRC0, SSRC1
     * Operation:
     * SDST = SSRC0 ^SSRC1
     * SCC = SDST != 0
     */
    S_XOR_B32,
    /**
     * Syntax: S_XOR_B64 SDST(2), SSRC0(2), SSRC1(2)
     * Operation:
     * SDST = SSRC0 ^SSRC1
     * SCC = SDST != 0
     */
    S_XOR_B64,
    // END SOP2_FORMAT

    // SOPK_FORMAT
    /**
     * Syntax: S_ADDK_I32 SDST, SIMM16
     * Operation:
     * SDST = SDST + SIMM16
     * INT64 temp = SEXT64(SDST) + SEXT64(SIMM16)
     * SCC = temp > ((1LL<<31)-1) || temp < (-1LL<<31)
     */
    S_ADDK_I32,
    /**
     * Syntax: S_CALL_B64 SDST(2), RELADDR
     * Operation:
     * SDST = PC + 4
     * PC = RELADDR
     */
    S_CALL_B64,
    /**
     * Syntax: S_CBRANCH_I_FORK SSRC0(2), RELADDR
     * Operation:
     * UINT64 passes = (EXEC & SSRC0)
     * UINT64 failures = (EXEC & ~SSRC0)
     * if (passes == EXEC)
     *  PC = RELADDR
     * else if (failures == EXEC)
     *  PC += 4
     * else if (BITCOUNT(failures) < BITCOUNT(passes)) {
     *  EXEC = failures
     *  SGPR[CSP*4:CSP*4+1] = passes
     *  SGPR[CSP*4+2:CSP*4+3] = RELADDR
     *  CSP++
     *  PC += 4 //jump to failure
     * } else {
     *  EXEC = passes
     *  SGPR[CSP*4:CSP*4+1] = failures
     *  SGPR[CSP*4+2:CSP*4+3] = PC+4
     *  CSP++
     *  PC = RELADDR //jump to passes
     * }
     */
    S_CBRANCH_I_FORK,
    /**
     * Syntax: S_MOVK_I32 SDST, SIMM16
     * Operation:
     * SDST = SCC ? SIMM16 : SDST
     */
    S_CMOVK_I32,
    /**
     * Syntax: S_CMPK_EQ_I32 SDST, SIMM16
     * Operation:
     * SCC = (INT32)SDST == SIMM16
     */
    S_CMPK_EQ_I32,
    /**
     * Syntax: S_CMPK_EQ_U32 SDST, IMM16
     * Operation:
     * SCC = SDST == IMM16
     */
    S_CMPK_EQ_U32,
    /**
     * Syntax: S_CMPK_GE_I32 SDST, SIMM16
     * Operation:
     * SCC = (INT32)SDST >= SIMM16
     */
    S_CMPK_GE_I32,
    /**
     * Syntax: S_CMPK_GE_U32 SDST, IMM16
     * Operation:
     * SCC = SDST >= IMM16
     */
    S_CMPK_GE_U32,
    /**
     * Syntax: S_CMPK_GT_I32 SDST, SIMM16
     * Operation:
     * SCC = (INT32)SDST > SIMM16
     */
    S_CMPK_GT_I32,
    /**
     * Syntax: S_CMPK_GT_U32 SDST, IMM16
     * Operation:
     * SCC = SDST > IMM16
     */
    S_CMPK_GT_U32,
    /**
     * Syntax: S_CMPK_LE_I32 SDST, SIMM16
     * Operation:
     * SCC = (INT32)SDST <= SIMM16
     */
    S_CMPK_LE_I32,
    /**
     * Syntax: S_CMPK_LE_U32 SDST, IMM16
     * Operation:
     * SCC = SDST <= IMM16
     */
    S_CMPK_LE_U32,
    /**
     * Syntax: S_CMPK_LG_I32 SDST, SIMM16
     * Operation:
     * SCC = (INT32)SDST != SIMM16
     */
    S_CMPK_LG_I32,
    /**
     * Syntax: S_CMPK_LG_U32 SDST, IMM16
     * Operation:
     * SCC = SDST != IMM16
     */
    S_CMPK_LG_U32,
    /**
     * Syntax: S_CMPK_LT_I32 SDST, SIMM16
     * Operation:
     * SCC = (INT32)SDST < SIMM16
     */
    S_CMPK_LT_I32,
    /**
     * Syntax: S_CMPK_LT_U32 SDST, IMM16
     * Operation:
     * SCC = SDST < IMM16
     */
    S_CMPK_LT_U32,
    /**
     * Syntax: S_GETREG_B32 SDST, HWREG(HWREGNAME, BITOFFSET, BITSIZE)
     * Operation:
     * SDST = (HWREG >> BITOFFSET) & ((1U << BITSIZE) - 1U)
     */
    S_GETREG_B32,
    /**
     * Syntax: S_MOVK_I32 SDST, SIMM16
     * Operation:
     * SCC = SIMM16
     */
    S_MOVK_I32,
    /**
     * Syntax: S_MULK_I32 SDST, SIMM16
     * Operation:
     * SDST = SDST * SIMM16
     */
    S_MULK_I32,
    /**
     * Syntax: S_SETREG_B32 HWREG(HWREGNAME, BITOFFSET, BITSIZE), SDST
     * Operation:
     * UINT32 mask = ((1U<<BITSIZE) - 1U) << BITOFFSET
     * HWREG = (HWREG & ~mask) | ((SDST<<BITOFFSET) & mask)
     */
    S_SETREG_B32,
    /**
     * Syntax: S_SETREG_B32 HWREG(HWREGNAME, BITOFFSET, BITSIZE), IMM32
     * Operation:
     * UINT32 mask = ((1U<<BITSIZE) - 1U) << BITOFFSET
     * HWREG = (HWREG & ~mask) | ((IMM32<<BITOFFSET) & mask)
     */
    S_SETREG_IMM32_B32,
    // END SOPK_FORMAT

    // VOP1
    /**
     * D.u = S0.u.
     * Input and output modifiers not supported; this is an untyped operation.
     */
    V_MOV_B32,

    // VOP2

    /**
     * D.u = S0.u + S1.u.
     */
    V_ADD_U32,

    V_SUB_U32,

    /**
     * todo not in ofdoc
     */
    V_ADDC_U32,

    //VOP3
    V_MUL_LO_U32,

    // VOP3A
    /**
     * D.u64 = S1.u64 << S0.u[5:0].
     */
    V_LSHLREV_B64,

    // VOPC
    /**
     * D.u64[threadId] = (S0 == S1).
     */
    V_CMP_EQ_I32,

    // FLAT
    /**
     * Untyped buffer store dword
     */
    FLAT_STORE_DWORD,
    FLAT_STORE_DWORDX2,
    FLAT_STORE_DWORDX3,
    FLAT_STORE_DWORDX4,
    FLAT_STORE_SHORT
};

enum InstrFormat {
    SOP1,
    SOP2,
    SOPK, SOPP, SMEM, SOPC, VOP1, VOP2, VOPC, VINTRP, VOP3A, VOP3B, VOP3P, FLAT
};

//todo use this function to parse asm
/**
 * @return InstructionKey for the given mnemonic
 */
InstrKey get_instr_key(std::string_view);

/**
 * @return mnemonic for InstrKey
 */
char const* get_instr_str(InstrKey) noexcept;

/**
 * @return InstrFormat of the instruction which given InstrKey represents
 */
InstrFormat get_instr_format(InstrKey);

/**
 * @return instruction's width in bits
 */
uint8_t get_instr_width(InstrKey);


#endif  // RED_O_LATOR_INSTR_INFO_H
