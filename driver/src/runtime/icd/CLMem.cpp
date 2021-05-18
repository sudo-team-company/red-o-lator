#include "CLMem.h"

CLMem::CLMem(IcdDispatchTable* dispatchTable, CLContext* context)
    : dispatchTable(dispatchTable), context(context) {
    clRetainContext(context);
}

CLMem::~CLMem() {
    while (!destructorCallbacks.empty()) {
        const auto callback = destructorCallbacks.top();
        callback.function(this, callback.userData);
        destructorCallbacks.pop();
    }

    clReleaseContext(context);
}

void CLMem::registerCallback(CLMemDestructorCallback callback) {
    destructorCallbacks.push(callback);
}
