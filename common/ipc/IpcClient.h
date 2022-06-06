#ifndef RED_O_LATOR_IPCCLIENT_H
#define RED_O_LATOR_IPCCLIENT_H

#include <nngpp/nngpp.h>
#include <string>

class IpcClient {
   private:
    nng::socket socket;

   public:
    void connect();
    std::string send(const std::string& msg);
};

#endif  // RED_O_LATOR_IPCCLIENT_H
