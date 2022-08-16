/*
 * renderer.inc
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

/** ---- GLFW renderer event interface functions ------------------------------
 * glfw_error_callback
 * @brief Error callback function:
 *      glfwSetErrorCallback (GLFWerrorfun cbfun)
 *      typedef void(* GLFWerrorfun) (int, const char *);
 */
static void glfw_error_callback (int code, const char *desc)
{
    std::cerr << core::str_format("code: %d, description: %s\n", code, desc);
}

/**
 * glfw_frambuffer_size_callback
 * @brief Renderer framebuffer resize callback function:
 *      glfwSetFramebufferSizeCallback(GLFWwindow *window,
 *                                      GLFWframebuffersizefun cbfun)
 *      typedef void(* GLFWframebuffersizefun)(GLFWwindow *, int, int)
 */
static void glfw_frambuffer_size_callback (
    GLFWwindow *window,
    int width,
    int height)
{
    Event event(Event::FramebufferSize);
    event.framebuffersize = {width, height};
    g_event_queue.push(event);
}

/**
 * glfw_window_pos_callback
 * @brief Renderer position callback function:
 *      glfwSetWindowPosCallback(GLFWwindow *window, GLFWwindowposfun cbfun)
 *      typedef void(* GLFWwindowposfun)(GLFWwindow *, int, int)
 */
static void glfw_window_pos_callback(GLFWwindow *window, int xpos, int ypos)
{
    Event event(Event::WindowPos);
    event.windowpos = {xpos, ypos};
    g_event_queue.push(event);
}

/**
 * glfw_window_size_callback
 * @brief Renderer resize callback function:
 *      glfwSetWindowSizeCallback(GLFWwindow *window, GLFWwindowsizefun cbfun)
 *      typedef void(* GLFWwindowsizefun)(GLFWwindow *, int, int)
 */
static void glfw_window_size_callback (
    GLFWwindow *window,
    int width,
    int height)
{
    Event event(Event::WindowSize);
    event.windowsize = {width, height};
    g_event_queue.push(event);
}

/**
 * glfw_window_close_callback
 * @brief Renderer close callback function:
 *      glfwSetWindowCloseCallback(GLFWwindow *window, GLFWwindowclosefun cbfun)
 *      typedef void(* GLFWwindowclosefun)(GLFWwindow *)
 */
static void glfw_window_close_callback(GLFWwindow *window)
{
    Event event(Event::WindowClose);
    g_event_queue.push(event);
}

/**
 * glfw_window_maximize_callback
 * @brief Renderer maximize callback function:
 *      glfwSetWindowMaximizeCallback(GLFWwindow *window,
 *                                     GLFWwindowmaximizefun cbfun)
 *      typedef void(* GLFWwindowmaximizefun)(GLFWwindow *, int)
 */
static void glfw_window_maximize_callback(GLFWwindow *window, int iconified)
{
    Event event(Event::WindowMaximize);
    event.windowmaximize = {iconified};
    g_event_queue.push(event);
}

/**
 * glfw_key_callback
 * @brief Key callback function:
 *      glfwSetKeyCallback(GLFWwindow *window, GLFWkeyfun cbfun)
 *      typedef void(* GLFWkeyfun)(GLFWwindow *, int, int, int, int)
 */
static void glfw_key_callback (
    GLFWwindow *window,
    int code,
    int scancode,
    int action,
    int mods)
{
    Event event(Event::Key);
    event.key = {code, scancode, action, mods};
    g_event_queue.push(event);
}

/**
 * glfw_cursor_enter_callback
 * @brief Cursor enter callback function:
 *      glfwSetCursorEnterCallback(GLFWwindow *window,
 *                                  GLFWcursorenterfun cbfun)
 *      typedef void(* GLFWcursorenterfun)(GLFWwindow *, int)
 */
static void glfw_cursor_enter_callback(GLFWwindow *window, int entered)
{
    Event event(Event::CursorEnter);
    event.cursorenter = {entered};
    g_event_queue.push(event);
}

/**
 * glfw_cursor_pos_callback
 * @brief Cursor position callback function:
 *      glfwSetCursorPosCallback(GLFWwindow *window, GLFWcursorposfun cbfun)
 *      typedef void(* GLFWcursorposfun)(GLFWwindow *, double, double)
 */
static void glfw_cursor_pos_callback (
    GLFWwindow *window,
    double xpos,
    double ypos)
{
    Event event(Event::CursorPos);
    event.cursorpos = {xpos, ypos};
    g_event_queue.push(event);
}

/**
 * glfw_mouse_button_callback
 * @brief Mouse button callback function:
 *      glfwSetMouseButtonCallback(GLFWwindow *window,
 *                                 GLFWmousebuttonfun cbfun)
 *      typedef void(* GLFWmousebuttonfun)(GLFWwindow *, int, int, int)
 */
static void glfw_mouse_button_callback (
    GLFWwindow *window,
    int button,
    int action,
    int mods)
{
    Event event(Event::MouseButton);
    event.mousebutton = {button, action, mods};
    g_event_queue.push(event);
}

/**
 * glfw_mouse_scroll_callback
 * @brief Mouse scroll callback function:
 *      glfwSetScrollCallback(GLFWwindow *window, GLFWscrollfun cbfun)
 *      typedef void(* GLFWscrollfun)(GLFWwindow *, double, double)
 */
static void glfw_mouse_scroll_callback (
    GLFWwindow *window,
    double xoffset,
    double yoffset)
{
    Event event(Event::MouseScroll);
    event.mousescroll = {xoffset, yoffset};
    g_event_queue.push(event);
}
