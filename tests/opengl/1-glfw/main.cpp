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
    const char title[] = "Test GLFW";
    const double timeout = 0.001;

    gl::Renderer::init(width, height, title);
    gl::Renderer::enable_event(gl::Event::All);

    /* Create drawable. */
    Rop rop;

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

            if (event.type == gl::Event::FramebufferSize) {
                std::cout << core::to_string(
                    "FramebufferSize ",
                    event.framebuffersize.width, " ",
                    event.framebuffersize.height, "\n");
                int w = event.framebuffersize.width;
                int h = event.framebuffersize.height;
                gl::Renderer::viewport({0, 0, w, h});
            }

            if (event.type == gl::Event::WindowPos) {
                std::cout << core::to_string(
                    "WindowPos ",
                    event.windowpos.xpos, " ",
                    event.windowpos.ypos, "\n");
            }

            if (event.type == gl::Event::WindowSize) {
                std::cout << core::to_string(
                    "WindowSize ",
                    event.windowsize.width, " ",
                    event.windowsize.height, "\n");
            }

            if (event.type == gl::Event::WindowClose) {
                std::cout << core::to_string(
                    "WindowClose\n");
            }

            if (event.type == gl::Event::WindowMaximize) {
                std::cout << core::to_string(
                    "WindowMaximize ",
                    event.windowmaximize.iconified, "\n");
            }

            if (event.type == gl::Event::Key) {
                std::cout << core::to_string(
                    "Key ",
                    event.key.code, " ",
                    event.key.scancode, " ",
                    event.key.action, " ",
                    event.key.mods, "\n");
            }

            if (event.type == gl::Event::CursorEnter) {
                std::cout << core::to_string(
                    "CursorEnter ",
                    event.cursorenter.entered, "\n");
            }

            if (event.type == gl::Event::CursorPos) {
                std::cout << core::to_string(
                    "CursorPos ",
                    event.cursorpos.xpos, " ",
                    event.cursorpos.ypos, "\n");
            }

            if (event.type == gl::Event::MouseButton) {
                std::cout << core::to_string(
                    "MouseButton ",
                    event.mousebutton.button, " ",
                    event.mousebutton.action, " ",
                    event.mousebutton.mods, "\n");
            }

            if (event.type == gl::Event::MouseScroll) {
                std::cout << core::to_string(
                    "MouseScroll ",
                    event.mousescroll.xoffset, " ",
                    event.mousescroll.yoffset, "\n");
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
