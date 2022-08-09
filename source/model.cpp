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
Model::Model(void)
{
    /*
     * Setup Model data.
     */
    { /* Empty */ }

    /*
     * Setup OpenGL data.
     */
    { /* Empty */ }

    /*
     * Setup OpenCL data.
     */
    {
        /* Create OpenCL context. */
// #if USE_CL_GL_INTEROP
//     /* Setup OpenCL context based on the OpenGL context in the device. */
//     m_device = cl::Context::get_device(m_context, Params::device_index);
//     m_context = cl::Context::create_cl_gl_shared(m_device);
//     m_queue = cl::Queue::create(m_context, m_device);
// #else
//     /* Setup OpenCL context with a command queue on the specified device. */
//     m_context = cl::Context::create(CL_DEVICE_TYPE_GPU);
//     m_device = cl::Context::get_device(m_context, Params::device_index);
//     m_queue = cl::Queue::create(m_context, m_device);
// #endif

        /* Create OpenCL program. */
        // std::string source;
        // source.append(cl::Program::load_source_from_file("data/empty.cl"));
        // m_program = cl::Program::create_from_source(m_context, source);
        // cl::Program::build(m_program, m_device, "");

        // m_program = cl::Program::create_from_file(m_context, "data/empty.cl");
        // cl::Program::build(m_program, m_device, "");

        /* Create OpenCL kernel. */
        // m_kernels.resize(NumKernels, NULL);
        // m_kernels[KernelEmpty] = cl::Kernel::create(m_program, "empty");
    }
}

/**
 * Model::~Model
 * @brief Destroy the OpenCL context and associated objects.
 */
Model::~Model()
{
    /* Teardown model data. */
    {
    }

    /* Teardown OpenCL data. */
    {
    //     for (auto &it : m_images) {
    //         cl::Memory::release(it);
    //     }
    //     for (auto &it : m_buffers) {
    //         cl::Memory::release(it);
    //     }
    //     for (auto &it : m_kernels) {
    //         cl::Kernel::release(it);
    //     }
    //     cl::Program::release(m_program);
    }
}

/** ---------------------------------------------------------------------------
 * Model::handle
 * @brief Handle the event.
 */
void Model::handle(const gl::Event &event)
{}

/** ---------------------------------------------------------------------------
 * Model::draw
 * @brief Render the drawable.
 */
void Model::draw(void *data)
{
    GLFWwindow *window = gl::Renderer::window();
    if (window == nullptr) {
        return;
    }
}

/** ---------------------------------------------------------------------------
 * Model::execute
 * @brief Execute the model.
 */
void Model::execute(void)
{}
