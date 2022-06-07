#ifndef RED_O_LATOR_IPCWRAPPER_H
#define RED_O_LATOR_IPCWRAPPER_H

#include "ipc/proto/KernelProto.h"

class IPCWrapper {
   public:
    static void sendKernelForExecution(const KernelProto& kernel);
    static KernelProto receiveKernelForExecution(const std::string& source);
};

#endif  // RED_O_LATOR_IPCWRAPPER_H
