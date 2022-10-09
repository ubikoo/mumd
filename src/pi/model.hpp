/*
 * model.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include "base.hpp"

struct Model {
    /* ---- Model OpenCL data ---------------------------------------------- */
    cl_context m_context = NULL;
    cl_device_id m_device = NULL;
    cl_command_queue m_queue = NULL;
    cl_program m_program = NULL;

    enum {
        KernelResetPi = 0,
        KernelComputePi,
        NumKernels
    };
    std::vector<cl_kernel> m_kernels;

    enum {
        BufferGroupSums = 0,
        NumBuffers
    };
    std::vector<cl_mem> m_buffers;

    enum {
        NumImages = 0
    };
    std::vector<cl_mem> m_images;

    /* ---- Model data ----------------------------------------------------- */
    struct Data {
        std::vector<cl_double> group_sums;
        cl_double pi_cpu;
        cl_double pi_gpu;
        cl_double xlo;
        cl_double xhi;
        cl_int proc_id;
        cl_int n_procs;
    } m_data;

    /* ---- Model member functions ----------------------------------------- */
    void execute();
    void execute_gpu(void);
    void execute_cpu(void);
    void handle(const atto::gl::Event &event);

    explicit Model(const int proc_id, const int n_procs);
    ~Model();
    Model(const Model &) = delete;
    Model &operator=(const Model &) = delete;
};

#endif /* MODEL_H_ */
