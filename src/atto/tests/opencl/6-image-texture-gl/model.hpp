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
#include "../base.hpp"

/**
 * Model
 * @brief Data aggregate holding the model state.
 */
struct Model : atto::gl::Drawable {
    /* ---- Model data ----------------------------------------------------- */
    const float m_theta_freq = 0.1 * M_PI;      /* angle rotation frequency */

    /* ---- Model OpenCL data ---------------------------------------------- */
    cl_context m_context = NULL;
    cl_device_id m_device = NULL;
    cl_command_queue m_queue = NULL;
    cl_program m_program = NULL;
    enum {
        ImageRotate = 0,
        NumKernels
    };
    std::vector<cl_kernel> m_kernels;
    enum {
        NumBuffers = 0
    };
    std::vector<cl_mem> m_buffers;
    enum {
        SrcImage = 0,
        DstImage,
        NumImages
    };
    std::vector<cl_mem> m_images;

    /* ---- Model OpenGL objects ------------------------------------------- */
    struct GLData {
        GLuint program;                         /* shader program object */
        std::unique_ptr<atto::gl::Mesh> mesh;
        std::unique_ptr<atto::gl::Image> image; /* image and texture */
        std::array<GLuint,2> texture;
    };
    GLData m_gl;

    /* ---- Model member functions ----------------------------------------- */
    void handle(const atto::gl::Event &event) override;
    void draw(void *data = nullptr) override;
    void execute(void);

    Model();
    ~Model();

    Model(const Model &) = delete;
    Model &operator=(const Model &) = delete;
};

#endif /* MODEL_H_ */
