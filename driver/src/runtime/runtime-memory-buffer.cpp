#include <command/Command.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>

#include "icd/CLCommandQueue.h"
#include "icd/CLContext.h"
#include "icd/CLDeviceId.hpp"
#include "runtime-commons.h"

CL_API_ENTRY cl_mem CL_API_CALL clCreateBuffer(cl_context context,
                                               cl_mem_flags flags,
                                               size_t size,
                                               void* host_ptr,
                                               cl_int* errcode_ret) {
    if (!context) {
        SET_ERROR_AND_RETURN(CL_INVALID_CONTEXT, "Context is null.");
    }

    if (size == 0) {
        SET_ERROR_AND_RETURN(CL_INVALID_BUFFER_SIZE, "Buffer size is 0.");
    }

    if (!host_ptr &&
        ((flags & CL_MEM_COPY_HOST_PTR) || (flags & CL_MEM_USE_HOST_PTR))) {
        SET_ERROR_AND_RETURN(
            CL_INVALID_HOST_PTR,
            "host_ptr is null but either CL_MEM_COPY_HOST_PTR or "
            "CL_MEM_USE_HOST_PTR is specified.");
    }

    if (host_ptr &&
        !((flags & CL_MEM_COPY_HOST_PTR) || (flags & CL_MEM_USE_HOST_PTR))) {
        SET_ERROR_AND_RETURN(
            CL_INVALID_HOST_PTR,
            "host_ptr is not null but neither CL_MEM_COPY_HOST_PTR nor "
            "CL_MEM_USE_HOST_PTR are present.");
    }

    if (utils::hasMutuallyExclusiveFlags(
            flags, {CL_MEM_READ_WRITE, CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY})) {
        SET_ERROR_AND_RETURN(
            CL_INVALID_VALUE,
            "CL_MEM_READ_WRITE, CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY are "
            "mutually exclusive.");
    }

    if (utils::hasMutuallyExclusiveFlags(
            flags, {CL_MEM_HOST_WRITE_ONLY, CL_MEM_HOST_READ_ONLY,
                    CL_MEM_HOST_NO_ACCESS})) {
        SET_ERROR_AND_RETURN(CL_INVALID_VALUE,
                             "CL_MEM_HOST_WRITE_ONLY, CL_MEM_HOST_READ_ONLY, "
                             "CL_MEM_HOST_NO_ACCESS are mutually exclusive.");
    }

    if (utils::hasMutuallyExclusiveFlags(
            flags, {CL_MEM_USE_HOST_PTR, CL_MEM_ALLOC_HOST_PTR})) {
        SET_ERROR_AND_RETURN(
            CL_INVALID_VALUE,
            "CL_MEM_USE_HOST_PTR, CL_MEM_ALLOC_HOST_PTR are mutually "
            "exclusive.");
    }

    if (utils::hasMutuallyExclusiveFlags(
            flags, {CL_MEM_USE_HOST_PTR, CL_MEM_COPY_HOST_PTR})) {
        SET_ERROR_AND_RETURN(
            CL_INVALID_VALUE,
            "CL_MEM_USE_HOST_PTR, CL_MEM_COPY_HOST_PTR are mutually "
            "exclusive.");
    }

    if (context->device->usedGlobalMemory + size >
        context->device->globalMemorySize) {
        // TODO: lookup error code
        SET_ERROR_AND_RETURN(
            CL_MEM_OBJECT_ALLOCATION_FAILURE,
            "Cannot allocate " + std::to_string(size) +
                " bytes of data. Used: " +
                std::to_string(context->device->usedGlobalMemory) + " / " +
                std::to_string(context->device->globalMemorySize) + " bytes.");
    }

    auto mem = new CLMem(kDispatchTable, context);

    bool kernelRWAccess = (!flags) || (flags & CL_MEM_READ_WRITE);
    mem->kernelCanRead = kernelRWAccess || (flags & CL_MEM_READ_ONLY);
    mem->kernelCanWrite = kernelRWAccess || (flags & CL_MEM_WRITE_ONLY);

    bool hostRWAccess =
        (flags & (CL_MEM_HOST_WRITE_ONLY | CL_MEM_HOST_READ_ONLY |
                  CL_MEM_HOST_NO_ACCESS)) == 0;
    mem->hostCanRead = hostRWAccess || (flags & CL_MEM_HOST_READ_ONLY);
    mem->hostCanWrite = hostRWAccess || (flags & CL_MEM_HOST_WRITE_ONLY);

    mem->size = size;

    if (flags & CL_MEM_USE_HOST_PTR) {
        mem->address = static_cast<std::byte*>(host_ptr);

    } else {
        if (flags & CL_MEM_ALLOC_HOST_PTR) {
            // TODO: support CL_MEM_ALLOC_HOST_PTR
            kLogger.warn(
                "CL_MEM_ALLOC_HOST_PTR is not supported,"
                "will act like flag was not specified!");
        }

        mem->address = new std::byte[mem->size];

        if (flags & CL_MEM_COPY_HOST_PTR) {
            memcpy(mem->address, host_ptr, mem->size);
        } else {
            memset(mem->address, 0, mem->size);
        }
    }

    if (!mem->address) {
        delete mem;
        SET_ERROR_AND_RETURN(CL_MEM_OBJECT_ALLOCATION_FAILURE,
                             "Failed to allocate buffer memory.");
    }

    context->device->usedGlobalMemory += mem->size;

    SET_SUCCESS();

    return mem;
}

CL_API_ENTRY cl_mem CL_API_CALL
clCreateSubBuffer(cl_mem buffer,
                  cl_mem_flags flags,
                  cl_buffer_create_type buffer_create_type,
                  const void* buffer_create_info,
                  cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateSubBuffer"
              << std::endl;
    return nullptr;
}

#define CHECK_BUFFER_PARAMETERS()                                        \
    do {                                                                 \
        if (!ptr || !size) {                                             \
            RETURN_ERROR(CL_INVALID_VALUE, "ptr or size is null.");       \
        }                                                                \
                                                                         \
        if (offset + size > buffer->size) {                              \
            RETURN_ERROR(CL_INVALID_VALUE,                               \
                         "Requested size of " + std::to_string(size) +   \
                             " bytes with offset of " +                  \
                             std::to_string(offset) + " bytes (total " + \
                             std::to_string(size + offset) +             \
                             " bytes) is more than buffer size of " +    \
                             std::to_string(buffer->size) + " bytes.");   \
        }                                                                \
    } while (0)

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
    CHECK_BUFFER_PARAMETERS();

    if (!buffer->hostCanRead) {
        RETURN_ERROR(CL_INVALID_OPERATION,
                     "clEnqueueReadBuffer on write-only buffer.");
    }

    const auto command =
        std::make_shared<BufferReadCommand>(buffer, size, offset, ptr);

    command_queue->enqueue(command);

    if (blocking_read) {
        clFlush(command_queue);
    }

    return CL_SUCCESS;
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
    CHECK_BUFFER_PARAMETERS();

    if (!buffer->hostCanWrite) {
        RETURN_ERROR(CL_INVALID_OPERATION,
                     "clEnqueueWriteBuffer on read-only buffer.");
    }

    const auto command =
        std::make_shared<BufferWriteCommand>(buffer, size, offset, ptr);

    command_queue->enqueue(command);

    if (blocking_write) {
        clFlush(command_queue);
    }

    return CL_SUCCESS;
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
    if (blocking_read) {
        clFlush(command_queue);
    }

    std::cerr << "Unimplemented OpenCL API call: clEnqueueReadBufferRect"
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
    if (blocking_write) {
        clFlush(command_queue);
    }

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
    if (blocking_map) {
        clFlush(command_queue);
    }

    std::cerr << "Unimplemented OpenCL API call: clEnqueueMapBuffer"
              << std::endl;
    return nullptr;
}
