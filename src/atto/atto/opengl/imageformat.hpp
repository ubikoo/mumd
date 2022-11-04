/*
 * imageformat.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_IMAGEFORMAT_H_
#define ATTO_OPENGL_IMAGEFORMAT_H_

#include "atto/opengl/base.hpp"

namespace atto {
namespace gl {
namespace ImageFormat {

/**
 * ImageFormat::contains
 * @brief Does the map contain the key of the GL image format?
 */
bool contains(const GLint internalformat);

/**
 * ImageFormat::baseformat
 * @brief Return the pixel base format of the specified GL image format.
 */
GLenum baseformat(const GLint internalformat);

/**
 * ImageFormat::datatype
 * @brief Return the pixel data type of the specified GL image format.
 */
GLenum datatype(const GLint internalformat);

/**
 * ImageFormat::datasize
 * @brief Return the size in bytes of the specified GL image format.
 */
GLsizei datasize(const GLint internalformat);

/**
 * ImageFormat::n_components
 * @brief Return the number of components of the specified GL image format.
 */
GLsizei n_components(const GLint internalformat);

} /* ImageFormat */
} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_IMAGEFORMAT_H_ */
