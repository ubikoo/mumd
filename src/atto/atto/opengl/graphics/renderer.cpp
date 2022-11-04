/*
 * renderer.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/graphics/renderer.hpp"

namespace atto {
namespace gl {
namespace Renderer {

/** ---- Renderer window API --------------------------------------------------
 * @brief Renderer is an interface to a GLFWwindow object, its associated
 * OpenGL context and a queue of events.
 *
 * Both GLFWwindow and OpenGL context are closely linked and the object pointer
 * is used as both a context and window handle.
 * The Renderer is responsible to initialize and to terminate the GLFW library.
 * Its purpose is to to provide a simple means of issuing OpenGL commands and
 * handling GLFW events.
 *
 * @see https://stackoverflow.com/questions/29617370
 *      https://stackoverflow.com/questions/35683334
 *      https://stackoverflow.com/questions/7676971
 *      https://discourse.glfw.org/t/what-is-a-possible-use-of-glfwgetwindowuserpointer/1294
 *      https://isocpp.org/wiki/faq/pointers-to-members#memfnptr-vs-fnptr
 */
static GLFWwindow *g_window = nullptr;
static std::queue<Event> g_event_queue;
#include "atto/opengl/graphics/renderer.inc"

/**
 * init
 * @brief Initialize the GLFW library and create a GLFW window.
 */
void init(
    const int width,
    const int height,
    const char *title,
    const int major,
    const int minor)
{
    core_assert(g_window == nullptr, "GLFW library already initialized");
    core_assert(width > 0 && height > 0, "invalid window dimensions");
    core_assert(title != nullptr, "invalid window title");
    core_assert(major >= 3, "client API major version number < 3");
    core_assert(minor >= 3, "client API minor version number < 3");

    /*
     * Initialize the GLFW library.
     */
    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() != GLFW_TRUE) {
        core_throw("failed to initialise GLFW library");
    }

    /*
     * Set OpenGL context and renderer related hints.
     * macOS only supports forward-compatible core profile contexts for
     * OpenGL versons 3.2 and later.
     * Before creating an OpenGL context(at renderer creation), hints
     * GLFW_OPENGL_FORWARD_COMPAT and GLFW_OPENGL_PROFILE must be set
     * accordingly.
     * A forward-compatible OpenGL context is one where all functionality
     * deprecated in the requested version of OpenGL is removed.
     */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /*
     * Create a new GLFWwindow object.
     */
    GLFWmonitor *monitor = nullptr; /* run in windowed mode */
    GLFWwindow *share = nullptr;    /* do not share the OpenGL context */
    g_window = glfwCreateWindow(width, height, title, monitor, share);
    if (g_window == nullptr) {
        glfwTerminate();
        core_throw("failed to create GLFWwindow");
    }

    /*
     * Make current the OpenGL context of the newly created window and
     * load the OpenGL function pointers using the context provided by
     * the GLFW loader function.
     * The watertight method is to call gladLoadGLLoader after each and
     * every context change.
     */
    glfwMakeContextCurrent(g_window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        glfwTerminate();
        core_throw("failed to initialise glad");
    }

    /*
     * Set the buffer swap interval to a single monitor refresh
     * between each buffer swap to synchronize buffer swap with
     * the monitor refresh rate.
     */
    glfwSwapInterval(1);

    /*
     * Set OpenGL viewport defined as the affine transform from
     * normalized device coordinates to screen coordinate space.
     */
    int w, h;
    glfwGetFramebufferSize(g_window, &w, &h);
    glViewport(0, 0, w, h);

    /*
     * Return a string with the following format:
     *  the version of GLFW,
     *  the name of the renderer system API,
     *  the name of the context creation API,
     *  any additional options or APIs.
     */
    std::cout << core::str_format(
        "GLFW version: %s\nOpenGL renderer: %s\nOpenGL version: %s\n",
        glfwGetVersionString(),
        glGetString(GL_RENDERER),
        glGetString(GL_VERSION));
}

/**
 * terminate
 * @brief Destroy the renderer and terminate the GLFW library.
 */
void terminate(void)
{
    glfwDestroyWindow(g_window);
    glfwTerminate();
}

/**
 * window
 * @brief Return a const pointer to the renderer GLFWwindow object.
 */
GLFWwindow *window(void)
{
    return g_window;
}

/**
 * is_open
 * @brief Is the closed status flag of the renderer window false?
 */
bool is_open(void)
{
    return (glfwWindowShouldClose(g_window) == GLFW_FALSE);
}

/**
 * close
 * @brief Set the closed status flag of the renderer window.
 */
void close(void)
{
    glfwSetWindowShouldClose(g_window, GLFW_TRUE);
}

/**
 * display
 * @brief Swap the front and back buffers of the renderer window.
 * If the swap interval is greater than zero, the GPU driver waits
 * the specified number of screen updates before swapping the buffers.
 */
void display(void)
{
    glfwSwapBuffers(g_window);
}

/**
 * clear
 * @brief Clear OpenGL color and depth buffers using the specified
 * red, green, blue, alpha and depth values.
 */
void clear(
    GLfloat red,
    GLfloat green,
    GLfloat blue,
    GLfloat alpha,
    GLdouble depth)
{
    glClearColor(red, green, blue, alpha);
    glClearDepth(depth);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Get the renderer viewport position, width and height.
 */
std::array<GLint, 4> viewport(void)
{
    std::array<GLint, 4> viewport;
    glGetIntegerv(GL_VIEWPORT, viewport.data());
    return viewport;
}

/**
 * viewport
 * @brief Set the renderer viewport x-position, y-position, width and height.
 * @param viewport: array specifying the elements (x, y, w, h).
 * x and y specify the lower left corner of the viewport rectangle; initial
 * value is (0,0).
 * w and h specify the rectangle size; initial value is the window size.
 */
void viewport(const std::array<GLint, 4> &viewport)
{
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

/**
 * framebuffer_sizei
 * @brief Get the renderer framebuffer size.
 */
std::array<GLint,2> framebuffer_sizei(void)
{
    int i_width, i_height;
    glfwGetFramebufferSize(g_window, &i_width, &i_height);
    return std::array<GLint,2>{i_width, i_height};
}

/**
 * framebuffer_sizef
 * @brief Get the renderer framebuffer size.
 */
std::array<GLfloat,2> framebuffer_sizef(void)
{
    std::array<GLint,2> sizei = framebuffer_sizei();
    return std::array<GLfloat,2>{(GLfloat) sizei[0], (GLfloat) sizei[1]};
}

/** ---- Renderer event API ---------------------------------------------------
 * has_event
 * @brief Does the queue have any events to be processed?
 */
bool has_event(void)
{
    return !g_event_queue.empty();
}

/**
 * poll_event
 * @brief Poll events until the specified timeout is reached.
 * The timeout value must be a positive finite number.
 */
void poll_event(double timeout)
{
    glfwWaitEventsTimeout(std::max(0.0, timeout));
}

/**
 * push_event
 * @brief Add an event to the renderer event queue.
 */
void push_event(const Event &event)
{
    core_assert(event.type & Event::All, "invalid event type");
    g_event_queue.push(event);
}

/**
 * pop_event
 * @brief Get the top event from the renderer event queue.
 */
Event pop_event(void)
{
    core_assert(has_event(), "empty event queue");
    Event top = g_event_queue.front();
    g_event_queue.pop();
    return top;
}

/**
 * @brief Define the renderer event callback static functions.
 * For each event enabled in the renderer, there is an associated static
 * callback function. The callback is responsible to store the event data
 * in the renderer event queue. Two types of events are recognised here:
 *  - window events(position, size, etc)
 *  - input events(key, cursor movement, mouse buttons, etc)
 *
 * @note Event processing is normally done each frame after buffer swapping.
 * This step is performed even in the absence of windows, in order to receive
 * monitor connection events.
 */

/**
 * @brief Define GLFW library version macros
 *  glfw_version_hash       Hash version macro creator
 *  glfw_version_min        Minimum version comparison
 *  glfw_version_error      GLFW version error
 */
#ifndef glfw_version_hash
#define glfw_version_hash(major, minor) (((major)<<8) |(minor))
#endif /* glfw_version_hash */

#ifndef glfw_version_min
#define glfw_version_min(major, minor) \
        glfw_version_hash(GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR) >= \
        glfw_version_hash((major), (minor))
#endif /* glfw_version_min */

#ifndef glfw_version_error
#define glfw_version_error(major, minor) \
do { \
    std::cout << core::str_format( \
        "need GLFW version >= %d.%d: %s\n", \
       (major), (minor), glfwGetVersionString()); \
} while(0)
#endif /* glfw_version_error */

/**
 * enable_event
 * @brief Enable a collection of events in the renderer.
 * @param mask bitwise OR of event types to be enabled.
 */
void enable_event(const GLenum mask)
{
    core_assert(mask & Event::All, "invalid event type");

    /* Set FramebufferSize callback. */
    if (mask & Event::FramebufferSize) {
#if glfw_version_min(3,0)
        glfwSetFramebufferSizeCallback(g_window, glfw_frambuffer_size_callback);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Set WindowPos callback. */
    if (mask & Event::WindowPos) {
#if glfw_version_min(3,0)
        glfwSetWindowPosCallback(g_window, glfw_window_pos_callback);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Set WindowSize callback. */
    if (mask & Event::WindowSize) {
#if glfw_version_min(3,0)
        glfwSetWindowSizeCallback(g_window, glfw_window_size_callback);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Set WindowClose callback. */
    if (mask & Event::WindowClose) {
#if glfw_version_min(3,0)
        glfwSetWindowCloseCallback(g_window, glfw_window_close_callback);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Set WindowMaximize callback. */
    if (mask & Event::WindowMaximize) {
#if glfw_version_min(3,3)
        glfwSetWindowMaximizeCallback(g_window, glfw_window_maximize_callback);
#else
        glfw_version_error(3,3);
#endif
    }

    /* Set Key callback. */
    if (mask & Event::Key) {
#if glfw_version_min(3,0)
        glfwSetKeyCallback(g_window, glfw_key_callback);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Set CursorEnter callback. */
    if (mask & Event::CursorEnter) {
#if glfw_version_min(3,0)
        glfwSetCursorEnterCallback(g_window, glfw_cursor_enter_callback);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Set CursorPos callback. */
    if (mask & Event::CursorPos) {
#if glfw_version_min(3,0)
        glfwSetCursorPosCallback(g_window, glfw_cursor_pos_callback);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Set MouseButton callback. */
    if (mask & Event::MouseButton) {
#if glfw_version_min(3,0)
        glfwSetMouseButtonCallback(g_window, glfw_mouse_button_callback);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Set MouseScroll callback. */
    if (mask & Event::MouseScroll) {
#if glfw_version_min(3,0)
        glfwSetScrollCallback(g_window, glfw_mouse_scroll_callback);
#else
        glfw_version_error(3,0);
#endif
    }
}

/**
 * disable_event
 * @brief Disable a collection of events in the renderer.
 * @param mask bitwise OR of event types to be disabled.
 */
void disable_event(const GLenum mask)
{
    core_assert(mask & Event::All, "invalid event type");

    /* Unset FramebufferSize callback. */
    if (mask & Event::FramebufferSize) {
#if glfw_version_min(3,0)
        glfwSetFramebufferSizeCallback(g_window, nullptr);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Unset WindowPos callback. */
    if (mask & Event::WindowPos) {
#if glfw_version_min(3,0)
        glfwSetWindowPosCallback(g_window, nullptr);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Unset WindowSize callback. */
    if (mask & Event::WindowSize) {
#if glfw_version_min(3,0)
        glfwSetWindowSizeCallback(g_window, nullptr);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Unset WindowClose callback. */
    if (mask & Event::WindowClose) {
#if glfw_version_min(3,0)
        glfwSetWindowCloseCallback(g_window, nullptr);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Unset WindowMaximize callback. */
    if (mask & Event::WindowMaximize) {
#if glfw_version_min(3,3)
        glfwSetWindowMaximizeCallback(g_window, nullptr);
#else
        glfw_version_error(3,3);
#endif
    }

    /* Unset Key callback. */
    if (mask & Event::Key) {
#if glfw_version_min(3,0)
        glfwSetKeyCallback(g_window, nullptr);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Unset CursorEnter callback. */
    if (mask & Event::CursorEnter) {
#if glfw_version_min(3,0)
        glfwSetCursorEnterCallback(g_window, nullptr);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Unset CursorPos callback. */
    if (mask & Event::CursorPos) {
#if glfw_version_min(3,0)
        glfwSetCursorPosCallback(g_window, nullptr);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Unset MouseButton callback. */
    if (mask & Event::MouseButton) {
#if glfw_version_min(3,0)
        glfwSetMouseButtonCallback(g_window, nullptr);
#else
        glfw_version_error(3,0);
#endif
    }

    /* Unset MouseScroll callback. */
    if (mask & Event::MouseScroll) {
#if glfw_version_min(3,0)
        glfwSetScrollCallback(g_window, nullptr);
#else
        glfw_version_error(3,0);
#endif
    }
}

/**
 * @brief Undefine GLFW library version macros
 *  glfw_version_hash      Hash version macro creator
 *  glfw_version_min       Minimum version comparison
 *  glfw_version_error     GLFW version error
 */
#ifdef glfw_version_hash
#undef glfw_version_hash
#endif

#ifdef glfw_version_min
#undef glfw_version_min
#endif

#ifdef glfw_version_error
#undef glfw_version_error
#endif

} /* Renderer */
} /* gl */
} /* atto */
