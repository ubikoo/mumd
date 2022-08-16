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
#include "camera.hpp"
#include "rop.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * Rop::Rop
 * @brief Create a image panorama drawable
 */
Rop::Rop(const std::string &filename, const size_t n1, const size_t n2)
{
    /*
     * Create the shader program object.
     */
    {
        std::vector<GLuint> shaders{
            gl::create_shader(GL_VERTEX_SHADER, "data/panorama.vert"),
            gl::create_shader(GL_FRAGMENT_SHADER, "data/panorama.frag")};
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
            m_image->width(),           /* texture width */
            m_image->height(),          /* texture height */
            m_image->pixelformat(),     /* pixel format */
            GL_UNSIGNED_BYTE,           /* pixel type */
            m_image->bitmap());         /* pixel data */

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
         * Create a mesh over a rectangle with screen size
        * and set the mesh vertex attributes in the program.
        */
        m_mesh = gl::Mesh::Plane(
            m_program,                  /* shader program object */
            "quad",                     /* vertex attributes prefix */
            n1,                         /* n1 vertices */
            n2,                         /* n2 vertices */
            -1.0,                       /* xlo */
            1.0,                        /* xhi */
            -1.0,                       /* ylo */
            1.0);                       /* yhi */
    }
}

/** ---------------------------------------------------------------------------
 * Rop::handle
 * @brief Handle the event.
 */
void Rop::handle(const gl::Event &event)
{
    GLFWwindow *window = gl::Renderer::window();
    if (window == nullptr) {
        return;
    }

    if (event.type == gl::Event::Key && event.key.code == GLFW_KEY_LEFT) {
        m_camera.rotate_left();
    }

    if (event.type == gl::Event::Key && event.key.code == GLFW_KEY_RIGHT) {
        m_camera.rotate_right();
    }

    if (event.type == gl::Event::Key && event.key.code == GLFW_KEY_UP) {
        m_camera.rotate_up();
    }

    if (event.type == gl::Event::Key && event.key.code == GLFW_KEY_DOWN) {
        m_camera.rotate_down();
    }
}

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
        /* Update the panorama camera and modelviewprojection matrix */
        std::array<GLfloat,2> sizef = gl::Renderer::framebuffer_sizef();
        const float fovy = 0.5*M_PI;
        const float aspect = sizef[0] / sizef[1];
        const float znear = 0.1f;
        const float zfar = 10.0f;
        math::mat4f proj = math::perspective(fovy, aspect, znear, zfar);
        math::mat4f view = m_camera.lookat();
        m_mvp = math::dot(proj, view);
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
    std::array<GLfloat,2> sizef = gl::Renderer::framebuffer_sizef();
    gl::set_uniform(m_program, "u_width", GL_FLOAT, &sizef[0]);
    gl::set_uniform(m_program, "u_height", GL_FLOAT, &sizef[1]);
    gl::set_uniform_matrix(m_program, "u_mvp", GL_FLOAT_MAT4, true, m_mvp.data());

    /* Set the sampler uniform with the texture unit and bind the texture */
    GLenum texunit = 0;
    gl::set_uniform(m_program, "u_texsampler", GL_SAMPLER_2D, &texunit);
    gl::active_bind_texture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, m_texture);

    /* Draw the mesh */
    m_mesh->draw();

    /* Unbind the shader program object. */
    glUseProgram(0);
}
