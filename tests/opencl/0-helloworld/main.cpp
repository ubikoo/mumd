/*
 * main.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include <fstream>
#include <chrono>

#include "../base.hpp"
using namespace atto;

/**
 * Constants
 */
static const size_t ARRAY_SIZE = 10000000;

/**
 *  Create an OpenCL context on the first available platform using
 *  either a GPU or CPU depending on what is available.
 */
cl_context CreateContext()
{
    cl_int errNum;
    cl_uint numPlatforms;
    cl_platform_id firstPlatformId;
    cl_context context = NULL;

    /*
     * First, select an OpenCL platform to run on.  For this example, we simply
     * choose the first available platform.  Normally, you would query for all
     * available platforms and select the most appropriate one.
     */
    errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
    if (errNum != CL_SUCCESS || numPlatforms <= 0)
    {
        std::cerr << "Failed to find any OpenCL platforms." << std::endl;
        return NULL;
    }

    /*
     * Next, create an OpenCL context on the platform.  Attempt to create a
     * GPU-based context, and if that fails, try to create a CPU-based context.
     */
    cl_context_properties contextProperties[] =
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)firstPlatformId,
        0
    };
    context = clCreateContextFromType(contextProperties,
        CL_DEVICE_TYPE_GPU, NULL, NULL, &errNum);
    if (errNum != CL_SUCCESS)
    {
        std::cout << "Could not create GPU context, trying CPU..." << std::endl;
        context = clCreateContextFromType(contextProperties,
            CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
        if (errNum != CL_SUCCESS)
        {
            std::cerr << "Failed to create an OpenCL GPU or CPU context."
                      << std::endl;
            return NULL;
        }
    }

    return context;
}

/**
 * Create a command queue on the first device available on the context.
 */
cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device)
{
    cl_int errNum;
    cl_device_id *devices;
    cl_command_queue commandQueue = NULL;
    size_t deviceBufferSize = -1;

    /* First get the size of the devices buffer */
    errNum = clGetContextInfo(
        context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
    if (errNum != CL_SUCCESS)
    {
        std::cerr << "Failed call to clGetContextInfo(GL_CONTEXT_DEVICES)";
        return NULL;
    }

    if (deviceBufferSize <= 0)
    {
        std::cerr << "No devices available.";
        return NULL;
    }

    /* Allocate memory for the devices buffer */
    devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
    errNum = clGetContextInfo(
        context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
    if (errNum != CL_SUCCESS)
    {
        delete [] devices;
        std::cerr << "Failed to get device IDs";
        return NULL;
    }

    for (size_t i = 0; i < deviceBufferSize / sizeof(cl_device_id); ++i) {
        cl_device_type param_device_type;
        errNum = clGetDeviceInfo(
            devices[i],
            CL_DEVICE_TYPE,
            sizeof(cl_device_type),
            &param_device_type,
            NULL);
        if (errNum != CL_SUCCESS)
        {
            delete [] devices;
            std::cerr << "Failed to get device IDs";
            return NULL;
        }

        std::cout << "device i: " << i << ", " <<
                    (
                      (param_device_type  == CL_DEVICE_TYPE_CPU) ? "CPU" :
                      (param_device_type  == CL_DEVICE_TYPE_GPU) ? "GPU" :
                      "UNKNOWN"
                    )
                  << "\n";
    }

    /*
     * In this example, we just choose the first available device.  In a
     * real program, you would likely use all available devices or choose
     * the highest performance device based on OpenCL device queries
     */
    commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
    if (commandQueue == NULL)
    {
        delete [] devices;
        std::cerr << "Failed to create commandQueue for device 0";
        return NULL;
    }

    *device = devices[0];
    delete [] devices;
    return commandQueue;
}

/**
 *  Create an OpenCL program from the kernel source file.
 */
cl_program CreateProgram(
    cl_context context,
    cl_device_id device,
    const char* fileName)
{
    cl_int errNum;
    cl_program program;

    std::ifstream kernelFile(fileName, std::ios::in);
    if (!kernelFile.is_open())
    {
        std::cerr << "Failed to open file for reading: "
                  << fileName
                  << std::endl;
        return NULL;
    }

    std::ostringstream oss;
    oss << kernelFile.rdbuf();

    std::string srcStdStr = oss.str();
    const char *srcStr = srcStdStr.c_str();
    program = clCreateProgramWithSource(
        context, 1, (const char**)&srcStr, NULL, NULL);
    if (program == NULL)
    {
        std::cerr << "Failed to create CL program from source." << std::endl;
        return NULL;
    }

    errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (errNum != CL_SUCCESS)
    {
        /* Determine the reason for the error */
        char buildLog[16384];
        clGetProgramBuildInfo(
            program,
            device,
            CL_PROGRAM_BUILD_LOG,
            sizeof(buildLog),
            buildLog,
            NULL);

        std::cerr << "Error in kernel: " << std::endl;
        std::cerr << buildLog;
        clReleaseProgram(program);
        return NULL;
    }

    return program;
}

/**
 * Create memory objects used as the arguments to the kernel. The kernel takes
 * three arguments: result (output), a (input), and b (input).
 */
bool CreateMemObjects(
    cl_context context,
    cl_mem memObjects[3],
    float *a,
    float *b)
{
    memObjects[0] = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(float) * ARRAY_SIZE,
        a,
        NULL);
    memObjects[1] = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(float) * ARRAY_SIZE,
        b,
        NULL);
    memObjects[2] = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        sizeof(float) * ARRAY_SIZE,
        NULL,
        NULL);

    if (memObjects[0] == NULL || memObjects[1] == NULL || memObjects[2] == NULL)
    {
        std::cerr << "Error creating memory objects." << std::endl;
        return false;
    }

    return true;
}

/**
*  Cleanup any created OpenCL resources
*/
void Cleanup(
    cl_context context,
    cl_command_queue commandQueue,
    cl_program program,
    cl_kernel kernel,
    cl_mem memObjects[3])
{
    for (int i = 0; i < 3; i++)
    {
        if (memObjects[i] != 0)
            clReleaseMemObject(memObjects[i]);
    }
    if (commandQueue != 0)
        clReleaseCommandQueue(commandQueue);

    if (kernel != 0)
        clReleaseKernel(kernel);

    if (program != 0)
        clReleaseProgram(program);

    if (context != 0)
        clReleaseContext(context);

}

/**
 * main
 */
int main(int argc, char const *argv[])
{
    cl_context context = 0;
    cl_command_queue commandQueue = 0;
    cl_program program = 0;
    cl_device_id device = 0;
    cl_kernel kernel = 0;
    cl_mem memObjects[3] = { 0, 0, 0 };
    cl_int errNum;

    /* Create an OpenCL context on first available platform */
    context = CreateContext();
    if (context == NULL)
    {
        std::cerr << "Failed to create OpenCL context." << std::endl;
        return 1;
    }

    /*
     * Create a command-queue on the first device available on the
     * created context
     */
    commandQueue = CreateCommandQueue(context, &device);
    if (commandQueue == NULL)
    {
        Cleanup(context, commandQueue, program, kernel, memObjects);
        return 1;
    }

    /* Create OpenCL program from helloworld.cl kernel source */
    program = CreateProgram(context, device, "helloworld.cl");
    if (program == NULL)
    {
        Cleanup(context, commandQueue, program, kernel, memObjects);
        return 1;
    }

    /* Create OpenCL kernel */
    kernel = clCreateKernel(program, "helloworld", NULL);
    if (kernel == NULL)
    {
        std::cerr << "Failed to create kernel" << std::endl;
        Cleanup(context, commandQueue, program, kernel, memObjects);
        return 1;
    }

    /*
     * Create memory objects that will be used as arguments to kernel. First
     * create host memory arrays that will be used to store the arguments to
     * the kernel
     */
    std::vector<float> result(ARRAY_SIZE, 0.0f);
    std::vector<float> a(ARRAY_SIZE, 0.0f);
    std::vector<float> b(ARRAY_SIZE, 0.0f);
    {
        /* Start time */
        auto tic = std::chrono::high_resolution_clock::now();

        /* Run */
        for (size_t i = 0; i < ARRAY_SIZE; i++)
        {
            a[i] = (float)i / (float) ARRAY_SIZE;
            b[i] = (float)(i * 2) / (float) ARRAY_SIZE;
        }

        /* End time */
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::printf("initialize %lf\n", msec.count());
    }
    if (!CreateMemObjects(context, memObjects, a.data(), b.data()))
    {
        Cleanup(context, commandQueue, program, kernel, memObjects);
        return 1;
    }

    /* Set the kernel arguments (result, a, b) */
    errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
    errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);
    errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObjects[2]);
    errNum |= clSetKernelArg(kernel, 3, sizeof(cl_ulong), &ARRAY_SIZE);
    if (errNum != CL_SUCCESS)
    {
        std::cerr << "Error setting kernel arguments." << std::endl;
        Cleanup(context, commandQueue, program, kernel, memObjects);
        return 1;
    }

    size_t globalWorkSize[1] = { ARRAY_SIZE };
    size_t localWorkSize[1] = { 1 };

    /* Queue the kernel up for execution across the array */
    for (size_t i = 0; i < 1; i++)
    {
        auto tic = std::chrono::high_resolution_clock::now();

        errNum = clEnqueueNDRangeKernel(
            commandQueue,
            kernel,
            1,
            NULL,
            globalWorkSize,
            localWorkSize,

            0,
            NULL,
            NULL);

        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::printf("elapsed time: %lf msec\n", msec.count());
    }
    if (errNum != CL_SUCCESS)
    {
        std::cerr << "Error queuing kernel for execution." << std::endl;
        Cleanup(context, commandQueue, program, kernel, memObjects);
        return 1;
    }

    /* Read the output buffer back to the Host */
    errNum = clEnqueueReadBuffer(
        commandQueue,               /* command queue */
        memObjects[2],              /* buffer */
        CL_TRUE,
        0,
        ARRAY_SIZE * sizeof(float),
        result.data(),
        0,
        NULL,
        NULL);
    if (errNum != CL_SUCCESS)
    {
        std::cerr << "Error reading result buffer." << std::endl;
        Cleanup(context, commandQueue, program, kernel, memObjects);
        return 1;
    }

    std::cout << "Executed program succesfully." << std::endl;
    Cleanup(context, commandQueue, program, kernel, memObjects);

    exit(EXIT_SUCCESS);
}
