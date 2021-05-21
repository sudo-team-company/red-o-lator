#ifndef RED_O_LATOR_HEX_UTILS_H
#define RED_O_LATOR_HEX_UTILS_H

inline size_t getStartAddress(size_t address) {
    return address & ~0xF;
}

inline size_t getEndAddress(size_t address, size_t size) {
    return (address + size + 16) & ~0xF;
}

inline size_t getLineCount(size_t startAddress, size_t endAddress) {
    return (endAddress - startAddress) / 16;
}

inline size_t getLineAddress(size_t startAddress, size_t currentLine) {
    return startAddress + currentLine * 16;
}

#endif  // RED_O_LATOR_HEX_UTILS_H
