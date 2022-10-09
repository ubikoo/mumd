/*
 * model.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "model.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * Model::Model
 * @brief Create OpenCL context and associated objects.
 */
Model::Model(const int proc_id, const int n_procs)
{
    /*
     * Setup OpenCL program.
     */
    {
        /* Create a context with a command queue on the specified device. */
        m_context = cl::Context::create(CL_DEVICE_TYPE_GPU);
        m_device = cl::Context::get_device(m_context, Params::device_index);
        m_queue = cl::Queue::create(m_context, m_device);
        std::cout << cl::Device::get_info_string(m_device) << "\n";

        /* Create the program object. */
        m_program = cl::Program::create_from_file(m_context, "data/pi.cl");
        cl::Program::build(m_program, m_device, "");
        std::cout << cl::Program::get_source(m_program) << "\n";
    }

    /*
     * Setup Model data.
     */
    {
        m_data.group_sums.resize(Params::num_work_groups, 0.0);
        m_data.pi_cpu = 0.0;
        m_data.pi_gpu = 0.0;

        cl_double delta_x = 1.0 / (cl_double) n_procs;
        m_data.xlo = delta_x * (cl_double) proc_id;
        m_data.xhi = m_data.xlo + delta_x;

        m_data.proc_id = proc_id;
        m_data.n_procs = n_procs;
    }

    /*
     * Setup Pi Kernel data.
     */
    {
        /* Create the pi integration in the program object. */
        m_kernels.resize(NumKernels, NULL);
        m_kernels[KernelResetPi] = cl::Kernel::create(m_program, "reset_pi");
        m_kernels[KernelComputePi] = cl::Kernel::create(m_program, "compute_pi");

        /* Create memory buffers. */
        m_buffers.resize(NumBuffers, NULL);
        m_buffers[BufferGroupSums] = cl::Memory::create_buffer(
            m_context,
            CL_MEM_WRITE_ONLY,
            m_data.group_sums.size() * sizeof(m_data.group_sums[0]),
            (void *) NULL);
    }
}

/**
 * Model::~Model
 * @brief Destroy the OpenCL context and associated objects.
 */
Model::~Model()
{
    /* Teardown OpenCL data. */
    {
        for (auto &it : m_images) {
            cl::Memory::release(it);
        }
        for (auto &it : m_buffers) {
            cl::Memory::release(it);
        }
        for (auto &it : m_kernels) {
            cl::Kernel::release(it);
        }
        cl::Program::release(m_program);
        cl::Queue::release(m_queue);
        cl::Device::release(m_device);
        cl::Context::release(m_context);
    }
}

/** ---------------------------------------------------------------------------
 * Model::execute
 * @brief Execute the model.
 */
void Model::execute()
{
    execute_cpu();
    execute_gpu();
}

/**
 * Model::execute_cpu
 * @brief Integrate 4/(1+x*x) dx from 0 to 1 to return pi.
 */
void Model::execute_cpu(void)
{
    cl_ulong n_steps = Params::n_interval_steps * Params::n_intervals;
    cl_double step_size = (m_data.xhi - m_data.xlo) / (cl_double) n_steps;

    cl_double sum = 0.0;
    for (cl_ulong i = 0; i < n_steps; ++i) {
        cl_double x = m_data.xlo + (i + 0.5) * step_size;
        sum += 4.0 / (1.0 + x*x);
    }
    m_data.pi_cpu = sum * step_size;
}

/**
 * Model::execute_gpu
 */
void Model::execute_gpu(void)
{
    /* Reset kernel partial sums. */
    {
        /* Set kernel arguments. */
        cl::Kernel::set_arg(m_kernels[KernelResetPi], 0, sizeof(cl_mem), &m_buffers[BufferGroupSums]);
        cl::Kernel::set_arg(m_kernels[KernelResetPi], 1, sizeof(cl_ulong), &Params::n_intervals);

        /* Run the kernel */
        cl::Queue::enqueue_nd_range_kernel(
            m_queue,
            m_kernels[KernelResetPi],
            cl::NDRange::Null,                      /* global work offset */
            cl::NDRange(Params::num_work_items),    /* global work size */
            cl::NDRange(Params::work_group_size),   /* local work size */
            NULL,
            NULL);
    }

    /* Compute pi. */
    {
        /* Set kernel arguments. */
        cl::Kernel::set_arg(m_kernels[KernelComputePi], 0, sizeof(cl_mem), &m_buffers[BufferGroupSums]);
        cl::Kernel::set_arg(m_kernels[KernelComputePi], 1, Params::work_group_size * sizeof(cl_double), NULL);
        cl::Kernel::set_arg(m_kernels[KernelComputePi], 2, sizeof(cl_ulong), &Params::n_intervals);
        cl::Kernel::set_arg(m_kernels[KernelComputePi], 3, sizeof(cl_ulong), &Params::n_interval_steps);
        cl::Kernel::set_arg(m_kernels[KernelComputePi], 4, sizeof(cl_double), &m_data.xlo);
        cl::Kernel::set_arg(m_kernels[KernelComputePi], 5, sizeof(cl_double), &m_data.xhi);

        /* Run the kernel */
        cl::Queue::enqueue_nd_range_kernel(
            m_queue,
            m_kernels[KernelComputePi],
            cl::NDRange::Null,                      /* global work offset */
            cl::NDRange(Params::num_work_items),    /* global work size */
            cl::NDRange(Params::work_group_size),   /* local work size */
            NULL,
            NULL);

        /* Wait for kernel to compute */
        cl::Queue::finish(m_queue);

        /* Read the partial sums back to the host. */
        cl::Queue::enqueue_read_buffer(
            m_queue,
            m_buffers[BufferGroupSums],
            CL_TRUE,
            0,
            m_data.group_sums.size() * sizeof(m_data.group_sums[0]),
            (void *) &m_data.group_sums[0],
            NULL,
            NULL);
    }

    /* Compute final integral value from the kernel partial sums. */
    {
        cl_double sum = 0.0;
        for (cl_ulong i = 0; i < Params::num_work_groups; ++i) {
            sum += m_data.group_sums[i];
        }
        m_data.pi_gpu = sum;
    }
}

/** ---------------------------------------------------------------------------
 * Model::handle
 * Handle an event.
 */
void Model::handle(const gl::Event &event)
{}
