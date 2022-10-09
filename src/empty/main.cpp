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
#include <chrono>
#include <numeric>
#include <vector>

#include "atto/opencl/opencl.hpp"
#include "model.hpp"
using namespace atto;

/**
 * main test client
 */
int main(int argc, char const *argv[])
{
    /* Setup renderer OpenGL context and initialize the GLFW library. */
    gl::Renderer::init(
        Params::window_width,
        Params::window_height,
        Params::window_title);
    gl::Renderer::enable_event(
        gl::Event::FramebufferSize |
        gl::Event::WindowClose     |
        gl::Event::Key);

    /*
     * Render loop:
     *  poll and handle events
     *  update, draw and swap buffers
     */
    Model model;
    while (gl::Renderer::is_open()) {
        /* Poll events and handle. */
        gl::Renderer::poll_event(Params::poll_timeout);
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
            model.handle(event);
        }

        {
            /* Update the model state. */
            model.execute();

            /* Draw and swap buffers. */
            gl::Renderer::clear(0.2, 0.4, 0.6, 1.0, 1.0);
            model.draw();
            gl::Renderer::display();
        }
    }

    exit(EXIT_SUCCESS);
}
