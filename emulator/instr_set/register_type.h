/**
 * @author dkudaiberdieva
 */

enum RegisterType {
    /**
     * Points to the memory address of the next shader instruction to execute.
     */
    PC,

    /**
     * V0-V255 - Vector general-purpose register
     */
      V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15, V16, V17, V18, V19, V20, V21, V22, V23, V24, V25, V26, V27, V28, V29, V30, V31, V32, V33, V34, V35, V36, V37, V38, V39, V40, V41, V42, V43, V44, V45, V46, V47, V48, V49, V50, V51, V52, V53, V54, V55, V56, V57, V58, V59, V60, V61, V62, V63, V64, V65, V66, V67, V68, V69, V70, V71, V72, V73, V74, V75, V76, V77, V78, V79, V80, V81, V82, V83, V84, V85, V86, V87, V88, V89, V90, V91, V92, V93, V94, V95, V96, V97, V98, V99, V100, V101, V102, V103, V104, V105, V106, V107, V108, V109, V110, V111, V112, V113, V114, V115, V116, V117, V118, V119, V120, V121, V122, V123, V124, V125, V126, V127, V128, V129, V130, V131, V132, V133, V134, V135, V136, V137, V138, V139, V140, V141, V142, V143, V144, V145, V146, V147, V148, V149, V150, V151, V152, V153, V154, V155, V156, V157, V158, V159, V160, V161, V162, V163, V164, V165, V166, V167, V168, V169, V170, V171, V172, V173, V174, V175, V176, V177, V178, V179, V180, V181, V182, V183, V184, V185, V186, V187, V188, V189, V190, V191, V192, V193, V194, V195, V196, V197, V198, V199, V200, V201, V202, V203, V204, V205, V206, V207, V208, V209, V210, V211, V212, V213, V214, V215, V216, V217, V218, V219, V220, V221, V222, V223, V224, V225, V226, V227, V228, V229, V230, V231, V232, V233, V234, V235, V236, V237, V238, V239, V240, V241, V242, V243, V244, V245, V246, V247, V248, V249, V250, V251, V252, V253, V254, V255,

    /**
     * S0-S103 - Scalar general-purpose register
     */
    S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15, S16, S17, S18, S19, S20, S21, S22, S23, S24, S25, S26, S27, S28, S29, S30, S31, S32, S33, S34, S35, S36, S37, S38, S39, S40, S41, S42, S43, S44, S45, S46, S47, S48, S49, S50, S51, S52, S53, S54, S55, S56, S57, S58, S59, S60, S61, S62, S63, S64, S65, S66, S67, S68, S69, S70, S71, S72, S73, S74, S75, S76, S77, S78, S79, S80, S81, S82, S83, S84, S85, S86, S87, S88, S89, S90, S91, S92, S93, S94, S95, S96, S97, S98, S99, S100, S101, S102, S103,

    /**
     * Local data share is a scratch RAM with built-in arithmetic capabilities that allow data to be shared between threads in a workgroup.
     * This memory (64 kB total) is configured with 32 banks, each with 512 entries of 4 bytes.
     */
    LDS,
    /**
     *  A bit mask with one bit per thread, which is applied to vector instructions and controls that threads execute and that ignore the instruction.
     */
    EXEC,
    /**
     * A single bit flag indicating that the EXEC mask is all zeros.
     */
    EXECZ,
    /**
     * A bit mask with one bit per thread; it holds the result of a vector compare operation.
     */
    VCC,
    /**
     * A single bit-flag indicating that the VCC mask is all zeros.
     */
    VCCZ,
    /**
     * Result from a scalar ALU comparison instruction.
     */
    SCC,
    /**
     * The base address of scratch memory
     */
    FLAT_SCRATCH,
    /**
     * Bit mask of threads that have failed their address translation.
     */
    XNACK_MASK,
    /**
     * Read-only shader status bar
     */
    STATUS,
    /**
     * Writable shader mode bits
     */
    MODE,
    /**
     * A temporary register that has various uses, including GPR indexing and bounds checking
     */
    M0,
    /**
     * Holds information about exceptions and pending traps
     */
    TRAPSTS,
    /**
     * Holds the pointer to the current trap handler program
     */
    TBA,
    /**
     * Temporary register for shader operations. For example, can hold a pointer to memory used by the trap handler
     */
    TMA,
    
    /**
     * TTMP0-TTMP15 - Trap Temporary SGPRs. 16 SGPRs available only to the Trap Handler for temporary storage.
     */
    TTMP0, TTMP1, TTMP2, TTMP3, TTMP4, TTMP5, TTMP6, TTMP7, TTMP8, TTMP9, TTMP10, TTMP11, TTMP12, TTMP13, TTMP14, TTMP15,

    /**
     * Counts the number of VMEM instructions issued but not yet completed
     */
    VMCNT,
    /**
     * Counts the number of Export and GDS instructions issued but not yet completed. Also counts VMEM writes that have not yet sent their write-data to the TC.
     */
    EXPCNT,
    /**
     * Counts the number of LDS, GDS, constant-fetch (scalar memory read), and message instructions issued but not yet completed
     */
    LGKMCNT
};

