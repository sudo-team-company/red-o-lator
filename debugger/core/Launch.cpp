#include "Launch.h"

Launch::Launch(const Kernel& kernel)
    : kernel{kernel},
      wgChannels{} {}
