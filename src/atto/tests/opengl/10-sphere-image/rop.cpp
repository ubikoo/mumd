/*
 * rop.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/opengl.hpp"
#include "rop.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * Rop::Rop
 */
Rop::Rop(const std::string &filename, const size_t n1, const size_t n2)
{
    /*
     * Create the shader program object.
     */
    {
        std::vector<GLuint> shaders{
            gl::create_shader(GL_VERTEX_SHADER, "data/viewer.vert"),
            gl::create_shader(GL_FRAGMENT_SHADER, "data/viewer.frag")};
        m_program = gl::create_program(shaders);
        std::cout << gl::get_program_info(m_program) << "\n";
    }

    /*
     * Load the 2d-image from the specified filename
     */
    {
        m_image = std::make_unique<gl::Image>(filename, true);
        m_texture = gl::create_texture2d(
            GL_RGBA8,                   /* texture internal format */
            m_image->width(),            /* texture width */
            m_image->height(),           /* texture height */
            m_image->pixelformat(),      /* pixel format */
            GL_UNSIGNED_BYTE,           /* pixel type */
            m_image->bitmap());          /* pixel data */

        glBindTexture(GL_TEXTURE_2D, m_texture);
        gl::set_texture_mipmap(
            GL_TEXTURE_2D,
            GL_TRUE);                   /* generate mipmap */
        gl::set_texture_wrap(
            GL_TEXTURE_2D,
            GL_CLAMP_TO_EDGE,           /* wrap_s */
            GL_CLAMP_TO_EDGE);          /* wrap_t */
        gl::set_texture_filter(
            GL_TEXTURE_2D,
            GL_LINEAR,                  /* filter_min */
            GL_LINEAR);                 /* filter_mag */
        glBindTexture(GL_TEXTURE_2D, 0);

        /*
         * Create a sphere mesh and set the mesh vertex attributes in the program.
        */
        m_mesh = gl::Mesh::Sphere(
            m_program,                  /* shader program object */
            "sphere",                   /* vertex attributes prefix */
            n1,                         /* n1 vertices */
            n2,                         /* n2 vertices */
            1.0,                        /* radius */
            0.0,                        /* phi_lo */
            M_PI,                       /* phi_hi */
            -M_PI,                      /* theta_lo */
            M_PI);                      /* theta_hi */
    }
}

/** ---------------------------------------------------------------------------
 * Rop::handle
 * @brief Handle the event.
 */
void Rop::handle(const gl::Event &event)
{}

/** ---------------------------------------------------------------------------
 * Rop::draw
 * @brief Render the drawable.
 */
void Rop::draw(void *data)
{
    GLFWwindow *window = gl::Renderer::window();
    if (window == nullptr) {
        return;
    }

    /*
     * Update the drawable state.
     */
    {
        /* Update the modelviewprojection matrix */
        float time = (float) glfwGetTime();

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f(1.0f, 0.0f, 0.0f), (float) (0.5*M_PI));

        math::vec4f dir_y = math::dot(m, math::vec4f(0.0, 1.0, 0.0, 1.0));
        m = math::rotate(m, math::vec3f(dir_y), 0.2f*time);

        math::vec4f dir_z = math::dot(m, math::vec4f(0.0, 0.0, 1.0, 1.0));
        m = math::rotate(m, math::vec3f(dir_z), 0.8f*time);

        std::array<GLfloat,2> size = gl::Renderer::framebuffer_sizef();
        float ratio = size[0] / size[1];

        math::mat4f p = math::ortho(-ratio, ratio, -1.0f, 1.0f, 0.1f, 1.0f);
        m_mvp = math::dot(p, m);
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
    glUseProgram(m_program);

    /* Set window dimensions. */
    std::array<GLfloat,2> size = gl::Renderer::framebuffer_sizef();
    gl::set_uniform(m_program, "u_width", GL_FLOAT, &size[0]);
    gl::set_uniform(m_program, "u_height", GL_FLOAT, &size[1]);
    gl::set_uniform_matrix(
        m_program, "u_mvp", GL_FLOAT_MAT4, true, m_mvp.data());

    /* Set the sampler uniform with the texture unit and bind the texture */
    GLenum texunit = 0;
    gl::set_uniform(m_program, "u_texsampler", GL_SAMPLER_2D, &texunit);
    gl::active_bind_texture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, m_texture);

    /* Draw the mesh */
    m_mesh->draw();

    /* Unbind the shader program object. */
    glUseProgram(0);
}
