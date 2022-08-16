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

#include <vector>
#include <chrono>
#include <numeric>

#include "../base.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * Program vecsum kernel source.
 */
const std::string vecsum_source = core_strify(
__kernel void vecsum(
    const unsigned long array_size,
    __global const float *a,
    __global const float *b,
    __global float *result)
{
    const size_t ix = get_global_id(0);
    if (ix < array_size) {
       result[ix] = a[ix] + b[ix];
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
    cl::Program::build(program, device, "");

    /* Create the OpenCL kernel. */
    kernel = cl::Kernel::create(program, "vecsum");
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
     * Create memory objects for the kernel arguments. First create host memory
     * arrays for the kernel args, then create the memory objects on the device.
     */
    size_t array_size = 1000000;
    std::vector<float> result(array_size, 0.0f);
    std::vector<float> a(array_size, 0.0f);
    std::vector<float> b(array_size, 0.0f);

    std::iota(a.begin(), a.end(), 0);
    std::iota(b.begin(), b.end(), 0);

    buffers.emplace_back(cl::Memory::create_buffer(
        context,
        CL_MEM_READ_ONLY,
        array_size * sizeof(float),
        (void *) NULL));

    buffers.emplace_back(cl::Memory::create_buffer(
        context,
        CL_MEM_READ_ONLY,
        array_size * sizeof(float),
        (void *) NULL));

    buffers.emplace_back(cl::Memory::create_buffer(
        context,
        CL_MEM_READ_WRITE,
        array_size * sizeof(float),
        (void *) NULL));

    /*
     * Set the kernel arguments (result, a, b).
     */
    cl::Kernel::set_arg(kernel, 0, sizeof(cl_ulong), &array_size);
    cl::Kernel::set_arg(kernel, 1, sizeof(cl_mem), &buffers[0]);
    cl::Kernel::set_arg(kernel, 2, sizeof(cl_mem), &buffers[1]);
    cl::Kernel::set_arg(kernel, 3, sizeof(cl_mem), &buffers[2]);

    /*
     * Update the gpu buffer store.
     */
    cl::Queue::enqueue_write_buffer(
        queue,
        buffers[0],
        CL_TRUE,
        0,
        array_size * sizeof(float),
        (void *) a.data(),
        NULL,
        NULL);

    cl::Queue::enqueue_write_buffer(
        queue,
        buffers[1],
        CL_TRUE,
        0,
        array_size * sizeof(float),
        (void *) b.data(),
        NULL,
        NULL);

    /*
     * Queue the kernel up for execution
     */
    {
        /* Start time */
        auto tic = std::chrono::high_resolution_clock::now();

        /* Set the size of the NDRange workgroups */
        cl::NDRange local_ws(Params::work_group_size_1d);
        cl::NDRange global_ws(cl::NDRange::Roundup(
            array_size,
            Params::work_group_size_1d));

        /* Run the kernel */
        cl::Queue::enqueue_nd_range_kernel(
            queue,
            kernel,
            cl::NDRange::Null,
            global_ws,
            local_ws,
            NULL,
            NULL);

        /* End time */
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::printf("elapsed time %lf\n", msec.count());
    }

    /*
     * Read the output buffer back to the host.
     */
    cl::Queue::enqueue_read_buffer(
        queue,
        buffers[2],
        CL_TRUE,
        0,
        array_size * sizeof(float),
        (void *) result.data(),
        NULL,   /* event_wait_list */
        NULL);  /* event */

    /*
     * Teardown OpenCL data.
     */
    Teardown(context, device, queue, program, kernel, buffers, images);

    exit(EXIT_SUCCESS);
}