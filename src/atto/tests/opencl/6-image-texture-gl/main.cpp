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
#include <vector>
#include <chrono>
#include <numeric>

#include "../base.hpp"
#include "model.hpp"
using namespace atto;

/**
 * main test client
 */
int main(int argc, char const *argv[])
{
    /*
     * Setup renderer OpenGL context and initialize the GLFW library.
     */
    const int WINDOW_WIDTH = 512;
    const int WINDOW_HEIGHT = 512;
    const char WINDOW_TITLE[] = "CL/GL interop";
    const double POLL_TIMEOUT = 0.01;

    gl::Renderer::init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    gl::Renderer::enable_event(
        gl::Event::FramebufferSize |
        gl::Event::WindowClose     |
        gl::Event::Key);

    /*
     * Create model object.
     */
    Model model;

    /*
     * Render loop:
     *  poll events and handle
     *  update the state
     *  draw and swap buffers
     */
    while (gl::Renderer::is_open()) {
        /* Poll events and handle. */
        gl::Renderer::poll_event(POLL_TIMEOUT);
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

        /* Execute the model and draw. */
        auto tic = std::chrono::high_resolution_clock::now();
        model.execute();
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::printf("elapsed: %lf msec\n", msec.count());
        gl::Renderer::clear(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
        model.draw();
        gl::Renderer::display();
    }

    exit(EXIT_SUCCESS);
}
