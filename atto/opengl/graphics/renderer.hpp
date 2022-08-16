/*
 * renderer.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GRAPHICS_RENDERER_H_
#define ATTO_OPENGL_GRAPHICS_RENDERER_H_

#include "atto/opengl/base.hpp"
#include "atto/opengl/graphics/event.hpp"
#include <queue>

namespace atto {
namespace gl {
namespace Renderer {

/** ---- Renderer window API --------------------------------------------------
 * Initialize the GLFW library and create a GLFW window.
 */
void init(
    const int width,
    const int height,
    const char *title,
    const int major = 3,
    const int minor = 3);

/** Destroy the GLFW window and terminate the GLFW library. */
void terminate(void);

/** Return a const pointer to the renderer GLFWwindow object. */
GLFWwindow *window(void);

/** Is the closed status flag of the renderer window false? */
bool is_open(void);

/** Set the closed status flag of the renderer window. */
void close(void);

/** Swap the front and back buffers of the renderer window. */
void display(void);

/** Clear OpenGL color and depth buffers. */
void clear(
    GLfloat red,
    GLfloat green,
    GLfloat blue,
    GLfloat alpha,
    GLdouble depth);

/** Get the renderer viewport position, width and height. */
std::array<GLint, 4> viewport(void);

/** Set the renderer viewport x-, y-position, width and height. */
void viewport(const std::array<GLint, 4> &viewport);

/** Get the renderer framebuffer size. */
std::array<GLint,2> framebuffer_sizei(void);

/** Get the renderer framebuffer size. */
std::array<GLfloat,2> framebuffer_sizef(void);

/** ---- Renderer event API ---------------------------------------------------
 * Does the queue have any events to be processed?
 */
bool has_event(void);

/** Poll events until the specified timeout is reached.  */
void poll_event(double timeout);

/** Add an event to the queue. */
void push_event(const Event &event);

/** Get the next event from the queue. */
Event pop_event(void);

/** Enable a collection of events in the current renderer. */
void enable_event(const GLenum mask);

/** Disable a collection of events in the current renderer. */
void disable_event(const GLenum mask);

} /* Renderer */
} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GRAPHICS_RENDERER_H_ */
