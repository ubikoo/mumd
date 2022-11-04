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
    : r(0.0f)
    , g(0.0f)
    , b(0.0f)
    , a(1.0f)
    , d(1.0f)
{}

/** ---------------------------------------------------------------------------
 * Rop::handle
 */
void Rop::handle(const gl::Event &event)
{}

/** ---------------------------------------------------------------------------
 * Rop::draw
 */
void Rop::draw(void *data)
{
    GLFWwindow *window = gl::Renderer::window();
    if (window == nullptr) {
        return;
    }

    /*
     * Update OpenGL rop and depth buffer values.
     */
    {
        const double wr = 0.3;
        const double wg = 0.2;
        const double wb = 0.1;
        const double t = glfwGetTime();
        r = 0.5*(std::cos(wr*t) + 1.0);
        g = 0.5*(std::cos(wg*t) + 1.0);
        b = 0.5*(std::cos(wb*t) + 1.0);
        a = 1.0;
    }

    gl::Renderer::clear(r, g, b, a, 1.0);
}
