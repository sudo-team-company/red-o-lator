#ifndef RED_O_LATOR_ICDDISPATCHTABLE_H
#define RED_O_LATOR_ICDDISPATCHTABLE_H

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

#endif  // RED_O_LATOR_ICDDISPATCHTABLE_H
