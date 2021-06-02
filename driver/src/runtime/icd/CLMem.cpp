#include "CLMem.h"

CLMem::CLMem(IcdDispatchTable* dispatchTable, CLContext* context)
    : dispatchTable(dispatchTable), context(context) {
    clRetainContext(context);
}

CLMem::~CLMem() {
    while (!mDestructorCallbacks.empty()) {
        const auto callback = mDestructorCallbacks.top();
        callback.function(this, callback.userData);
        mDestructorCallbacks.pop();
    }

    clReleaseContext(context);
}

void CLMem::registerCallback(const CLMemDestructorCallback& callback) {
    mDestructorCallbacks.push(callback);
}
