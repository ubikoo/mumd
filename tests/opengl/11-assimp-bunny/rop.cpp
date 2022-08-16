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
 * @brief Create the bunny drawable
 */
Rop::Rop(const std::string &filename)
{
    /*
     * Create the shader program object.
     */
    std::vector<GLuint> shaders{
        gl::create_shader(GL_VERTEX_SHADER, "data/bunny.vert"),
        gl::create_shader(GL_FRAGMENT_SHADER, "data/bunny.frag")};
    m_program = gl::create_program(shaders);
    std::cout << gl::get_program_info(m_program) << "\n";

    /*
     * Create a mesh over a rectangle with screen size
     * and set the mesh vertex attributes in the program.
     */
    m_bunny = std::make_unique<gl::MeshModel>(
        m_program,                  /* shader program object */
        "bunny",                    /* vertex attributes prefix */
        "data/bunny.ply");
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

        float ang_x = 0.6 * time;
        float ang_y = 0.4 * time;
        float ang_z = 0.2 * time;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f(0.0f, 0.0f, 1.0f), ang_z);
        m = math::rotate(m, math::vec3f(0.0f, 1.0f, 0.0f), ang_y);
        m = math::rotate(m, math::vec3f(1.0f, 0.0f, 0.0f), ang_x);
        m = math::scale(m, math::vec3f(5.0f, 5.0f, 5.0f));

        std::array<GLfloat,2> size = gl::Renderer::framebuffer_sizef();
        float ratio = size[0] / size[1];

        math::mat4f p = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
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

    /* Set uniform and draw. */
    gl::set_uniform_matrix(m_program, "u_mvp", GL_FLOAT_MAT4, true, m_mvp.data());
    m_bunny->draw();

    /* Unbind the shader program object. */
    glUseProgram(0);
}
