#ifndef RED_O_LATOR_BREAKPOINTSTORAGE_H
#define RED_O_LATOR_BREAKPOINTSTORAGE_H

#include <map>
#include <set>
#include <vector>
#include "Breakpoint.h"

class BreakpointStorage {
   private:
    std::set<uint32_t> globalBreakpoints;
    std::map<int, std::set<uint32_t>> localBreakpoints;

   public:
    void addBreakpoint(const Breakpoint& breakpoint);
    void removeBreakpoint(const Breakpoint& breakpoint);
    bool hasBreakpoint(uint32_t address);
    bool hasBreakpoint(uint32_t address, int wfId);
    std::vector<Breakpoint> getWavefrontBreakpoints(int wfId);
    void clear();
};

#endif  // RED_O_LATOR_BREAKPOINTSTORAGE_H
