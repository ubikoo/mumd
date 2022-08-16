/*
 * buffer.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_BUFFER_H_
#define ATTO_OPENGL_BUFFER_H_

#include "atto/opengl/base.hpp"

namespace atto {
namespace gl {

/**
 * create_buffer
 * @brief Create a buffer object bound to a specified target and allocate
 * the buffer data store with a specified size in bytes.
 */
GLuint create_buffer(
    const GLenum target,
    const GLsizeiptr size,
    const GLenum usage);

/**
 * destroy_buffer
 * @brief Delete a buffer object.
 */
void destroy_buffer(const GLuint &buffer);

/**
 * get_access
 * @brief Return the access policy set while mapping the buffer object.
 */
GLenum get_buffer_access(const GLenum target);

/**
 * get_size
 * @brief Return the parameters of the buffer currently bound to the target.
 */
GLsizeiptr get_buffer_size(const GLenum target);

/**
 * get_usage
 * @brief Return the usage pattern of the buffer currently bound to the target.
 */
GLenum get_buffer_usage(const GLenum target);

/**
 * is_mapped
 * @brief Return the flag indicating whether the buffer is currently mapped.
 */
GLboolean is_buffer_mapped(const GLenum target);

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_BUFFER_H_ */
