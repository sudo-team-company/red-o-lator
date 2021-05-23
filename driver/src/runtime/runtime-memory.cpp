#include <iostream>
#include <memory>
#include "icd/CLContext.h"
#include "icd/CLDeviceId.hpp"
#include "icd/CLMem.h"
#include "runtime-commons.h"

CL_API_ENTRY cl_int CL_API_CALL clRetainMemObject(cl_mem memobj) {
    if (!memobj) {
        RETURN_ERROR(CL_INVALID_MEM_OBJECT, "Memory object is null.")
    }

    memobj->referenceCount++;

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseMemObject(cl_mem memobj) {
    if (!memobj) {
        RETURN_ERROR(CL_INVALID_MEM_OBJECT, "Memory object is null.")
    }

    memobj->referenceCount--;

    if (memobj->referenceCount == 0) {
        delete[] memobj->address;
        memobj->context->device->usedGlobalMemory -= memobj->size;
        delete memobj;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetMemObjectInfo(cl_mem memobj,
                   cl_mem_info param_name,
                   size_t param_value_size,
                   void* param_value,
                   size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call: clGetMemObjectInfo"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueUnmapMemObject(cl_command_queue command_queue,
                        cl_mem memobj,
                        void* mapped_ptr,
                        cl_uint num_events_in_wait_list,
                        const cl_event* event_wait_list,
                        cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueUnmapMemObject"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueMigrateMemObjects(cl_command_queue command_queue,
                           cl_uint num_mem_objects,
                           const cl_mem* mem_objects,
                           cl_mem_migration_flags flags,
                           cl_uint num_events_in_wait_list,
                           const cl_event* event_wait_list,
                           cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueMigrateMemObjects"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clSetMemObjectDestructorCallback(cl_mem memobj,
                                 CLMemDestructorCallbackFunction pfn_notify,
                                 void* user_data) {
    if (!memobj) {
        RETURN_ERROR(CL_INVALID_MEM_OBJECT, "Memory object is null.")
    }

    if (!pfn_notify) {
        RETURN_ERROR(CL_INVALID_VALUE, "Callback is null.")
    }

    auto callback =
        std::make_shared<CLMemDestructorCallback>(pfn_notify, user_data);
    memobj->registerCallback(callback);

    return CL_SUCCESS;
}
