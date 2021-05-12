#include "runtime-commons.h"

Logger kLogger = Logger("[red-o-lator driver] ---");  // NOLINT(cert-err58-cpp)

IcdDispatchTable* kDispatchTable =  // NOLINT(cert-err58-cpp)
    IcdDispatchTableProvider().get();

DeviceConfigurationParser kDeviceConfigurationParser =
    DeviceConfigurationParser();

CLPlatformId* kPlatform = nullptr;
CLDeviceId* kDevice = nullptr;

bool utils::isMutuallyExclusive(cl_bitfield flags,
                                std::initializer_list<cl_int> checkFlags) {
    bool foundFlag = false;

    for (auto flag : checkFlags) {
        const bool flagIsSet = (flags & flag);
        if (flagIsSet && !foundFlag) {
            foundFlag = true;
        } else if (flagIsSet) {
            return false;
        }
    }

    return true;
}
