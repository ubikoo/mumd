/*
 * error.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_ERROR_H_
#define ATTO_OPENGL_ERROR_H_

#include "atto/opengl/base.hpp"

namespace atto {
namespace gl {
namespace Error {

/**
 * Error::name
 * @brief Return the error name.
 * Error is a nmespace-scoped set of OpenGL error handling functions.
 * @see https://www.khronos.org/opengl/wiki/Example_Code
 *      https://www.khronos.org/opengl/wiki/OpenGL_Error
 */
core_inline
std::string name(const GLenum error) {
    switch (error) {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
        break;
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
        break;
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
        break;
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
        break;
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
        break;
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}

/**
 * Error::clear
 * @brief Clear the OpenGL error queue.
 */
core_inline
void clear(void)
{
    while (glGetError() != GL_NO_ERROR);
}

/**
 * Error::check
 * @brief Check the OpenGL error queue.
 */
core_inline
void check(const char *message)
{
    /* Get the error information and log error message */
    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
        std::cerr << core::str_format(
            "OpenGL error: %s (%d), %s ", name(error).c_str(), error, message);
        error = glGetError();
    }
}

} /* Error */
} /* gl */
} /* atto */

/**
 * gl_debug(func)
 * @brief OpenGL call macro with error checking.
 * @note Enclose the compound statement inside a do {...} while(0)
 * to swallow the semicolon and the macro in one statement.
 */
#ifndef gl_debug
#define gl_debug(func) \
do { \
    atto::gl::Error::clear(); \
    (func); \
    atto::gl::Error::check(core_strify(func)); \
} while(0)
#endif /* gl_debug */

#endif /* ATTO_OPENGL_ERROR_H_ */
