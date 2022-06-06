#ifndef RED_O_LATOR_LAUNCH_H
#define RED_O_LATOR_LAUNCH_H

#include <string>
#include <utility>
#include <vector>
#include "debugger/channel/Channel.h"
#include "model/kernel/Kernel.h"

struct Launch {
    const Kernel& kernel;
    size_t workGroupCnt;
    std::vector<Channel<std::string>> wgChannels;

    explicit Launch(const Kernel& kernel);

};

#endif  // RED_O_LATOR_LAUNCH_H
