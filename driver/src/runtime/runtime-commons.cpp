#include "runtime-commons.h"

IcdDispatchTable* kDispatchTable =  // NOLINT(cert-err58-cpp)
    IcdDispatchTableProvider().get();

DeviceConfigurationParser kDeviceConfigurationParser =
    DeviceConfigurationParser();

CLPlatformId* kPlatform = nullptr;
CLDeviceId* kDevice = nullptr;
