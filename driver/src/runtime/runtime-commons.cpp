#include "icd.h"

IcdDispatchTable* mDispatchTable =  // NOLINT(cert-err58-cpp)
    IcdDispatchTableProvider().get();
