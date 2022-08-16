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
Rop::Rop()
{
    /*
     * Create the vertex position and color attributes with layout:
     * {(xyzw)_1,
     *  (xyzw)_2,
     *  ...
     *  (rgba)_1,
     *  (rgba)_2}
     */
    const GLfloat scale = 1.0 / static_cast<GLfloat>(m_offset_n_cells);
    std::vector<GLfloat> vertex_data = {
        /* positions */
        -1.0f*scale, -1.0f*scale, 0.0f*scale, 1.0f, /* bottom left */
         1.0f*scale, -1.0f*scale, 0.0f*scale, 1.0f, /* bottom right */
        -0.0f*scale,  1.0f*scale, 0.0f*scale, 1.0f, /* top */
        /* colors */
         0.0f,  0.0f, 1.0f, 1.0f,                   /* bottom left */
         0.0f,  1.0f, 0.0f, 1.0f,                   /* bottom right */
         1.0f,  0.0f, 0.0f, 1.0f};                  /* top  */

    /*
     * Compute the rop offset along each dimension.
     */
    m_offset_data.clear();
    for (size_t i = 0; i < m_offset_n_cells; ++i) {
        for (size_t j = 0; j < m_offset_n_cells; ++j) {
            for (size_t k = 0; k < m_offset_n_cells; ++k) {
                m_offset_data.push_back(
                    -1.0f + 2.0f*scale*static_cast<GLfloat>(i));
                m_offset_data.push_back(
                    -1.0f + 2.0f*scale*static_cast<GLfloat>(j));
                m_offset_data.push_back(
                    -1.0f + 2.0f*scale*static_cast<GLfloat>(k));
            }
        }
    }

    /*
     * Create the shader program object.
     */
    std::vector<GLuint> shaders{
        gl::create_shader(GL_VERTEX_SHADER, "data/triangle.vert"),
        gl::create_shader(GL_FRAGMENT_SHADER, "data/triangle.frag")};
    m_program = gl::create_program(shaders);
    std::cout << gl::get_program_info(m_program) << "\n";

    /*
     * Create vertex array object.
     */
    m_vao = gl::create_vertex_array();
    glBindVertexArray(m_vao);

    /*
     * Create a buffer storage for the vertex position and color attributes.
     */
    GLsizeiptr vertex_data_size = vertex_data.size() * sizeof(GLfloat);
    m_vbo = gl::create_buffer(
        GL_ARRAY_BUFFER,
        vertex_data_size,
        GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,            /* target binding point */
        0,                          /* offset in data store */
        vertex_data_size,           /* data store size in bytes */
        vertex_data.data());        /* pointer to data source */

    /*
     * Specify how OpenGL interprets the vertex attributes.
     */
    GLsizeiptr offset_pos = 0;
    gl::enable_attribute(m_program, "a_pos");
    gl::attribute_pointer(
        m_program,
        "a_pos",
        GL_FLOAT_VEC4,
        4*sizeof(GLfloat),  /* byte offset between consecutive attributes */
        offset_pos,         /* byte offset of first element in the buffer */
        false);             /* normalized flag */

    GLsizeiptr offset_col = vertex_data_size / 2;
    gl::enable_attribute(m_program, "a_col");
    gl::attribute_pointer(
        m_program,
        "a_col",
        GL_FLOAT_VEC4,
        4*sizeof(GLfloat),  /* byte offset between consecutive attributes */
        offset_col,         /* byte offset of first element in the buffer */
        false);             /* normalized flag */

    /*
     * Create a buffer storage for the rop offsets.
     */
    GLsizeiptr offset_data_size = m_offset_data.size() * sizeof(GLfloat);
    m_offset_data_vbo = gl::create_buffer(
        GL_ARRAY_BUFFER,
        offset_data_size,
        GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_offset_data_vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,            /* target binding point */
        0,                          /* offset in data store */
        offset_data_size,           /* data store size in bytes */
        m_offset_data.data());       /* pointer to data source */

    gl::enable_attribute(m_program, "a_offset");
    gl::attribute_pointer(
        m_program,
        "a_offset",
        GL_FLOAT_VEC3,
        sizeof(GLfloat),    /* byte offset between consecutive attributes */
        0,                  /* byte offset of first element in the buffer */
        false);             /* normalized flag */
    gl::attribute_divisor(m_program, "a_offset", 1);

    /*
     * Unbind vertex array object.
     */
    glBindVertexArray(0);
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
        float time = (float) glfwGetTime();
        float ang_x = 0.6 * time;
        float ang_y = 0.4 * time;
        float ang_z = 0.2 * time;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f(0.0f, 0.0f, 1.0f), ang_z);
        m = math::rotate(m, math::vec3f(0.0f, 1.0f, 0.0f), ang_y);
        m = math::rotate(m, math::vec3f(1.0f, 0.0f, 0.0f), ang_x);
        m = math::scale(m, math::vec3f(1.0f));

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

    /* Get window dimensions and set corresponding uniforms. */
    glBindVertexArray(m_vao);   /* bind vertex array object */
    gl::set_uniform_matrix(
        m_program, "u_mvp", GL_FLOAT_MAT4, true, m_mvp.data());

    /* Draw multiple instances of a range of elements. */
    glDrawArraysInstanced(
        GL_TRIANGLES,           /* what kind of primitives to render */
        0,                      /* starting index in the enabled arrays */
        3,                      /* number of indices to be rendered */
        m_offset_data.size());   /* number of instances to be rendered */

    glBindVertexArray(0);       /* unbind vertex array object */

    /* Unbind the shader program object. */
    glUseProgram(0);
}
