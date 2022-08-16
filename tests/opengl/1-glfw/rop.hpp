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

#ifndef TEST_ATTO_OPENGL_GLFW_ROP_H_
#define TEST_ATTO_OPENGL_GLFW_ROP_H_

#include "atto/opengl/opengl.hpp"

/**
 * Rop
 * @brief Minimal drawable object that clears the OpenGL buffers.
 */
struct Rop : atto::gl::Drawable {
    /* OpenGL color buffer and color depth variables. */
    GLclampf r;                    /* color buffer red channel */
    GLclampf g;                    /* color buffer green channel */
    GLclampf b;                    /* color buffer blue channel */
    GLclampf a;                    /* color buffer alpha channel */
    GLclampd d;                    /* depth buffer value */

    /* Handle and draw member functions. */
    void handle(const atto::gl::Event &event) override;
    void draw(void *data = nullptr) override;

    /* Constructor/destructor. */
    Rop();
    ~Rop() = default;
    Rop(const Rop &) = delete;
    Rop &operator=(const Rop &) = delete;
};

#endif /* TEST_ATTO_OPENGL_GLFW_ROP_H_ */
