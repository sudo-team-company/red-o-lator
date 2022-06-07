#include "IpcClient.h"
#include <nngpp/protocol/req0.h>

void IpcClient::connect() {
    socket = nng::req::open();
    socket.dial("ipc://red-o-lator.ipc");
}

std::string IpcClient::send(const std::string& msg) {
    socket.send(nng::view(msg.data(), msg.size()));
    nng::buffer buf = socket.recv();
    return std::string{(const char*) buf.data(), buf.size()};
}
