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
    std::vector<GLfloat> vertex_data = {
        /* positions */
        -0.5f, -0.5f, 0.0f, 1.0f,   /* bottom left */
         0.5f, -0.5f, 0.0f, 1.0f,   /* bottom right */
        -0.5f,  0.5f, 0.0f, 1.0f,   /* top left */
         0.5f,  0.5f, 0.0f, 1.0f,   /* top right */
        /* colors */
         0.0f,  0.0f, 1.0f, 1.0f,   /* bottom left */
         0.0f,  1.0f, 0.0f, 1.0f,   /* bottom right */
         1.0f,  0.0f, 0.0f, 1.0f,   /* top left */
         1.0f,  0.0f, 0.0f, 1.0f};  /* top right */
    GLsizeiptr vertex_data_size = vertex_data.size() * sizeof(GLfloat);

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
    m_vbo = gl::create_buffer(
        GL_ARRAY_BUFFER,
        vertex_data_size,
        GL_STATIC_DRAW);
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

    std::array<GLfloat,2> sizef = gl::Renderer::framebuffer_sizef();
    gl::set_uniform(m_program, "u_width", GL_FLOAT, &sizef[0]);
    gl::set_uniform(m_program, "u_height", GL_FLOAT, &sizef[1]);

    glDrawArrays(
        GL_TRIANGLES,           /* what kind of primitives to render */
        0,                      /* starting index in the enabled arrays */
        3);                     /* number of indices to be rendered */

    glBindVertexArray(0);       /* unbind vertex array object */

    /* Unbind the shader program object. */
    glUseProgram(0);
}
