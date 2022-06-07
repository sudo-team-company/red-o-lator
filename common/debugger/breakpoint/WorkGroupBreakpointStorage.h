#ifndef RED_O_LATOR_WORKGROUPBREAKPOINTSTORAGE_H
#define RED_O_LATOR_WORKGROUPBREAKPOINTSTORAGE_H

#include <set>
#include "Breakpoint.h"

class WorkGroupBreakpointStorage {
   private:
    typedef std::pair<Breakpoint::Type, uint32_t> BPEntry;
    std::set<BPEntry> breakpoints;

   public:
    void addBreakpoint(const Breakpoint& breakpoint);
    void removeBreakpoint(const Breakpoint& breakpoint);
    bool checkBreakpoint(uint32_t address);
};

#endif  // RED_O_LATOR_WORKGROUPBREAKPOINTSTORAGE_H
