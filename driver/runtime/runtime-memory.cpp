#include <opencl.h>
#include <iostream>

CL_API_ENTRY cl_mem CL_API_CALL clCreateBuffer(cl_context context,
                                               cl_mem_flags flags,
                                               size_t size,
                                               void* host_ptr,
                                               cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_mem CL_API_CALL
clCreateSubBuffer(cl_mem buffer,
                  cl_mem_flags flags,
                  cl_buffer_create_type buffer_create_type,
                  const void* buffer_create_info,
                  cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_mem CL_API_CALL
clCreateImage(cl_context context,
              cl_mem_flags flags,
              const cl_image_format* image_format,
              const cl_image_desc* image_desc,
              void* host_ptr,
              cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainMemObject(cl_mem memobj) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseMemObject(cl_mem memobj) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetSupportedImageFormats(cl_context context,
                           cl_mem_flags flags,
                           cl_mem_object_type image_type,
                           cl_uint num_entries,
                           cl_image_format* image_formats,
                           cl_uint* num_image_formats) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetMemObjectInfo(cl_mem memobj,
                   cl_mem_info param_name,
                   size_t param_value_size,
                   void* param_value,
                   size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clGetImageInfo(cl_mem image,
                                               cl_image_info param_name,
                                               size_t param_value_size,
                                               void* param_value,
                                               size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clSetMemObjectDestructorCallback(
    cl_mem memobj, void (*pfn_notify)(cl_mem, void*), void* user_data) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY CL_API_SUFFIX__VERSION_1_1_DEPRECATED cl_mem CL_API_CALL
clCreateImage2D(cl_context context,
                cl_mem_flags flags,
                const cl_image_format* image_format,
                size_t image_width,
                size_t image_height,
                size_t image_row_pitch,
                void* host_ptr,
                cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return nullptr;
}

CL_API_ENTRY
CL_API_PREFIX__VERSION_1_1_DEPRECATED
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
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return nullptr;
}
