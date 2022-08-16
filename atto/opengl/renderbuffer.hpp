/*
 * renderbuffer.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_RENDERBUFFER_H_
#define ATTO_OPENGL_RENDERBUFFER_H_

#include "atto/opengl/base.hpp"
#include "atto/opengl/imageformat.hpp"

namespace atto {
namespace gl {

/**
 * create_renderbuffer
 * @brief Create a renderbuffer object and allocate the data store
 * with the specified internal format and size given by the width
 * and height dimensions.
 */
GLuint create_renderbuffer(
    const GLenum internalformat,
    const GLsizei width,
    const GLsizei height);

/**
 * destroy_renderbuffer
 * @brief Delete a renderbuffer object.
 */
void destroy_renderbuffer(const GLuint &renderbuffer);

/**
 * get_renderbuffer_width
 * @brief Return the width of a renderbuffer currently bound to the target.
 */
GLsizei get_renderbuffer_width(const GLenum target);

/**
 * get_renderbuffer_height
 * @brief Return the height of a renderbuffer currently bound to the target.
 */
GLsizei get_renderbuffer_height(const GLenum target);

/**
 * get_renderbuffer_internalformat
 * @brief Return the internal format of a renderbuffer currently
 * bound to the target.
 */
GLint get_renderbuffer_internalformat(const GLenum target);

/**
 * is_valid_renderbuffer_internalformat
 * @brief Is the renderbuffer internal format a valid GL renderbuffer format?
 */
bool is_valid_renderbuffer_internalformat(const GLint internalformat);

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_RENDERBUFFER_H_ */
