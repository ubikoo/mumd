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

#include "../base.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * Program vecsum kernel source.
 */
const std::string vecsum_source = core_strify(
__kernel void vecsum(
    __global const float *a,
    __global const float *b,
    __global float *result,
    const unsigned long count)
{
    size_t idx = get_global_id(0);
    if (idx < count) {
       result[idx] = a[idx] + b[idx];
    }
});

/** ---------------------------------------------------------------------------
 * Setup OpenCL context.
 */
void Setup(
    cl_context &context,
    cl_device_id &device,
    cl_command_queue &queue,
    cl_program &program,
    cl_kernel &kernel,
    std::vector<cl_mem> &buffers,
    std::vector<cl_mem> &images)
{
    /* Setup OpenCL context on the first available platform. */
    context = cl::Context::create(CL_DEVICE_TYPE_GPU);
    device = cl::Context::get_device(context, Params::device_index);
    std::cout << cl::Device::get_info_string(device) << "\n";

    /* Create a command queue on the specified device. */
    queue = cl::Queue::create(context, device);

    /* Create a OpenCL program for the kernel source. */
    program = cl::Program::create_from_source(context, vecsum_source);
}

/** ---------------------------------------------------------------------------
 * Teardown OpenCL data.
 */
void Teardown(
    cl_context &context,
    cl_device_id &device,
    cl_command_queue &queue,
    cl_program &program,
    cl_kernel &kernel,
    std::vector<cl_mem> &buffers,
    std::vector<cl_mem> &images)
{
    for (auto &it : images) {
        cl::Memory::release(it);
    }
    for (auto &it : buffers) {
        cl::Memory::release(it);
    }
    cl::Kernel::release(kernel);
    cl::Program::release(program);
    cl::Queue::release(queue);
    cl::Device::release(device);
    cl::Context::release(context);
}

/** ---------------------------------------------------------------------------
 * main
 */
int main(int argc, char const *argv[])
{
    cl_context context = NULL;
    cl_device_id device = NULL;
    cl_command_queue queue = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    std::vector<cl_mem> buffers;
    std::vector<cl_mem> images;

    /*
     * Setup OpenCL context.
     */
    Setup(context, device, queue, program, kernel, buffers, images);

    /*
     * Create memory objects for the kernel arguments. First create the host
     * memory arrays for the kernel args. Then create the memory objects on
     * the device.
     */
    const size_t array_size = 1000000;
    std::vector<float> result(array_size, 0.0f);
    std::vector<float> a(array_size, 0.0f);
    std::vector<float> b(array_size, 0.0f);

    buffers.emplace_back(cl::Memory::create_buffer(
        context,
        CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(float) * array_size,
        (void *) a.data()));

    buffers.emplace_back(cl::Memory::create_buffer(
        context,
        CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(float) * array_size,
        (void *) b.data()));

    buffers.emplace_back(cl::Memory::create_buffer(
        context,
        CL_MEM_READ_WRITE,
        sizeof(float) * array_size,
        (void *) NULL));

    /*
     * Teardown OpenCL data.
     */
    Teardown(context, device, queue, program, kernel, buffers, images);

    exit(EXIT_SUCCESS);
}