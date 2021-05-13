#pragma once

// TODO(IcdDispatchTable): change to singleton with static method?
struct IcdDispatchTable {
    void* entries[256];
    int entryCount;
};

class IcdDispatchTableProvider {
   public:
    IcdDispatchTable* get();

   private:
    IcdDispatchTable* mDispatchTable = nullptr;
};
