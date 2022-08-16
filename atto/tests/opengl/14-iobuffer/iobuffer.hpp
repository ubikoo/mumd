/*
 * iobuffer.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ATTO_OPENGL_IOBUFFER_H_
#define TEST_ATTO_OPENGL_IOBUFFER_H_

#include "atto/opengl/opengl.hpp"

/**
 * @brief IOBuffer represents a framebuffer object with a specified size,
 * using a single color and an option depth attachments.
 */
struct IOBuffer {
    GLint m_width;
    GLint m_height;
    GLuint m_fbo;
    GLuint m_color_texture;
    GLuint m_depth_texture;

    /* Return framebuffer size */
    const GLint &width() const { return m_width; }
    const GLint &height() const { return m_height; }

    /* Return IOBuffer color and depth textures */
    const GLuint &fbo(void) const { return m_fbo; }
    const GLuint &color_texture(void) const { return m_color_texture; }
    const GLuint &depth_texture(void) const { return m_depth_texture; }

    /* Bind the framebuffer for reading/writing */
    void bind(void);
    void unbind(void);

    /* Constructor/destructor. */
    IOBuffer(
        const GLint width,
        const GLint height,
        const GLenum internal_format);
    ~IOBuffer();

    IOBuffer(const IOBuffer &) = delete;
    IOBuffer &operator=(const IOBuffer &) = delete;
};

#endif /* TEST_ATTO_OPENGL_IOBUFFER_H_ */
