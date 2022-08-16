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

#include <memory>
#include "atto/math/math.hpp"
#include "atto/opengl/opengl.hpp"

/**
 * Rop
 */
struct Rop : atto::gl::Drawable {
    GLuint m_program;                           /* shader program object */
    std::unique_ptr<atto::gl::Mesh> m_mesh;     /* image and texture */
    std::unique_ptr<atto::gl::Image> m_image;
    GLuint m_texture;
    atto::math::mat4f m_mvp;                    /* modelviewprojection */

    /* Handle and draw member functions. */
    void handle(const atto::gl::Event &event) override;
    void draw(void *data = nullptr) override;

    /* Constructor/destructor. */
    Rop(const std::string &filename, const size_t n1, const size_t n2);
    ~Rop() = default;
    Rop(const Rop &) = delete;
    Rop &operator=(const Rop &) = delete;
};

#endif /* TEST_ATTO_OPENGL_ROP_H_ */
