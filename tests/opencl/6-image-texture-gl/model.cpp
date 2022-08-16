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
 * @brief Create the OpenCL context and associated objects.
 */
Model::Model()
{
    /*
     * Setup Model OpenGL data.
     */
    {
        /*
         * Create the shader program object.
         */
        std::vector<GLuint> shaders{
            gl::create_shader(GL_VERTEX_SHADER, "data/image-rotate.vert"),
            gl::create_shader(GL_FRAGMENT_SHADER, "data/image-rotate.frag")};
        m_gl.program = gl::create_program(shaders);
        std::cout << gl::get_program_info(m_gl.program) << "\n";

        /*
         * Load the 2d-image from the specified filename.
         */
        m_gl.image = std::make_unique<gl::Image>("../data/monarch_512.png", true, 4);
        std::cout << m_gl.image->infolog("Image attributes:") << "\n";

        m_gl.texture[0] = gl::create_texture2d(
            GL_RGBA8,                       /* texture internal format */
            m_gl.image->width(),            /* texture width */
            m_gl.image->height(),           /* texture height */
            m_gl.image->pixelformat(),      /* pixel format */
            GL_UNSIGNED_BYTE,               /* pixel type */
            m_gl.image->bitmap());          /* pixel data */
        glBindTexture(GL_TEXTURE_2D, m_gl.texture[0]);
        gl::set_texture_mipmap(
            GL_TEXTURE_2D,
            GL_TRUE);                       /* generate mipmap */
        gl::set_texture_wrap(
            GL_TEXTURE_2D,
            GL_CLAMP_TO_EDGE,               /* wrap_s */
            GL_CLAMP_TO_EDGE);              /* wrap_t */
        gl::set_texture_filter(
            GL_TEXTURE_2D,
            GL_LINEAR,                      /* filter_min */
            GL_LINEAR);                     /* filter_mag */
        glBindTexture(GL_TEXTURE_2D, 0);

        m_gl.texture[1] = gl::create_texture2d(
            GL_RGBA8,                       /* texture internal format */
            m_gl.image->width(),            /* texture width */
            m_gl.image->height(),           /* texture height */
            m_gl.image->pixelformat(),      /* pixel format */
            GL_UNSIGNED_BYTE,               /* pixel type */
            m_gl.image->bitmap());          /* pixel data */
        glBindTexture(GL_TEXTURE_2D, m_gl.texture[1]);
        gl::set_texture_mipmap(
            GL_TEXTURE_2D,
            GL_TRUE);                       /* generate mipmap */
        gl::set_texture_wrap(
            GL_TEXTURE_2D,
            GL_CLAMP_TO_EDGE,               /* wrap_s */
            GL_CLAMP_TO_EDGE);              /* wrap_t */
        gl::set_texture_filter(
            GL_TEXTURE_2D,
            GL_LINEAR,                      /* filter_min */
            GL_LINEAR);                     /* filter_mag */
        glBindTexture(GL_TEXTURE_2D, 0);

        /*
         * Create a mesh over a rectangle with screen size and set the mesh
         * vertex attributes in the program.
         */
        GLfloat aspect = (GLfloat) m_gl.image->width() / m_gl.image->height();
        GLfloat xrange = aspect > 1.0 ? 1.0 : aspect;
        GLfloat yrange = aspect > 1.0 ? 1.0 / aspect : 1.0;
        m_gl.mesh = gl::Mesh::Plane(
            m_gl.program,           /* shader program object */
            "quad",                 /* vertex attributes prefix */
            2,                      /* n1 vertices */
            2,                      /* n2 vertices */
            -xrange,                /* xlo */
            xrange,                 /* xhi */
            -yrange,                /* ylo */
            yrange);                /* yhi */
    }

    /*
     * Setup Model OpenCL data.
     */
    {
        /*
         * Setup OpenCL context based on the OpenGL context in the device.
         */
        std::vector<cl_device_id> devices = cl::Device::get_device_ids(CL_DEVICE_TYPE_GPU);
        core_assert(Params::device_index < devices.size(), "device index overflow");
        m_device = devices[Params::device_index];
        m_context = cl::Context::create_cl_gl_shared(m_device);

        /*
         * Create a command queue on the specified device.
         */
        m_queue = cl::Queue::create(m_context, m_device);

        /*
         * Create a OpenCL program for the kernel source.
         */
        m_program = cl::Program::create_from_file(m_context, "data/image-rotate.cl");
        std::cout << cl::Program::get_source(m_program) << "\n";
        cl::Program::build(m_program, m_device, "");

        /*
         * Create the OpenCL kernel.
         */
        m_kernels.resize(NumKernels, NULL);
        m_kernels[ImageRotate] = cl::Kernel::create(m_program, "image_rotate");

        /*
         * Create OpenCL image store from OpenGL texture objects.
         */
        m_images.resize(NumImages, NULL);

        glBindTexture(GL_TEXTURE_2D, m_gl.texture[0]);
        m_images[SrcImage] = cl::gl::create_from_gl_texture(
            m_context,
            CL_MEM_READ_ONLY,
            GL_TEXTURE_2D,
            0,
            m_gl.texture[0]);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, m_gl.texture[1]);
        m_images[DstImage] = cl::gl::create_from_gl_texture(
            m_context,
            CL_MEM_WRITE_ONLY,
            GL_TEXTURE_2D,
            0,
            m_gl.texture[1]);
        glBindTexture(GL_TEXTURE_2D, 0);
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

    /*
     * Specify draw state modes.
     */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /* Bind the shader program object. */
    glUseProgram(m_gl.program);

    /* Set the sampler uniform with the texture unit and bind the texture */
    GLenum texunit = 0;
    gl::set_uniform(m_gl.program, "u_texsampler", GL_SAMPLER_2D, &texunit);
    gl::active_bind_texture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, m_gl.texture[1]);

    /* Draw the mesh */
    m_gl.mesh->draw();

    /* Unbind the shader program object. */
    glUseProgram(0);
}

/** ---------------------------------------------------------------------------
 *
 * Model::execute
 * @brief Execute the model.
 */
void Model::execute(void)
{
    /* Set kernel arguments. */
    cl_sampler sampler = cl::Sampler::create(
        m_context,
        CL_FALSE,
        CL_ADDRESS_CLAMP,
        CL_FILTER_LINEAR);

    float theta = m_theta_freq * glfwGetTime();
    float cos_theta = std::cos(theta);
    float sin_theta = std::sin(theta);
    cl_long width = m_gl.image->width();
    cl_long height = m_gl.image->height();

    cl::Kernel::set_arg(m_kernels[ImageRotate], 0, sizeof(cl_mem),     &m_images[SrcImage]);
    cl::Kernel::set_arg(m_kernels[ImageRotate], 1, sizeof(cl_mem),     &m_images[DstImage]);
    cl::Kernel::set_arg(m_kernels[ImageRotate], 2, sizeof(cl_sampler), &sampler);
    cl::Kernel::set_arg(m_kernels[ImageRotate], 3, sizeof(cl_long),    &width);
    cl::Kernel::set_arg(m_kernels[ImageRotate], 4, sizeof(cl_long),    &height);
    cl::Kernel::set_arg(m_kernels[ImageRotate], 5, sizeof(cl_float),   &cos_theta);
    cl::Kernel::set_arg(m_kernels[ImageRotate], 6, sizeof(cl_float),   &sin_theta);

    std::cout << "\ntheta "
              << theta << " "
              << cos_theta << " "
              << sin_theta << "\n";

    /* Set the size of the NDRange workgroups and run the kernel. */
    cl::NDRange local_ws(
        Params::work_group_size_2d,
        Params::work_group_size_2d);
    cl::NDRange global_ws(
        cl::NDRange::Roundup(width,  local_ws(0)),
        cl::NDRange::Roundup(height, local_ws(1)));

    std::cout << "local_ws "
              << local_ws(0) << " "
              << local_ws(1) << " "
              << local_ws(2) << "\n";
    std::cout << "global_ws "
              << global_ws(0) << " "
              << global_ws(1) << " "
              << global_ws(2) << "\n";

    /* Wait for OpenGL to finish and acquire the gl objects. */
    cl::gl::enqueue_acquire_gl_objects(
        m_queue,
        &m_images,
        NULL,
        NULL);

    /* Enqueue the OpenCL kernel for execution. */
    cl::Queue::enqueue_nd_range_kernel(
        m_queue,
        m_kernels[ImageRotate],
        cl::NDRange::Null,
        global_ws,
        local_ws,
        NULL,
        NULL);

    /* Wait for OpenCL to finish and release the gl objects. */
    cl::gl::enqueue_release_gl_objects(
        m_queue,
        &m_images,
        NULL,
        NULL);
}
