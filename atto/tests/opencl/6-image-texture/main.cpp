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
    program = cl::Program::create_from_file(context, "image_rotate.cl");
    cl::Program::build(program, device, "");

    /* Create the OpenCL kernel. */
    kernel = cl::Kernel::create(program, "image_rotate");
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
     * Load image data and copy to the buffer
     */
    const std::string filename("../data/monarch_512.png");
    const uint32_t n_channels = 4;
    gl::Image image(filename, false, n_channels);
    std::cout << image.infolog() << "\n";

    /*
     * Create the gpu image data store.
     */
    {
        cl_image_format image_format{CL_RGBA, CL_UNSIGNED_INT8};

        images.emplace_back(cl::Memory::create_image2d(
            context,
            CL_MEM_READ_ONLY,
            image_format,
            image.width(),
            image.height(),
            image.pitch(),
            NULL));

        images.emplace_back(cl::Memory::create_image2d(
            context,
            CL_MEM_WRITE_ONLY,
            image_format,
            image.width(),
            image.height(),
            image.pitch(),
            NULL));
    }

    /*
     * Update the gpu buffer store.
     */
    {
        const std::array<size_t,3> origin{0, 0, 0};
        const std::array<size_t,3> region{image.width(), image.height(), 1};

        cl::Queue::enqueue_write_image(
            queue,
            images[0],
            CL_TRUE,
            origin,
            region,
            image.pitch(),
            0,
            (void *) image.bitmap(),
            NULL,
            NULL);
    }

    /*
     * Queue the kernel up for execution
     */
    float theta = (float) (0.5*M_PI);
    float cos_theta = std::cos(theta);
    float sin_theta = std::sin(theta);
    cl_sampler sampler = cl::Sampler::create(
        context,
        CL_FALSE,
        CL_ADDRESS_REPEAT,
        CL_FILTER_LINEAR);

    const size_t max_iters = 180;
    for (size_t iter = 0; iter < max_iters; ++iter) {
       /*
        * Generate new data.
        */
        {
            size_t width = image.width();
            size_t height = image.height();
            theta = (float) M_PI * iter / max_iters;
            cos_theta = std::cos(theta);
            sin_theta = std::sin(theta);
            std::cout << "\ntheta "
                      << theta << " "
                      << cos_theta << " "
                      << sin_theta << "\n";

            cl::Kernel::set_arg(kernel, 0, sizeof(cl_mem),      &images[0]);
            cl::Kernel::set_arg(kernel, 1, sizeof(cl_mem),      &images[1]);
            cl::Kernel::set_arg(kernel, 2, sizeof(cl_sampler),  &sampler);
            cl::Kernel::set_arg(kernel, 3, sizeof(cl_long),     &width);
            cl::Kernel::set_arg(kernel, 4, sizeof(cl_long),     &height);
            cl::Kernel::set_arg(kernel, 5, sizeof(cl_float),    &cos_theta);
            cl::Kernel::set_arg(kernel, 6, sizeof(cl_float),    &sin_theta);
        }

       /*
        * Run the kernel.
        */
        {
            /* Start time */
            auto tic = std::chrono::high_resolution_clock::now();

            /* Set the size of the NDRange workgroups */
            cl::NDRange local_ws(
                Params::work_group_size_2d,
                Params::work_group_size_2d);
            cl::NDRange global_ws(
                cl::NDRange::Roundup(image.width(),  local_ws(0)),
                cl::NDRange::Roundup(image.height(), local_ws(1)));

            std::cout << "local_ws "
                      << local_ws(0) << " "
                      << local_ws(1) << " "
                      << local_ws(2) << "\n";
            std::cout << "global_ws "
                      << global_ws(0) << " "
                      << global_ws(1) << " "
                      << global_ws(2) << "\n";

            /* Run the kernel */
            cl::Queue::enqueue_nd_range_kernel(
                queue,
                kernel,
                cl::NDRange::Null,
                global_ws,
                local_ws,
                NULL,
                NULL);

            /* Wait for kernel to compute */
            cl::Queue::finish(queue);

            /* End time */
            auto toc = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
            std::printf("iter %lu\n", iter);
            std::printf("elapsed: %lf msec\n", msec.count());
        }

        /*
         * Read the output buffer back to the host.
         */
        {
            const std::array<size_t,3> origin{0, 0, 0};
            const std::array<size_t,3> region{image.width(), image.height(), 1};

            cl::Queue::enqueue_read_image(
                queue,
                images[1],
                CL_TRUE,
                origin,
                region,
                image.pitch(),
                0,
                (void *) image.bitmap(),
                NULL,
                NULL);

            image.write_png(core::str_format("/tmp/out_%04d.png", iter));
        }
    }

    /*
     * Teardown OpenCL data.
     */
    Teardown(context, device, queue, program, kernel, buffers, images);

    exit(EXIT_SUCCESS);
}
