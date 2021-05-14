#pragma once

#include <string>

#include "icd.h"

struct CLPlatformId {
    explicit CLPlatformId(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
    std::string profile;
    std::string openClVersion;
    std::string driverVersion;
    std::string name;
    std::string vendor;
    std::string extensions;
    std::string suffix;
};
