#include <iostream>
#include "icd.h"

CL_API_ENTRY cl_mem CL_API_CALL
clCreateImage(cl_context context,
              cl_mem_flags flags,
              const cl_image_format* image_format,
              const cl_image_desc* image_desc,
              void* host_ptr,
              cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateImage" << std::endl;
    return nullptr;
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
clGetSupportedImageFormats(cl_context context,
                           cl_mem_flags flags,
                           cl_mem_object_type image_type,
                           cl_uint num_entries,
                           cl_image_format* image_formats,
                           cl_uint* num_image_formats) {
    std::cerr << "Unimplemented OpenCL API call: clGetSupportedImageFormats"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clGetImageInfo(cl_mem image,
                                               cl_image_info param_name,
                                               size_t param_value_size,
                                               void* param_value,
                                               size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call: clGetImageInfo" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_mem CL_API_CALL
clCreateImage2D(cl_context context,
                cl_mem_flags flags,
                const cl_image_format* image_format,
                size_t image_width,
                size_t image_height,
                size_t image_row_pitch,
                void* host_ptr,
                cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateImage2D" << std::endl;
    return nullptr;
}

CL_API_ENTRY
CL_API_ENTRY cl_mem CL_API_CALL
clCreateImage3D(cl_context context,
                cl_mem_flags flags,
                const cl_image_format* image_format,
                size_t image_width,
                size_t image_height,
                size_t image_depth,
                size_t image_row_pitch,
                size_t image_slice_pitch,
                void* host_ptr,
                cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateImage3D" << std::endl;
    return nullptr;
}
