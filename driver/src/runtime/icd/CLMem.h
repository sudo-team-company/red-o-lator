#pragma once

#include <cstddef>
#include <memory>
#include <stack>

#include "icd.h"

using CLMemDestructorCallbackFunction = void(CL_CALLBACK*)(cl_mem mem_obj,
                                                           void* user_data);

struct CLMemDestructorCallback {
    CLMemDestructorCallback(CLMemDestructorCallbackFunction callback,
                            void* userData)
        : function(callback), userData(userData) {}

    CLMemDestructorCallbackFunction function;
    void* userData;
};

struct CLMem {
   public:
    explicit CLMem(IcdDispatchTable* dispatchTable, CLContext* context);

    ~CLMem();

    IcdDispatchTable* const dispatchTable;
    CLContext* const context;

    std::byte* address = nullptr;
    size_t size = 0;

    bool kernelCanRead = false;
    bool kernelCanWrite = false;

    bool hostCanRead = false;
    bool hostCanWrite = false;

    unsigned int referenceCount = 1;

    void registerCallback(const CLMemDestructorCallback& callback);

   private:
    std::stack<CLMemDestructorCallback> mDestructorCallbacks{};
};
