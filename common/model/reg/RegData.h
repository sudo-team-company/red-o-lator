#ifndef RED_O_LATOR_REGDATA_H
#define RED_O_LATOR_REGDATA_H

struct RegData {
    std::vector<uint32_t> sFile;
    std::vector<uint32_t> vFile;

    uint64_t RELADDR = 0;
    uint64_t EXEC = 0;
    uint64_t VCC = 0;
    uint32_t M0 = 0;
    uint32_t SIMM16 = 0;
    bool SCC = false;
};

#endif  // RED_O_LATOR_REGDATA_H
