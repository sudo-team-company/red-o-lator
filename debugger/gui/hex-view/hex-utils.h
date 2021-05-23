#ifndef RED_O_LATOR_HEX_UTILS_H
#define RED_O_LATOR_HEX_UTILS_H

#include <cstdint>
#include <cstddef>

inline uint64_t getStartAddress(uint64_t address) {
    return address & ~0xF;
}

inline uint64_t getEndAddress(uint64_t address, uint64_t size) {
    return (address + size + 16) & ~0xF;
}

inline uint64_t getLineCount(uint64_t startAddress, uint64_t endAddress) {
    return (endAddress - startAddress) / 16;
}

inline uint64_t getLineAddress(uint64_t startAddress, size_t currentLine) {
    return startAddress + currentLine * 16;
}

#endif  // RED_O_LATOR_HEX_UTILS_H
