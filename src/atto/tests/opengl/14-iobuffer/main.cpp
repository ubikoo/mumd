/*
 * main.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include <memory>
#include "atto/opengl/opengl.hpp"
#include "iobuffer.hpp"
#include "rop.hpp"
using namespace atto;

/**
 * main test client
 */
int main(int argc, char const *argv[])
{
    /*
     * Setup renderer OpenGL context and initialize the GLFW library.
     */
    const int width = 800;
    const int height = 800;
    const char title[] = "Example framebuffer";
    const double timeout = 0.001;

    gl::Renderer::init(width, height, title);
    gl::Renderer::enable_event(
        gl::Event::FramebufferSize |
        gl::Event::WindowClose     |
        gl::Event::Key             |
        gl::Event::CursorPos       |
        gl::Event::MouseButton     |
        gl::Event::MouseScroll);

    /* Create drawable. */
    Rop rop("../images/monarch_512.png", 512, 512);

    /*
     * Render loop:
     *  poll events and handle
     *  update the state
     *  draw and swap buffers
     */
    while (gl::Renderer::is_open()) {
        /* Poll events and handle. */
        gl::Renderer::poll_event(timeout);
        while (gl::Renderer::has_event()) {
         gl::Event event = gl::Renderer::pop_event();

            if (event.type == gl::Event::FramebufferSize) {
                int w = event.framebuffersize.width;
                int h = event.framebuffersize.height;
                gl::Renderer::viewport({0, 0, w, h});
            }

            if ((event.type == gl::Event::WindowClose) ||
                (event.type == gl::Event::Key &&
                 event.key.code == GLFW_KEY_ESCAPE)) {
                gl::Renderer::close();
            }

            /* Handle the event. */
            rop.handle(event);
        }

        /* Draw and swap buffers. */
        gl::Renderer::clear(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
        rop.draw();
        gl::Renderer::display();
    }

    exit(EXIT_SUCCESS);
}
