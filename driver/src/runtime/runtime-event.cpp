#include <iostream>
#include "icd/icd.h"

CL_API_ENTRY cl_int CL_API_CALL clWaitForEvents(cl_uint num_events,
                                                const cl_event* event_list) {
    // TODO(clWaitForEvents): flush queue
    std::cerr << "Unimplemented OpenCL API call: clWaitForEvents" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clGetEventInfo(cl_event event,
                                               cl_event_info param_name,
                                               size_t param_value_size,
                                               void* param_value,
                                               size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call: clGetEventInfo" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_event CL_API_CALL clCreateUserEvent(cl_context context,
                                                    cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateUserEvent"
              << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainEvent(cl_event event) {
    std::cerr << "Unimplemented OpenCL API call: clRetainEvent" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseEvent(cl_event event) {
    std::cerr << "Unimplemented OpenCL API call: clReleaseEvent" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clSetUserEventStatus(cl_event event,
                                                     cl_int execution_status) {
    std::cerr << "Unimplemented OpenCL API call: clSetUserEventStatus"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clSetEventCallback(cl_event event,
                   cl_int command_exec_callback_type,
                   void(CL_CALLBACK* pfn_notify)(cl_event event,
                                                 cl_int event_command_status,
                                                 void* user_data),
                   void* user_data) {
    std::cerr << "Unimplemented OpenCL API call: clSetEventCallback"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetEventProfilingInfo(cl_event event,
                        cl_profiling_info param_name,
                        size_t param_value_size,
                        void* param_value,
                        size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call: clGetEventProfilingInfo"
              << std::endl;
    return CL_INVALID_PLATFORM;
}
