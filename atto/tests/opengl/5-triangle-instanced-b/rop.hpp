/*
 * rop.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ATTO_OPENGL_ROP_H_
#define TEST_ATTO_OPENGL_ROP_H_

#include <vector>
#include "atto/math/math.hpp"
#include "atto/opengl/opengl.hpp"

/**
 * Rop
 */
struct Rop : atto::gl::Drawable {
    GLuint m_program;                           /* shader program object */
    GLuint m_vao;                               /* vertex array object */
    GLuint m_vbo;                               /* vertex buffer object */

    const size_t m_ncells = 10;
    std::vector<atto::math::vec3f> m_offset;    /* triangle offsets */
    atto::math::mat4f m_mvp;                    /* modelviewprojection */

    /* Handle and draw member functions. */
    void handle(const atto::gl::Event &event) override;
    void draw(void *data = nullptr) override;

    /* Constructor/destructor. */
    Rop();
    ~Rop() = default;
    Rop(const Rop &) = delete;
    Rop &operator=(const Rop &) = delete;
};

#endif /* TEST_ATTO_OPENGL_ROP_H_ */
