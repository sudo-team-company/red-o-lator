#ifndef RED_O_LATOR_SERVERJOB_H
#define RED_O_LATOR_SERVERJOB_H

#include <nngpp/nngpp.h>
#include "IpcServer.h"

class IpcServer;

struct ServerJob {
    enum { INIT, RECEIVE, SEND } state = INIT;

    nng::aio aio;
    nng::msg msg;
    nng::ctx ctx;

    IpcServer* server;

    explicit ServerJob(void (*func)(void*),
                       nng::socket_view socket,
                       IpcServer* server)
        : ctx(socket), aio{func, this}, server(server) {}
};

#endif  // RED_O_LATOR_SERVERJOB_H
