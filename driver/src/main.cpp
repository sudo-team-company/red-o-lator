#include <iostream>
#include "runtime/runtime-commons.h"

int main() {
    const auto deviceConfigurationFile = "resources/rx-550.ini";
    kDeviceConfigurationParser.load(deviceConfigurationFile);
}
