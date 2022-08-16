/*
 * texture.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_TEXTURE_H_
#define ATTO_OPENGL_TEXTURE_H_

#include "atto/opengl/base.hpp"
#include "atto/opengl/imageformat.hpp"

namespace atto {
namespace gl {

/**
 * create_texture1d
 * @brief Create a 1-dimensional texture bound to GL_TEXTURE_1D target and
 * allocate the data store with a size specified by size and internal format.
 */
GLuint create_texture1d(
    const GLint internalformat,
    const GLsizei width,
    const GLenum pixelformat,
    const GLenum pixeltype,
    const GLvoid *pixels);

/**
 * create_texture2d
 * @brief Create a 2-dimensional texture bound to GL_TEXTURE_2D target and
 * allocate the data store with a size specified by size and internal format.
 */
GLuint create_texture2d(
    const GLint internalformat,
    const GLsizei width,
    const GLsizei height,
    const GLenum pixelformat,
    const GLenum pixeltype,
    const GLvoid *pixels);

/**
 * create_texture3d
 * @brief Create a 3-dimensional texture bound to GL_TEXTURE_3D target and
 * allocate the data store with a size specified by size and internal format.
 */
GLuint create_texture3d(
    const GLint internalformat,
    const GLsizei width,
    const GLsizei height,
    const GLsizei depth,
    const GLenum pixelformat,
    const GLenum pixeltype,
    const GLvoid *pixels);

/**
 * create_texture_buffer
 * @brief Create a 1-dimensional texture bound to GL_TEXTURE_BUFFER target
 * and attach the storage for the buffer object to the newly active buffer
 * texture.
 */
GLuint create_texture_buffer(GLint internalformat, GLuint buffer);

/**
 * destroy_texture
 * @brief Delete a texture object.
 */
void destroy_texture(const GLuint &texture);

/**
 * set_texture_mipmap
 * @brief Generate texture mipmaps and set the indices of the lowest
 * GL_TEXTURE_BASE_LEVEL and highest GL_TEXTURE_MAX_LEVEL mipmap levels.
 * Initially, GL_TEXTURE_BASE_LEVEL is 0 and GL_TEXTURE_MAX_LEVEL is 1000.
 */
void set_texture_mipmap(
    const GLenum target,
    const GLboolean mipmap = false,
    const GLint base_level = 0,
    const GLint max_level = 1000);

/**
 * set_texture_filter
 * @brief Set texture  minifying and magnifying functions.
 *  Initially, GL_TEXTURE_MIN_FILTER and GL_TEXTURE_MAG_FILTER are set to
 *  GL_NEAREST.
 */
void set_texture_filter(
    const GLenum target,
    const GLint filter_min = GL_NEAREST,
    const GLint filter_mag = GL_NEAREST);

/**
 * set_texture_wrap
 * @brief Set wrap parameter for the texture coordinates GL_TEXTURE_WRAP_S,
 * GL_TEXTURE_WRAP_T and GL_TEXTURE_WRAP_R.
 * Initially, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T and GL_TEXTURE_WRAP_R are
 * set to GL_REPEAT.
 */
void set_texture_wrap(
    const GLenum target,
    const GLint wrap_s = GL_REPEAT,
    const GLint wrap_t = GL_REPEAT,
    const GLint wrap_r = GL_REPEAT);

/**
 * get_texture_width
 * @brief Return the width for 0th level of detail (base image)
 * of a texture currently bound a specified target.
 */
GLsizei get_texture_width(const GLenum target);

/**
 * get_texture_height
 * @brief Return the height for 0th level of detail (base image)
 * of a texture currently bound a specified target.
 */
GLsizei get_texture_height(const GLenum target);

/**
 * get_texture_depth
 * @brief Return the depth for 0th level of detail (base image)
 * of a texture currently bound a specified target.
 */
GLsizei get_texture_depth(const GLenum target);

/**
 * get_texture_internalformat
 * @brief Return the internal format for 0th level of detail
 * (base image) of a texture currently bound a specified target.
 */
GLint get_texture_internalformat(const GLenum target);

/**
 * is_valid_texture_internalformat
 * @brief Is the texture internal format a valid GL texture format?
 */
bool is_valid_texture_internalformat(const GLint internalformat);

/**
 * active_bind_texture
 * @brief Bind the texture to the target of the specified texture unit.
 */
void active_bind_texture(GLenum target, GLenum texunit, GLuint texture);

/**
 * active_bind_texture_buffer
 * @brief Bind the texture to the target of the specified texture unit and
 * attach the buffer storage to the texture.
 */
void active_bind_texture_buffer(
    GLenum target,
    GLenum texunit,
    GLuint texture,
    GLenum internalformat,
    GLuint buffer);

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_TEXTURE_H_ */
