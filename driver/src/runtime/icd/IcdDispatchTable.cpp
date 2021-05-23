#include <cassert>

#include "IcdDispatchTable.h"
#include "icd/icd.h"

#define ICD_DISPATCH_TABLE_ENTRY(fn)         \
    assert(dispatchTable->entryCount < 256); \
    dispatchTable->entries[dispatchTable->entryCount++] = (void*) (fn)

void initDispatchTable(IcdDispatchTable* dispatchTable);

IcdDispatchTable* IcdDispatchTableProvider::get() {
    if (mDispatchTable == nullptr) {
        mDispatchTable = new IcdDispatchTable();
        initDispatchTable(mDispatchTable);
    }

    return mDispatchTable;
}

void initDispatchTable(IcdDispatchTable* dispatchTable) {
    // OpenCL 1.0
    ICD_DISPATCH_TABLE_ENTRY(clGetPlatformIDs);
    ICD_DISPATCH_TABLE_ENTRY(clGetPlatformInfo);
    ICD_DISPATCH_TABLE_ENTRY(clGetDeviceIDs);
    ICD_DISPATCH_TABLE_ENTRY(clGetDeviceInfo);
    ICD_DISPATCH_TABLE_ENTRY(clCreateContext);
    ICD_DISPATCH_TABLE_ENTRY(clCreateContextFromType);
    ICD_DISPATCH_TABLE_ENTRY(clRetainContext);
    ICD_DISPATCH_TABLE_ENTRY(clReleaseContext);
    ICD_DISPATCH_TABLE_ENTRY(clGetContextInfo);
    ICD_DISPATCH_TABLE_ENTRY(clCreateCommandQueue);
    ICD_DISPATCH_TABLE_ENTRY(clRetainCommandQueue);
    ICD_DISPATCH_TABLE_ENTRY(clReleaseCommandQueue);
    ICD_DISPATCH_TABLE_ENTRY(clGetCommandQueueInfo);
    ICD_DISPATCH_TABLE_ENTRY(/* clSetCommandQueueProperty */ nullptr);
    ICD_DISPATCH_TABLE_ENTRY(clCreateBuffer);
    ICD_DISPATCH_TABLE_ENTRY(clCreateImage2D);
    ICD_DISPATCH_TABLE_ENTRY(clCreateImage3D);
    ICD_DISPATCH_TABLE_ENTRY(clRetainMemObject);
    ICD_DISPATCH_TABLE_ENTRY(clReleaseMemObject);
    ICD_DISPATCH_TABLE_ENTRY(clGetSupportedImageFormats);
    ICD_DISPATCH_TABLE_ENTRY(clGetMemObjectInfo);
    ICD_DISPATCH_TABLE_ENTRY(clGetImageInfo);
    ICD_DISPATCH_TABLE_ENTRY(clCreateSampler);
    ICD_DISPATCH_TABLE_ENTRY(clRetainSampler);
    ICD_DISPATCH_TABLE_ENTRY(clReleaseSampler);
    ICD_DISPATCH_TABLE_ENTRY(clGetSamplerInfo);
    ICD_DISPATCH_TABLE_ENTRY(clCreateProgramWithSource);
    ICD_DISPATCH_TABLE_ENTRY(clCreateProgramWithBinary);
    ICD_DISPATCH_TABLE_ENTRY(clRetainProgram);
    ICD_DISPATCH_TABLE_ENTRY(clReleaseProgram);
    ICD_DISPATCH_TABLE_ENTRY(clBuildProgram);
    ICD_DISPATCH_TABLE_ENTRY(clUnloadCompiler);
    ICD_DISPATCH_TABLE_ENTRY(clGetProgramInfo);
    ICD_DISPATCH_TABLE_ENTRY(clGetProgramBuildInfo);
    ICD_DISPATCH_TABLE_ENTRY(clCreateKernel);
    ICD_DISPATCH_TABLE_ENTRY(clCreateKernelsInProgram);
    ICD_DISPATCH_TABLE_ENTRY(clRetainKernel);
    ICD_DISPATCH_TABLE_ENTRY(clReleaseKernel);
    ICD_DISPATCH_TABLE_ENTRY(clSetKernelArg);
    ICD_DISPATCH_TABLE_ENTRY(clGetKernelInfo);
    ICD_DISPATCH_TABLE_ENTRY(clGetKernelWorkGroupInfo);
    ICD_DISPATCH_TABLE_ENTRY(clWaitForEvents);
    ICD_DISPATCH_TABLE_ENTRY(clGetEventInfo);
    ICD_DISPATCH_TABLE_ENTRY(clRetainEvent);
    ICD_DISPATCH_TABLE_ENTRY(clReleaseEvent);
    ICD_DISPATCH_TABLE_ENTRY(clGetEventProfilingInfo);
    ICD_DISPATCH_TABLE_ENTRY(clFlush);
    ICD_DISPATCH_TABLE_ENTRY(clFinish);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueReadBuffer);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueWriteBuffer);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueCopyBuffer);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueReadImage);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueWriteImage);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueCopyImage);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueCopyImageToBuffer);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueCopyBufferToImage);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueMapBuffer);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueMapImage);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueUnmapMemObject);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueNDRangeKernel);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueTask);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueNativeKernel);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueMarker);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueWaitForEvents);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueBarrier);
    ICD_DISPATCH_TABLE_ENTRY(clGetExtensionFunctionAddress);
    ICD_DISPATCH_TABLE_ENTRY(/*clCreateFromGLBuffer*/ nullptr);
    ICD_DISPATCH_TABLE_ENTRY(/*clCreateFromGLTexture2D*/ nullptr);
    ICD_DISPATCH_TABLE_ENTRY(/*clCreateFromGLTexture3D*/ nullptr);
    ICD_DISPATCH_TABLE_ENTRY(/*clCreateFromGLRenderbuffer*/ nullptr);
    ICD_DISPATCH_TABLE_ENTRY(/*clGetGLObjectInfo*/ nullptr);
    ICD_DISPATCH_TABLE_ENTRY(/*clGetGLTextureInfo*/ nullptr);
    ICD_DISPATCH_TABLE_ENTRY(/*clEnqueueAcquireGLObjects*/ nullptr);
    ICD_DISPATCH_TABLE_ENTRY(/*clEnqueueReleaseGLObjects*/ nullptr);

    // cl_khr_gl_sharing
    ICD_DISPATCH_TABLE_ENTRY(/* clGetGLContextInfoKHR */ nullptr);

    // cl_khr_d3d10_sharing (windows-only)
#if 0 && defined(_WIN32)
    ICD_DISPATCH_TABLE_ENTRY ( clGetDeviceIDsFromD3D10KHR      );
    ICD_DISPATCH_TABLE_ENTRY ( clCreateFromD3D10BufferKHR      );
    ICD_DISPATCH_TABLE_ENTRY ( clCreateFromD3D10Texture2DKHR   );
    ICD_DISPATCH_TABLE_ENTRY ( clCreateFromD3D10Texture3DKHR   );
    ICD_DISPATCH_TABLE_ENTRY ( clEnqueueAcquireD3D10ObjectsKHR );
    ICD_DISPATCH_TABLE_ENTRY ( clEnqueueReleaseD3D10ObjectsKHR );
#else
    ICD_DISPATCH_TABLE_ENTRY(nullptr);
    ICD_DISPATCH_TABLE_ENTRY(nullptr);
    ICD_DISPATCH_TABLE_ENTRY(nullptr);
    ICD_DISPATCH_TABLE_ENTRY(nullptr);
    ICD_DISPATCH_TABLE_ENTRY(nullptr);
    ICD_DISPATCH_TABLE_ENTRY(nullptr);
#endif

    // OpenCL 1.1
    ICD_DISPATCH_TABLE_ENTRY(clSetEventCallback);
    ICD_DISPATCH_TABLE_ENTRY(clCreateSubBuffer);
    ICD_DISPATCH_TABLE_ENTRY(clSetMemObjectDestructorCallback);
    ICD_DISPATCH_TABLE_ENTRY(clCreateUserEvent);
    ICD_DISPATCH_TABLE_ENTRY(clSetUserEventStatus);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueReadBufferRect);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueWriteBufferRect);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueCopyBufferRect);

    /* cl_ext_device_fission */
    ICD_DISPATCH_TABLE_ENTRY(/*clCreateSubDevicesEXT*/ nullptr);
    ICD_DISPATCH_TABLE_ENTRY(/*clRetainDeviceEXT*/ nullptr);
    ICD_DISPATCH_TABLE_ENTRY(/*clReleaseDevice*/ nullptr);

    /* cl_khr_gl_event */
    ICD_DISPATCH_TABLE_ENTRY(/* clCreateEventFromGLsyncKHR */ nullptr);

    /* OpenCL 1.2 */
    ICD_DISPATCH_TABLE_ENTRY(clCreateSubDevices);
    ICD_DISPATCH_TABLE_ENTRY(clRetainDevice);
    ICD_DISPATCH_TABLE_ENTRY(clReleaseDevice);
    ICD_DISPATCH_TABLE_ENTRY(clCreateImage);
    ICD_DISPATCH_TABLE_ENTRY(clCreateProgramWithBuiltInKernels);
    ICD_DISPATCH_TABLE_ENTRY(clCompileProgram);
    ICD_DISPATCH_TABLE_ENTRY(clLinkProgram);
    ICD_DISPATCH_TABLE_ENTRY(clUnloadPlatformCompiler);
    ICD_DISPATCH_TABLE_ENTRY(clGetKernelArgInfo);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueFillBuffer);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueFillImage);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueMigrateMemObjects);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueMarkerWithWaitList);
    ICD_DISPATCH_TABLE_ENTRY(clEnqueueBarrierWithWaitList);
    ICD_DISPATCH_TABLE_ENTRY(clGetExtensionFunctionAddressForPlatform);
    ICD_DISPATCH_TABLE_ENTRY(/* clCreateFromGLTexture */ nullptr);
}
