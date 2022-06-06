#ifndef RED_O_LATOR_IPCSERVER_H
#define RED_O_LATOR_IPCSERVER_H

#include <functional>
#include <memory>
#include <string>
#include "ServerJob.h"

class IpcServer {
   private:
    static constexpr int JOB_COUNT = 5;
    std::unique_ptr<ServerJob> jobs[JOB_COUNT];

    nng::socket socket;

    std::function<std::string(const std::string&)> messageProcessor;

    void processJob(ServerJob* job);
    void socketHandler(ServerJob* arg);
    static void socketHandlerWrapper(void* arg);

   public:
    IpcServer(std::function<std::string(const std::string&)> messageProcessor);
    ~IpcServer();
    void listen();
};

#endif  // RED_O_LATOR_IPCSERVER_H
