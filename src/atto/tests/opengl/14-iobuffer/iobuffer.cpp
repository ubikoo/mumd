/*
 * iobuffer.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/opengl.hpp"
#include "iobuffer.hpp"
using namespace atto;

/**
 * @brief IOBuffer represents a framebuffer object with a specified size,
 * using a single color and an option depth attachments.
 */
IOBuffer::IOBuffer(
    const GLint width,
    const GLint height,
    const GLenum internal_format)
{
    m_width = width;
    m_height = height;
    m_fbo = gl::create_framebuffer(
        m_width,
        m_height,
        1,                              /* num color attachments */
        internal_format,                /* color buffer internal format */
        &m_color_texture,
        GL_DEPTH_COMPONENT32F,          /* depth buffer internal format */
        &m_depth_texture,
        GL_NEAREST,
        GL_NEAREST);
}

/**
 * IOBuffer::~IOBuffer
 */
IOBuffer::~IOBuffer()
{
    glDeleteTextures(1, &m_color_texture);
    glDeleteTextures(1, &m_depth_texture);
    glDeleteFramebuffers(1, &m_fbo);
}

/**
 * IOBuffer::bind
 * @brief Bind the IObuffer for writing.
 */
void IOBuffer::bind(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

/**
 * IOBuffer::bind
 * @brief Unbind the IObuffer for writing.
 */
void IOBuffer::unbind(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
