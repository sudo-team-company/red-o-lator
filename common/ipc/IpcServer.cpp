#include "IpcServer.h"
#include <nngpp/nngpp.h>
#include <nngpp/protocol/rep0.h>
#include <functional>
#include <memory>
#include "util.h"

void checkResult(const nng::aio& aio) {
    nng::error result = aio.result();
    if (result != nng::error::success) {
        throw nng::exception(result);
    }
}

nng::msg toMsg(const std::string& s) {
    nng::msg m = nng::make_msg(s.size());
    m.body().clear();
    m.body().append(nng::view(s.data(), s.size()));
    return m;
}

std::string fromMsg(const nng::msg& msg) {
    return std::string{(const char*) msg.body().data(), msg.body().size()};
}

void IpcServer::listen() {
    socket = nng::rep::open();
    socket.listen(IPC_URL);

    for (auto& job : jobs) {
        job = std::make_unique<ServerJob>(socketHandlerWrapper, socket, this);
        socketHandler(job.get());
    }
}

void IpcServer::socketHandler(ServerJob* job) {
    switch (job->state) {
        case ServerJob::INIT:
            job->state = ServerJob::RECEIVE;
            job->ctx.recv(job->aio);
            break;

        case ServerJob::RECEIVE:
            checkResult(job->aio);
            processJob(job);
            job->state = ServerJob::SEND;
            job->ctx.send(job->aio);
            break;

        case ServerJob::SEND:
            checkResult(job->aio);
            job->state = ServerJob::RECEIVE;
            job->ctx.recv(job->aio);
            break;

        default: throw nng::exception(nng::error::state);
    }
}

void IpcServer::socketHandlerWrapper(void* arg) {
    auto* job = (struct ServerJob*) arg;
    job->server->socketHandler(job);
}

void IpcServer::processJob(ServerJob* job) {
    nng::msg msg = job->aio.release_msg();
    std::string res = messageProcessor(fromMsg(msg));
    job->aio.set_msg(std::move(toMsg(res)));
}

IpcServer::IpcServer(std::function<std::string(const std::string&)> messageProcessor)
    : messageProcessor(messageProcessor) {}

IpcServer::~IpcServer() {
    for (auto& job : jobs) {
        job->aio.release();
    }
}
