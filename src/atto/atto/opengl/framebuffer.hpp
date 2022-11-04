/*
 * framebuffer.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_FRAMEBUFFER_H_
#define ATTO_OPENGL_FRAMEBUFFER_H_

#include "atto/opengl/base.hpp"
#include "atto/opengl/imageformat.hpp"
#include "atto/opengl/renderbuffer.hpp"
#include "atto/opengl/texture.hpp"

namespace atto {
namespace gl {

/**
 * create_framebuffer
 * @brief Create a framebuffer object with size width and height, using
 * textures as rendering surfaces for the color and depth attachments.
 */
GLuint create_framebuffer(
    const GLsizei width,
    const GLsizei height,
    const GLsizei n_color_attachments,
    const GLenum color_internalformat,
    GLuint *color_textures,
    const GLenum depth_internalformat = GL_DEPTH_COMPONENT,
    GLuint *depth_texture = nullptr,
    const GLint filter_min = GL_NEAREST,
    const GLint filter_mag = GL_NEAREST);

/**
 * create_framebuffer_texture
 * @brief Create a framebuffer object with size width and height, using
 * textures as rendering surfaces and no depth attachment.
 */
GLuint create_framebuffer_texture(
    const GLsizei width,
    const GLsizei height,
    const GLsizei n_color_attachments,
    const GLenum color_internalformat,
    GLuint *color_textures,
    const GLint filter_min = GL_NEAREST,
    const GLint filter_mag = GL_NEAREST);

/**
 * create_framebuffer_depth
 * @brief Create a framebuffer object with size width and height, using
 * only the depth attachment. The depth texture stores the depth buffer
 * which can be used for depth maps, etc.
 */
GLuint create_framebuffer_depth(
    const GLsizei width,
    const GLsizei height,
    const GLenum depth_internalformat,
    GLuint *depth_texture,
    const GLint filter_min = GL_NEAREST,
    const GLint filter_mag = GL_NEAREST);

/**
 * create_framebuffer_renderbuffer
 * @brief Create a framebuffer object with size width and height, using
 * renderbuffers as rendering surfaces for the color and depth attachments.
 */
GLuint create_framebuffer_renderbuffer(
    const GLsizei width,
    const GLsizei height,
    const GLsizei n_color_attachments,
    const GLenum color_internalformat,
    GLuint *color_renderbuffers,
    const GLenum depth_internalformat = GL_DEPTH24_STENCIL8,
    GLuint *depth_renderbuffer = nullptr);

/**
 * destroy_framebuffer
 * @brief Delete a framebuffer object.
 */
void destroy_framebuffer(const GLuint &framebuffer);

/**
 * get_framebuffer_max_color_attachments
 * @brief Return the maximum number of color attachment points in a
 * framebuffer object. The value must be at least 4.
 * @see glFramebufferRenderbuffer and glFramebufferTexture2D.
 */
GLint get_framebuffer_max_color_attachments(void);

/**
 * get_framebuffer_max_draw_buffers
 * @brief Return the maximum number of simultaneous outputs that may
 * be written in a fragment shader. The value must be at least 4.
 * @see glDrawBuffers.
 */
GLint get_framebuffer_max_draw_buffers(void);

/**
 * is_valid_framebuffer_color_internalformat
 * @brief Is format a valid color-renderable internal format?
 */
bool is_valid_framebuffer_color_internalformat(const GLenum internalformat);

/**
 * is_valid_framebuffer_depth_internalformat
 * @brief Is format a valid depth-renderable internal format?
 */
bool is_valid_framebuffer_depth_internalformat(const GLenum internalformat);

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_FRAMEBUFFER_H_ */
