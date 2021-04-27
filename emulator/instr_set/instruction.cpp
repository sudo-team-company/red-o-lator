/**
 * @author dkudaiberdieva
 */

#include <unordered_map>

using namespace std;

enum InstructionKey {

//SMEM
/**
 * Read 1 dword from scalar data cache. If the offset is specified as an SGPR, the SGPR contains an UNSIGNED BYTE
 * OFFSET (the 2 LSBs are ignored). If the offset is specified as an immediate 21-bit constant, the constant is
 * a SIGNED BYTE offset.
 */
    S_LOAD_DWORD,
/**
 * Read 2 dwords from scalar data cache. See S_LOAD_DWORD for details on the offset input
 */
    S_LOAD_DWORDX2,

//SOPC
/**
 * SCC = (S0.i < S1.i)
 */
    S_CMP_LT_I32,

    //SOPP
    /**
     * Wait for the counts of outstanding lds, vector-memory and export/vmem-write-data to be at or below the specified levels.
     * SIMM16[3:0] = vmcount (vector memory operations) lower bits [3:0], SIMM16[6:4] = export/mem-write-data count,
     * SIMM16[11:8] = LGKM_cnt (scalar-mem/GDS/LDS count), SIMM16[15:14] = vmcount (vector memory operations) upper bits [5:4],
     */
    S_WAITCNT,

    /**
     * if(SCC == 0) then
     *      PC = PC + signext(SIMM16 * 4) + 4;
     * endif.
     */
    S_CBRANCH_SCC0,

    //SOP1
    /**
     *     Syntax: S_ABS_B32 SDST, SSRC0
     *     Operation:
     *     SDST = ABS(SSRC0)
     *     SCC = SDST!=0
     */
    S_ABS_I32,
    /**
     *     Syntax: S_AND_SAVEEXEC_B64 SDST(2), SSRC0(2)
    Operation:
    SDST = EXEC
    EXEC = SSRC0 & EXEC
    SCC = EXEC!=0

     */
    S_AND_SAVEEXEC_B64,
    /**
     * Syntax: S_ANDN2_SAVEEXEC_B64 SDST(2), SSRC0(2)
    Operation:
    SDST = EXEC
    EXEC = ~SSRC0 & EXEC
    SCC = EXEC!=0
     */
    S_ANDN1_SAVEEXEC_B64,
    /**
     *     Syntax: S_ANDN1_WREXEC_B64 SDST(2), SSRC0(2)
    Operation:
    EXEC = ~SSRC0 & EXEC
    SDST = EXEC
    SCC = EXEC!=0

     */
    S_ANDN1_WREXEC_B64,
    /**
     *     Syntax: S_ANDN2_SAVEEXEC_B64 SDST(2), SSRC0(2)
    Operation:
    SDST = EXEC
    EXEC = SSRC0 & ~EXEC
    SCC = EXEC!=0
     */
    S_ANDN2_SAVEEXEC_B64,
    /**
     *     Syntax: S_ANDN2_WREXEC_B64 SDST(2), SSRC0(2)
    Operation:
    EXEC = SSRC0 & ~EXEC
    SDST = EXEC
    SCC = EXEC!=0

     */
    S_ANDN2_WREXEC_B64,
    /**
     *     Syntax: S_BCNT0_I32_B32 SDST, SSRC0
    Operation:
    SDST = BITCOUNT(~SSRC0)
    SCC = SDST!=0

     */
    S_BCNT0_I32_B32,
    /**
     *     Syntax: S_BCNT0_I32_B64 SDST, SSRC0(2)
    Operation:
    SDST = BITCOUNT(~SSRC0)
    SCC = SDST!=0

     */
    S_BCNT0_I32_B64,
    /**
     *    Syntax: S_BCNT1_I32_B64 SDST, SSRC0
    Operation:
    SDST = BITCOUNT(SSRC0)
    SCC = SDST!=0

     */
    S_BCNT1_I32_B32,
    /**
     *     Syntax: S_BCNT1_I32_B64 SDST, SSRC0(2)
    Operation:
    SDST = BITCOUNT(SSRC0)
    SCC = SDST!=0
     */
    S_BCNT1_I32_B64,
    /**
     *    Syntax: S_BITREPLICATE_B64_B32 SDST(2), SSRC0
     *    Operation:
     *    SDST = 0
     *    for (BYTE I=0; I<32; I++)
     *    SDST |= (((SSRC0>>I)&1)*3)<<(I<<1)
     */
    S_BITREPLICATE_B64_B32,
/**
 *     Syntax: S_BITSET0_B32 SDST, SSRC0
 *     Operation:
 *     SDST &= ~(1U << (SSRC0&31))
 */
    S_BITSET0_B32,
    /**
     *    Syntax: S_BITSET0_B64 SDST(2), SSRC0
    Operation:
    SDST &= ~(1ULL << (SSRC0&63))
     */
    S_BITSET0_B64,
    /**
     *     Syntax: S_BITSET1_B32 SDST, SSRC0
    Operation:
    SDST |= 1U << (SSRC0&31)
     */
    S_BITSET1_B32,
    /**
     *    Syntax: S_BITSET1_B64 SDST(2), SSRC0
    Operation:
    SDST |= 1ULL << (SSRC0&63)
     */
    S_BITSET1_B64,
    /**
     * Syntax: S_BREV_B32 SDST, SSRC0
    Operation:
    SDST = REVBIT(SSRC0)
     */
    S_BREV_B32,
    /**
     *     Syntax: S_BREV_B64 SDST(2), SSRC0(2)
    Operation:
    SDST = REVBIT(SSRC0)
     */
    S_BREV_B64,
    /**
     *     Syntax: S_CBRANCH_JOIN SSRC0
     *     Operation:
     *     if (CSP==SSRC0)
     *          PC += 4
     *     else {
     *          CSP--
     *          EXEC = SGPR[CSP*4:CSP*4+1]
     *          PC = SGPRS[CSP*4+2:CSP*4+3]
     *     }
     */

    S_CBRANCH_JOIN,
    /**
     *     Syntax: S_CMOV_B32 SDST, SSRC0
    Operation:
    SDST = SCC ? SSRC0 : SDST
     */
    S_CMOV_B32,
    /**
     *    Syntax: S_CMOV_B64 SDST(2), SSRC0(2)
    Operation:
    SDST = SCC ? SSRC0 : SDST

     */
    S_CMOV_B64,
    /**
     *    Syntax: S_FF0_I32_B32 SDST, SSRC0
    Operation:
    SDST = -1
    for (UINT8 i = 0; i < 32; i++)
    if ((1U<<i) & SSRC0) == 0)
    { SDST = i; break; }

     */
    S_FF0_I32_B32,
    /**
     *     Syntax: S_FF0_I32_B64 SDST, SSRC0(2)
    Operation:
    SDST = -1
    for (UINT8 i = 0; i < 64; i++)
    if ((1ULL<<i) & SSRC0) == 0)
    { SDST = i; break; }

     */
    S_FF0_I32_B64,
    /**
     *     Syntax: S_FF1_I32_B32 SDST, SSRC0
    Operation:
    SDST = -1
    for (UINT8 i = 0; i < 32; i++)
    if ((1U<<i) & SSRC0) != 0)
    { SDST = i; break; }

     */
    S_FF1_I32_B32,
    /**
     *    Syntax: S_FF0_I32_B64 SDST, SSRC0(2)
    Operation:
    SDST = -1
    for (UINT8 i = 0; i < 64; i++)
    if ((1ULL<<i) & SSRC0) != 0)
    { SDST = i; break; }

     */
    S_FF1_I32_B64,
    /**
     *     Syntax: S_FLBIT_I32_B32 SDST, SSRC0
     *     Operation:
     *     SDST = -1
     *     for (INT8 i = 31; i >= 0; i--)
     *     if ((1U<<i) & SSRC0) != 0)
     *     { SDST = 31-i; break; }
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
     *    if ((1U<<i) & SSRC0) == (bitval<<i))
     *       { SDST = 31-i; break; }
     */
    S_FLBIT_I32,
    /**
     *     Syntax: S_FLBIT_I32_I64 SDST, SSRC0(2)
    Operation:
    SDST = -1
    UINT64 bitval = (INT64)SSRC0>=0 ? 1 : 0
    for (INT8 i = 63; i >= 0; i--)
    if ((1U<<i) & SSRC0) == (bitval<<i))
    { SDST = 63-i; break; }

     */
    S_FLBIT_I32_I64,
    /**
     *     Syntax: S_GETPC_B64 SDST(2)
     *     Operation:
     *     SDST = PC + 4
     */
    S_GETPC_B64,
    /**
     *     Syntax: S_MOV_B32 SDST, SSRC0
    Operation:
    SDST = SSRC0

     */
    S_MOV_B32,
    /**
     *     Syntax: S_MOV_B64 SDST(2), SSRC0(2)
    Operation:
    SDST = SSRC0

     */
    S_MOV_B64,
    /**
     *     Syntax: S_MOVRELD_B32 SDST, SSRC0
    Operation:
    SGPR[SDST_NUMBER + M0] = SSRC0

     */
    S_MOVRELD_B32,
    /**
     *     Syntax: S_MOVRELD_B64 SDST, SSRC0
    Operation:
    SGPR[SDST_NUMBER + M0 : SDST_NUMBER + M0 + 1] = SSRC0

     */
    S_MOVRELD_B64,
    /**
     *     Syntax: S_MOVRELS_B32 SDST, SSRC0
    Operation:
    SDST = SGPR[SSRC0_NUMBER + M0]

     */
    S_MOVRELS_B32,
    /**
     *     Syntax: S_MOVRELS_B64 SDST(2), SSRC0(2)
    Operation:
    SDST = SGPR[SSRC0_NUMBER + M0 : SSRC0_NUMBER + M0 + 1]
     */
    S_MOVRELS_B64,
    /**
     *     Syntax: S_NAND_SAVEEXEC_B64 SDST(2), SSRC0(2)
    Operation:
    SDST = EXEC
    EXEC = ~(SSRC0 & EXEC)
    SCC = EXEC!=0
     */
    S_NAND_SAVEEXEC_B64,
    /**
     *     Syntax: S_NOR_SAVEEXEC_B64 SDST(2), SSRC0(2)
    Operation:
    SDST = EXEC
    EXEC = ~(SSRC0 | EXEC)
    SCC = EXEC!=0
     */
    S_NOR_SAVEEXEC_B64,
    /**
     *     Syntax: S_NOT_B32 SDST, SSRC0
    Operation:
    SDST = ~SSRC0
    SCC = SDST!=0

     */
    S_NOT_B32,
    /**
     * Syntax: S_NOT_B64 SDST(2), SSRC0(2)
    Operation:
    SDST = ~SSRC0
    SCC = SDST!=0
     */
    S_NOT_B64,
    /**
     *     Syntax: S_OR_SAVEEXEC_B64 SDST(2), SDST(2)
    Operation:
    SDST = EXEC
    EXEC = SSRC0 | EXEC
    SCC = EXEC!=0
     */
    S_OR_SAVEEXEC_B64,
    /**
     *     Syntax: S_ORN2_SAVEEXEC_B64 SDST(2), SSRC0(2)
    Operation:
    SDST = EXEC
    EXEC = ~SSRC0 & EXEC
    SCC = EXEC!=0

     */
    S_ORN2_SAVEEXEC_B64,
    /**
     *    Syntax: S_QUADMASK_B32 SDST, SSRC0
    Operation:
    UINT32 temp = 0
    for (UINT8 i = 0; i < 8; i++)
    temp |= ((SSRC0>>(i<<2)) & 15)!=0 ? (1U<<i) : 0
    SDST = temp
    SCC = SDST!=0

     */
    S_QUADMASK_B32,
    /**
     *     Syntax: S_QUADMASK_B64 SDST(2), SSRC0(2)
    Operation:
    UINT64 temp = 0
    for (UINT8 i = 0; i < 16; i++)
    temp |= ((SSRC0>>(i<<2)) & 15)!=0 ? (1U<<i) : 0
    SDST = temp
    SCC = SDST!=0

     */
    S_QUADMASK_B64,
    /**
     * Syntax: S_RFE_B64 SSRC0(2)
    Operation: ???
    PC = TTMP[0:1]
     */
    S_RFE_B64,
    /**
     *    Syntax S_SET_GPR_IDX_IDX SSRC0(1)
    Operation:
    M0 = (M0 & 0xffffff00) | (SSRC0 & 0xff)
     */
    S_SET_GPR_IDX_IDX,
    /**
     *     Syntax: S_SETPC_B64 SSRC0(2)
     *     Operation:
     *     PC = SSRC0
     */
    S_SETPC_B64,
    /**
     *    Syntax: S_SEXT_I32_I8 SDST, SSRC0
     *    Operation:
     *    SDST = SEXT((INT8)SSRC0)
     */
    S_SEXT_I32_I8,
    /**
     *     Syntax: S_SEXT_I32_I16 SDST, SSRC0
     *     Operation:
     *     SDST = SEXT((INT16)SSRC0)
     */
    S_SEXT_I32_I16,
    /**
     *     Syntax: S_SWAPPC_B64 SDST(2), SSRC0(2)
     *     Operation:
     *     SDST = PC + 4
     *     PC = SSRC0
     */
    S_SWAPPC_B64,
    /**
     *     Syntax: S_WQM_B32 SDST, SSRC0
     *     Operation:
     *     UINT32 temp = 0
     *     for (UINT8 i = 0; i < 32; i+=4)
     *     temp |= ((SSRC0>>i) & 15)!=0 ? (15<<i) : 0
     *     SDST = temp
     *     SCC = SDST!=0
     */
    S_WQM_B32,
    /**
     *     Syntax: S_WQM_B64 SDST(2), SSRC0(2)
     *     Operation:
     *     UINT64 temp = 0
     *     for (UINT8 i = 0; i < 64; i+=4)
     *     temp |= ((SSRC0>>i) & 15)!=0 ? (15ULL<<i) : 0
     *     SDST = temp
     *     SCC = SDST!=0
     */
    S_WQM_B64,
    /**
     *     Syntax: S_XNOR_SAVEEXEC_B64 SDST(2), SSRC0(2)
     *     Operation:
     *     SDST = EXEC
     *     EXEC = ~(SSRC0 ^ EXEC)
     *     SCC = EXEC!=0
     */
    S_XNOR_SAVEEXEC_B64,
    /**
     *     Syntax: S_XOR_SAVEEXEC_B64 SDST(2), SSRC0(2)
     *     Operation:
     *     SDST = EXEC
     *     EXEC = SSRC0 ^ EXEC
     *     SCC = EXEC!=0
     */
    S_XOR_SAVEEXEC_B64,
    /**
     * if(EXEC == 0) then
     *     PC = PC + signext(SIMM16 * 4) + 4;
     * endif.
     */
    S_CBRANCH_EXECZ,

    //SOP2
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
     *   SDST = SSRC0 + SSRC1
     *   INT64 temp = SEXT64(SSRC0) + SEXT64(SSRC1)
     *   SCC = temp > ((1LL<<31)-1) || temp < (-1LL<<31)
     */
    S_ADD_I32,
    /**
     * D.u = S0.u + S1.u;
     * SCC = (S0.u + S1.u >= 0x100000000ULL ? 1 : 0). // unsigned overflow/carry-out, S_ADDC_U32
     */
    S_ADD_U32,
    /**
     *     SDST = SSRC0 & SSRC1
     *     SCC = SDST!=0
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
     * UINT8 shift = SSRC1 & 31
     * UINT8 length = (SSRC1>>16) & 0x7f
     * if (length==0)
     *      SDST = 0
     * if (shift+length < 32)
     *      SDST = (INT32)(SSRC0 << (32 - shift - length)) >> (32 - length)
     * else
     *      SDST = (INT32)SSRC0 >> shift
     * SCC = SDST!=0
     */
    S_BFE_I32,
    /**
     * UINT8 shift = SSRC1 & 63
     * UINT8 length = (SSRC1>>16) & 0x7f
     * if (length==0)
     *      SDST = 0
     * if (shift+length < 64)
     *      SDST = (INT64)(SSRC0 << (64 - shift - length)) >> (64 - length)
     *  else
     *      SDST = (INT64)SSRC0 >> shift
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
     * Description: Return from exception handler and set: INST_ATC = SSRC1.U32[0] ???
     */
    S_RFE_RESTORE_B64,
    /**
     * Syntax: S_SUBB_U32 SDST, SSRC0, SSRC1
     * Operation:
     * UINT64 temp = (UINT64)
     * SSRC0 - (UINT64)SSRC1 - SCC
     * SDST = temp
     * SCC = (temp >> 32) & 1
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
     * UINT64 temp = (UINT64)
     * SSRC0 - (UINT64)SSRC1
     * SDST = temp
     * SCC = (temp >> 32) != 0
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

//VOP1

/**
 * D.u = S0.u.
 * Input and output modifiers not supported; this is an untyped operation.
 */
    V_MOV_B32,

//VOP2

/**
 * D.u = S0.u + S1.u.
 */
    V_ADD_U32,

/**
 * todo not in ofdoc
 */
    V_ADDC_U32,

//VOP3A
/**
 * D.u64 = S1.u64 << S0.u[5:0].
 */
    V_LSHLREV_B64,

//VOPC
/**
 * D.u64[threadId] = (S0 == S1).
 */
    V_CMP_EQ_I32,

//FLAT
/**
 * Untyped buffer store dword
 */
    FLAT_STORE_DWORD
};

char const *get_instruction_key_str(InstructionKey instruction) {
  static std::unordered_map<InstructionKey, char const *> instruction_repo
    {
      //SMEM
      {S_LOAD_DWORD,           "s_load_dword"},
      {S_LOAD_DWORDX2,         "s_load_dwordx2"},

      //SOPC
      {S_CMP_LT_I32,           "s_cmp_lt_i32"},

      //SOPP
      {S_WAITCNT,              "s_waitcnt"},
      {S_CBRANCH_SCC0,         "s_cbranch_scc0"},

      //SOP1
      {S_ABS_I32,              "s_abs_i32"},
      {S_AND_SAVEEXEC_B64,     "s_and_saveexec_b64"},
      {S_ANDN1_SAVEEXEC_B64,   "s_andn1_saveexec_b64"},
      {S_ANDN1_WREXEC_B64,     "s_andn1_wrexec_b64"},
      {S_ANDN2_SAVEEXEC_B64,   "s_andn2_saveexec_b64"},
      {S_ANDN2_WREXEC_B64,     "s_andn2_wrexec_b64"},
      {S_BCNT0_I32_B32,        "s_bcnt0_i32_b32"},
      {S_BCNT0_I32_B64,        "s_bcnt0_i32_b64"},
      {S_BCNT1_I32_B32,        "s_bcnt1_i32_b32"},
      {S_BCNT1_I32_B64,        "s_bcnt1_i32_b64"},
      {S_BITREPLICATE_B64_B32, "s_bitreplicate_b64_b32"},
      {S_BITSET0_B32,          "s_bitset0_b32"},
      {S_BITSET0_B64,          "s_bitset0_b64"},
      {S_BITSET1_B32,          "s_bitset1_b32"},
      {S_BITSET1_B64,          "s_bitset1_b64"},
      {S_BREV_B32,             "s_brev_b32"},
      {S_BREV_B64,             "s_brev_b64"},
      {S_CBRANCH_JOIN,         "s_cbranch_join"},
      {S_CMOV_B32,             "s_cmov_b32"},
      {S_CMOV_B64,             "s_cmov_b64"},
      {S_FF0_I32_B32,          "s_ff0_i32_b32"},
      {S_FF0_I32_B64,          "s_ff0_i32_b64"},
      {S_FF1_I32_B32,          "s_ff1_i32_b32"},
      {S_FF1_I32_B64,          "s_ff1_i32_b64"},
      {S_FLBIT_I32_B32,        "s_flbit_i32_b32"},
      {S_FLBIT_I32_B64,        "s_flbit_i32_b64"},
      {S_FLBIT_I32,            "s_flbit_i32"},
      {S_FLBIT_I32_I64,        "s_flbit_i32_i64"},
      {S_GETPC_B64,            "s_getpc_b64"},
      {S_MOV_B32,              "s_mov_b32"},
      {S_MOV_B64,              "s_mov_b64"},
      {S_MOVRELD_B32,          "s_movreld_b32"},
      {S_MOVRELD_B64,          "s_movreld_b64"},
      {S_MOVRELS_B32,          "s_movrels_b32"},
      {S_MOVRELS_B64,          "s_movrels_b64"},
      {S_NAND_SAVEEXEC_B64,    "s_nand_saveexec_b64"},
      {S_NOR_SAVEEXEC_B64,     "s_nor_saveexec_b64"},
      {S_NOT_B32,              "s_not_b32"},
      {S_NOT_B64,              "s_not_b64"},
      {S_OR_SAVEEXEC_B64,      "s_or_saveexec_b64"},
      {S_ORN2_SAVEEXEC_B64,    "s_orn2_saveexec_b64"},
      {S_QUADMASK_B32,         "s_quadmask_b32"},
      {S_QUADMASK_B64,         "s_quadmask_b64"},
      {S_RFE_B64,              "s_rfe_b64"},
      {S_SET_GPR_IDX_IDX,      "s_set_gpr_idx_idx"},
      {S_SETPC_B64,            "s_setpc_b64"},
      {S_SEXT_I32_I8,          "s_sext_i32_i8"},
      {S_SEXT_I32_I16,         "s_sext_i32_i16"},
      {S_SWAPPC_B64,           "s_swappc_b64"},
      {S_WQM_B32,              "s_wqm_b32"},
      {S_WQM_B64,              "s_wqm_b64"},
      {S_XNOR_SAVEEXEC_B64,    "s_xnor_saveexec_b64"},
      {S_XOR_SAVEEXEC_B64,     "s_xor_saveexec_b64"},
      {S_CBRANCH_EXECZ,        "s_cbranch_execz"},

      //SOP2
      {S_ABSDIFF_I32,          "s_absdiff_i32"},
      {S_ADDC_U32,             "s_addc_u32"},
      {S_ADD_I32,              "s_add_i32"},
      {S_ADD_U32,              "s_add_u32"},
      {S_AND_B32,              "s_and_b32"},
      {S_AND_B64,              "s_and_b64"},
      {S_ANDN2_B32,            "s_andn2_b32"},
      {S_ANDN2_B64,            "s_andn2_b64"},
      {S_ASHR_I32,             "s_ashr_i32"},
      {S_ASHR_I64,             "s_ashr_i64"},
      {S_BFE_I32,              "s_bfe_i32"},
      {S_BFE_I64,              "s_bfe_i64"},
      {S_BFE_U32,              "s_bfe_u32"},
      {S_BFE_U64,              "s_bfe_u64"},
      {S_BFM_B32,              "s_bfm_b32"},
      {S_BFM_B64,              "s_bfm_b64"},
      {S_CBRANCH_G_FORK,       "s_cbranch_g_fork"},
      {S_CSELECT_B32,          "s_cselect_b32"},
      {S_CSELECT_B64,          "s_cselect_b64"},
      {S_LSHL_B32,             "s_lshl_b32"},
      {S_LSHL_B64,             "s_lshl_b64"},
      {S_LSHL1_ADD_U32,        "s_lshl1_add_u32"},
      {S_LSHL2_ADD_U32,        "s_lshl2_add_u32"},
      {S_LSHL3_ADD_U32,        "s_lshl3_add_u32"},
      {S_LSHL4_ADD_U32,        "s_lshl4_add_u32"},
      {S_LSHR_B32,             "s_lshr_b32"},
      {S_LSHR_B64,             "s_lshr_b64"},
      {S_MAX_I32,              "s_max_i32"},
      {S_MAX_U32,              "s_max_u32"},
      {S_MIN_I32,              "s_min_i32"},
      {S_MIN_U32,              "s_min_u32"},
      {S_MUL_HI_I32,           "s_mul_hi_i32"},
      {S_MUL_HI_U32,           "s_mul_hi_u32"},
      {S_MUL_I32,              "s_mul_i32"},
      {S_NAND_B32,             "s_nand_b32"},
      {S_NAND_B64,             "s_nand_b64"},
      {S_NOR_B32,              "s_nor_b32"},
      {S_NOR_B64,              "s_nor_b64"},
      {S_OR_B32,               "s_or_b32"},
      {S_OR_B64,               "s_or_b64"},
      {S_ORN2_B32,             "s_orn2_b32"},
      {S_ORN2_B64,             "s_orn2_b64"},
      {S_PACK_HH_B32_B16,      "s_pack_hh_b32_b16"},
      {S_PACK_LH_B32_B16,      "s_pack_lh_b32_b16"},
      {S_PACK_LL_B32_B16,      "s_pack_ll_b32_b16"},
      {S_RFE_RESTORE_B64,      "s_rfe_restore_b64"},
      {S_SUBB_U32,             "s_subb_u32"},
      {S_SUB_I32,              "s_sub_i32"},
      {S_SUB_U32,              "s_sub_u32"},
      {S_XNOR_B32,             "s_xnor_b32"},
      {S_XNOR_B64,             "s_xnor_b64"},
      {S_XOR_B32,              "s_xor_b32"},
      {S_XOR_B64,              "s_xor_b64"},

      //VOP1
      {V_MOV_B32,              "v_mov_b32"},

      //VOP2
      {V_ADD_U32,              "v_add_u32"},
      {V_ADDC_U32,             "v_addc_u32"},

      //VOP3A
      {V_LSHLREV_B64,          "v_lshlrev_b64"},

      //VOPC
      {V_CMP_EQ_I32,           "v_cmp_eq_i32"},

      //FLAT
      {FLAT_STORE_DWORD,       "flat_store_dword"}
    };

  auto it = instruction_repo.find(instruction);

  if (it == instruction_repo.end()) {
    throw std::runtime_error("Undefined instruction!");
  }

  return it->second;
}