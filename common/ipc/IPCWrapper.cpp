#include "IPCWrapper.h"
#include "IpcClient.h"
#include "ipc/proto/KernelProto.h"
#include "kernel_request.pb.h"

void IPCWrapper::sendKernelForExecution(const KernelProto& kernel) {
    control::KernelRequest req;
    req.set_name(kernel.name);

    control::Config* c = req.mutable_config();
    c->set_dims(kernel.config.dims);
    for (const auto& u : kernel.config.globalWorkOffset) {
        c->add_globalworkoffset(u);
    }
    for (const auto& u : kernel.config.globalWorkSize) {
        c->add_globalworksize(u);
    }
    for (const auto& u : kernel.config.localWorkSize) {
        c->add_localworksize(u);
    }
    for (const auto& p : kernel.config.parameters) {
        c->add_parameters(p);
    }

    for (const auto& i : kernel.instructions) {
        req.add_instructions(i);
    }


    for (const auto& arg : kernel.args) {
        control::Argument* p = req.add_args();
        p->set_name(arg.name);
        if (arg.kind == ArgumentProto::POINTER) {
            p->set_kind(control::Argument_Kind_POINTER);
        } else {
            p->set_kind(control::Argument_Kind_SCALAR);
        }
        p->set_data(arg.data);
        p->set_size(arg.size);
        p->set_readpermission(arg.readPermission);
        p->set_writepermission(arg.writePermission);
    }

    std::string str = req.SerializeAsString();

    IpcClient client;
    client.connect();
    std::string res = client.send(str);
}

KernelProto IPCWrapper::receiveKernelForExecution(const std::string& source) {
    KernelProto res;

    control::KernelRequest req;
    req.ParseFromString(source);

    res.name = req.name();

    for (const auto& i : req.instructions()) {
        res.instructions.push_back(i);
    }

    res.config.dims = req.config().dims();

    for (const auto& p : req.config().parameters()) {
        res.config.parameters.push_back(p);
    }
    for (const auto& u : req.config().globalworkoffset()) {
        res.config.globalWorkOffset.push_back(u);
    }
    for (const auto& u : req.config().globalworksize()) {
        res.config.globalWorkSize.push_back(u);
    }
    for (const auto& u : req.config().localworksize()) {
        res.config.localWorkSize.push_back(u);
    }

    for (const auto& a : req.args()) {
        ArgumentProto ap;
        ap.name = a.name();
        ap.data = a.data();
        ap.size = a.size();
        ap.readPermission = a.readpermission();
        ap.writePermission = a.writepermission();
        if (a.kind() == control::Argument_Kind_POINTER) {
            ap.kind = ArgumentProto::POINTER;
        } else {
            ap.kind = ArgumentProto::SCALAR;
        }
        res.args.push_back(ap);
    }

    return res;
}
