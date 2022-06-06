#include "BreakpointStorage.h"

void BreakpointStorage::addBreakpoint(const Breakpoint& breakpoint) {
    switch (breakpoint.type) {
        case Breakpoint::GLOBAL:
            globalBreakpoints.insert(breakpoint.address);
            break;
        case Breakpoint::LOCAL:
            localBreakpoints[breakpoint.wfId].insert(breakpoint.address);
            break;
    }
}

void BreakpointStorage::removeBreakpoint(const Breakpoint& breakpoint) {
    switch (breakpoint.type) {
        case Breakpoint::GLOBAL:
            globalBreakpoints.erase(breakpoint.address);
            break;
        case Breakpoint::LOCAL:
            localBreakpoints[breakpoint.wfId].erase(breakpoint.address);
            break;
    }
}

std::vector<Breakpoint> BreakpointStorage::getWavefrontBreakpoints(
    int workGroupId) {
    std::vector<Breakpoint> result;

    for (uint32_t addr : globalBreakpoints) {
        result.emplace_back(addr);
    }
    for (uint32_t addr : localBreakpoints[workGroupId]) {
        result.emplace_back(addr, workGroupId);
    }

    return result;
}

void BreakpointStorage::clear() {
    globalBreakpoints.clear();
    localBreakpoints.clear();
}

bool BreakpointStorage::hasBreakpoint(uint32_t address) {
    return globalBreakpoints.find(address) != globalBreakpoints.end();
}

bool BreakpointStorage::hasBreakpoint(uint32_t address, int wfId) {
    return globalBreakpoints.find(address) != globalBreakpoints.end() ||
           localBreakpoints[wfId].find(address) != localBreakpoints[wfId].end();
}
