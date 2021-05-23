#include <iostream>

#include "icd/icd.h"




CL_API_ENTRY cl_int CL_API_CALL
clEnqueueMarkerWithWaitList(cl_command_queue command_queue,
                            cl_uint num_events_in_wait_list,
                            const cl_event* event_wait_list,
                            cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueMarkerWithWaitList"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueBarrierWithWaitList(cl_command_queue command_queue,
                             cl_uint num_events_in_wait_list,
                             const cl_event* event_wait_list,
                             cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueBarrierWithWaitList"
              << std::endl;
    return CL_INVALID_PLATFORM;
}


CL_API_ENTRY cl_int CL_API_CALL
clEnqueueMarker(cl_command_queue command_queue, cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueMarker" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueWaitForEvents(cl_command_queue command_queue,
                       cl_uint num_events,
                       const cl_event* event_list) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueWaitForEvents"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueBarrier(cl_command_queue command_queue) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueBarrier" << std::endl;
    return CL_INVALID_PLATFORM;
}
