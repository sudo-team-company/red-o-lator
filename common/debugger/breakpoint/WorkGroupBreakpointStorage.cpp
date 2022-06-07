#include "WorkGroupBreakpointStorage.h"

void WorkGroupBreakpointStorage::addBreakpoint(const Breakpoint& breakpoint) {
    breakpoints.insert({breakpoint.type, breakpoint.address});
}

void WorkGroupBreakpointStorage::removeBreakpoint(
    const Breakpoint& breakpoint) {
    breakpoints.erase({breakpoint.type, breakpoint.address});
}

bool WorkGroupBreakpointStorage::checkBreakpoint(uint32_t address) {
    return breakpoints.count({Breakpoint::LOCAL, address}) > 0 ||
           breakpoints.count({Breakpoint::GLOBAL, address}) > 0;
}
