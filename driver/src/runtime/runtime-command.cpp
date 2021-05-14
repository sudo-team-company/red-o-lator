#include <iostream>
#include "icd.h"

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueReadBuffer(cl_command_queue command_queue,
                    cl_mem buffer,
                    cl_bool blocking_read,
                    size_t offset,
                    size_t size,
                    void* ptr,
                    cl_uint num_events_in_wait_list,
                    const cl_event* event_wait_list,
                    cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueReadBuffer"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueReadBufferRect(cl_command_queue command_queue,
                        cl_mem buffer,
                        cl_bool blocking_read,
                        const size_t* buffer_origin,
                        const size_t* host_origin,
                        const size_t* region,
                        size_t buffer_row_pitch,
                        size_t buffer_slice_pitch,
                        size_t host_row_pitch,
                        size_t host_slice_pitch,
                        void* ptr,
                        cl_uint num_events_in_wait_list,
                        const cl_event* event_wait_list,
                        cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueReadBufferRect"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueWriteBuffer(cl_command_queue command_queue,
                     cl_mem buffer,
                     cl_bool blocking_write,
                     size_t offset,
                     size_t size,
                     const void* ptr,
                     cl_uint num_events_in_wait_list,
                     const cl_event* event_wait_list,
                     cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueWriteBuffer"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueWriteBufferRect(cl_command_queue command_queue,
                         cl_mem buffer,
                         cl_bool blocking_write,
                         const size_t* buffer_origin,
                         const size_t* host_origin,
                         const size_t* region,
                         size_t buffer_row_pitch,
                         size_t buffer_slice_pitch,
                         size_t host_row_pitch,
                         size_t host_slice_pitch,
                         const void* ptr,
                         cl_uint num_events_in_wait_list,
                         const cl_event* event_wait_list,
                         cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueWriteBufferRect"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueFillBuffer(cl_command_queue command_queue,
                    cl_mem buffer,
                    const void* pattern,
                    size_t pattern_size,
                    size_t offset,
                    size_t size,
                    cl_uint num_events_in_wait_list,
                    const cl_event* event_wait_list,
                    cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueFillBuffer"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyBuffer(cl_command_queue command_queue,
                    cl_mem src_buffer,
                    cl_mem dst_buffer,
                    size_t src_offset,
                    size_t dst_offset,
                    size_t size,
                    cl_uint num_events_in_wait_list,
                    const cl_event* event_wait_list,
                    cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueCopyBuffer"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyBufferRect(cl_command_queue command_queue,
                        cl_mem src_buffer,
                        cl_mem dst_buffer,
                        const size_t* src_origin,
                        const size_t* dst_origin,
                        const size_t* region,
                        size_t src_row_pitch,
                        size_t src_slice_pitch,
                        size_t dst_row_pitch,
                        size_t dst_slice_pitch,
                        cl_uint num_events_in_wait_list,
                        const cl_event* event_wait_list,
                        cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueCopyBufferRect"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueReadImage(cl_command_queue command_queue,
                   cl_mem image,
                   cl_bool blocking_read,
                   const size_t* origin,
                   const size_t* region,
                   size_t row_pitch,
                   size_t slice_pitch,
                   void* ptr,
                   cl_uint num_events_in_wait_list,
                   const cl_event* event_wait_list,
                   cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueReadImage"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueWriteImage(cl_command_queue command_queue,
                    cl_mem image,
                    cl_bool blocking_write,
                    const size_t* origin,
                    const size_t* region,
                    size_t input_row_pitch,
                    size_t input_slice_pitch,
                    const void* ptr,
                    cl_uint num_events_in_wait_list,
                    const cl_event* event_wait_list,
                    cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueWriteImage"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueFillImage(cl_command_queue command_queue,
                   cl_mem image,
                   const void* fill_color,
                   const size_t* origin,
                   const size_t* region,
                   cl_uint num_events_in_wait_list,
                   const cl_event* event_wait_list,
                   cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueFillImage"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyImage(cl_command_queue command_queue,
                   cl_mem src_image,
                   cl_mem dst_image,
                   const size_t* src_origin,
                   const size_t* dst_origin,
                   const size_t* region,
                   cl_uint num_events_in_wait_list,
                   const cl_event* event_wait_list,
                   cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueCopyImage"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyImageToBuffer(cl_command_queue command_queue,
                           cl_mem src_image,
                           cl_mem dst_buffer,
                           const size_t* src_origin,
                           const size_t* region,
                           size_t dst_offset,
                           cl_uint num_events_in_wait_list,
                           const cl_event* event_wait_list,
                           cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueCopyImageToBuffer"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyBufferToImage(cl_command_queue command_queue,
                           cl_mem src_buffer,
                           cl_mem dst_image,
                           size_t src_offset,
                           const size_t* dst_origin,
                           const size_t* region,
                           cl_uint num_events_in_wait_list,
                           const cl_event* event_wait_list,
                           cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueCopyBufferToImage"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY void* CL_API_CALL
clEnqueueMapBuffer(cl_command_queue command_queue,
                   cl_mem buffer,
                   cl_bool blocking_map,
                   cl_map_flags map_flags,
                   size_t offset,
                   size_t size,
                   cl_uint num_events_in_wait_list,
                   const cl_event* event_wait_list,
                   cl_event* event,
                   cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueMapBuffer"
              << std::endl;
    return nullptr;
}

CL_API_ENTRY void* CL_API_CALL
clEnqueueMapImage(cl_command_queue command_queue,
                  cl_mem image,
                  cl_bool blocking_map,
                  cl_map_flags map_flags,
                  const size_t* origin,
                  const size_t* region,
                  size_t* image_row_pitch,
                  size_t* image_slice_pitch,
                  cl_uint num_events_in_wait_list,
                  const cl_event* event_wait_list,
                  cl_event* event,
                  cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueMapImage"
              << std::endl;
    return nullptr;
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
clEnqueueNDRangeKernel(cl_command_queue command_queue,
                       cl_kernel kernel,
                       cl_uint work_dim,
                       const size_t* global_work_offset,
                       const size_t* global_work_size,
                       const size_t* local_work_size,
                       cl_uint num_events_in_wait_list,
                       const cl_event* event_wait_list,
                       cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueNDRangeKernel"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueNativeKernel(cl_command_queue command_queue,
                      void (*user_func)(void*),
                      void* args,
                      size_t cb_args,
                      cl_uint num_mem_objects,
                      const cl_mem* mem_list,
                      const void** args_mem_loc,
                      cl_uint num_events_in_wait_list,
                      const cl_event* event_wait_list,
                      cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueNativeKernel"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

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

CL_API_ENTRY CL_API_PREFIX__VERSION_1_2_DEPRECATED cl_int CL_API_CALL
clEnqueueTask(cl_command_queue command_queue,
              cl_kernel kernel,
              cl_uint num_events_in_wait_list,
              const cl_event* event_wait_list,
              cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueTask" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY CL_API_SUFFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
clEnqueueMarker(cl_command_queue command_queue, cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueMarker" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY CL_API_SUFFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
clEnqueueWaitForEvents(cl_command_queue command_queue,
                       cl_uint num_events,
                       const cl_event* event_list) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueWaitForEvents"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY CL_API_SUFFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
clEnqueueBarrier(cl_command_queue command_queue) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueBarrier" << std::endl;
    return CL_INVALID_PLATFORM;
}
