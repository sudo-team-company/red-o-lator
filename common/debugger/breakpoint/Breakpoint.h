#ifndef RED_O_LATOR_BREAKPOINT_H
#define RED_O_LATOR_BREAKPOINT_H

#include <cstdint>

struct Breakpoint {
    enum Type { GLOBAL, LOCAL };
    Type type;
    uint32_t address;
    int wfId;

    explicit Breakpoint(uint32_t address) : address{address}, type{GLOBAL}, wfId{0} {};
    explicit Breakpoint(uint32_t address, int wavefrontId)
        : address{address}, type{LOCAL}, wfId{wavefrontId} {};
};

#endif  // RED_O_LATOR_BREAKPOINT_H
