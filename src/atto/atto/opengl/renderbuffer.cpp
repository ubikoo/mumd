/*
 * renderbuffer.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/renderbuffer.hpp"

namespace atto {
namespace gl {

/**
 * create_renderbuffer
 * @brief Create a renderbuffer object and allocate the data store
 * with the specified internal format and size given by the width
 * and height dimensions.
 *
 * @see https://www.khronos.org/opengl/wiki/Image_Format
 *      https://www.khronos.org/opengl/wiki/Texture
 *      https://www.khronos.org/opengl/wiki/Renderbuffer_Object
 *      https://learnopengl.com/Getting-started/Textures
 *      http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping
 *      http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture
 *      https://paroj.github.io/gltut/Texturing/Tutorial%2014.html
 */
GLuint create_renderbuffer(
    const GLenum internalformat,
  	const GLsizei width,
  	const GLsizei height)
{
    /* Check renderbuffer internal format and size dimensions. */
    core_assert(is_valid_renderbuffer_internalformat(internalformat),
        "invalid renderbuffer internal format");
    core_assert(width > 0, "invalid renderbuffer width");
    core_assert(height > 0, "invalid renderbuffer height");

    /*
     * Generate a new renderbuffer object and bind it to the target point.
     */
    GLuint renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    core_assert(glIsRenderbuffer(renderbuffer),
        "failed to generate renderbuffer object");

    /* Create a new data store for the renderbuffer object. */
    glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);

    /* Unbind the renderbuffer from the target point.*/
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    return renderbuffer;
}

/**
 * destroy_renderbuffer
 * @brief Delete a renderbuffer object.
 */
void destroy_renderbuffer(const GLuint &renderbuffer)
{
    glDeleteRenderbuffers(1, &renderbuffer);
}

/**
 * get_renderbuffer_width
 * @brief Return the width of a renderbuffer currently bound to the target.
 */
GLsizei get_renderbuffer_width(const GLenum target)
{
    GLsizei width;
    glGetRenderbufferParameteriv(
        target,
        GL_RENDERBUFFER_WIDTH,
        (GLint *) &width);
    return width;
}

/**
 * get_renderbuffer_height
 * @brief Return the height of a renderbuffer currently bound to the target.
 */
GLsizei get_renderbuffer_height(const GLenum target)
{
    GLsizei height;
    glGetRenderbufferParameteriv(
        target,
        GL_RENDERBUFFER_HEIGHT,
        (GLint *) &height);
    return height;
}

/**
 * get_renderbuffer_internalformat
 * @brief Return the internal format of a renderbuffer currently
 * bound to the target.
 */
GLint get_renderbuffer_internalformat(const GLenum target)
{
    GLint internalformat;
    glGetRenderbufferParameteriv(
        target,
        GL_RENDERBUFFER_INTERNAL_FORMAT,
        &internalformat);
    return internalformat;
}

/**
 * is_valid_renderbuffer_internalformat
 * @brief Is the renderbuffer internal format a valid GL renderbuffer format?
 */
bool is_valid_renderbuffer_internalformat(const GLint internalformat)
{
    if (!ImageFormat::contains(internalformat)) {
        return false;
    }

    return (
        /* Texel normalized base internal formats */
        internalformat == GL_RED                ||
        internalformat == GL_RG                 ||
        internalformat == GL_RGB                ||
        internalformat == GL_RGBA               ||
        internalformat == GL_DEPTH_COMPONENT    ||
        internalformat == GL_DEPTH_STENCIL      ||
        /* Texel 8- and 16-bit normalized sized internal formats */
        internalformat == GL_R8                 ||
        internalformat == GL_RG8                ||
        internalformat == GL_RGB8               ||
        internalformat == GL_RGBA8              ||
        internalformat == GL_R16                ||
        internalformat == GL_RG16               ||
        internalformat == GL_RGB16              ||
        internalformat == GL_RGBA16             ||
        /* Texel 16- and 32-bit floating-point sized internal formats */
        internalformat == GL_R16F               ||
        internalformat == GL_RG16F              ||
        internalformat == GL_RGB16F             ||
        internalformat == GL_RGBA16F            ||
        internalformat == GL_R32F               ||
        internalformat == GL_RG32F              ||
        internalformat == GL_RGB32F             ||
        internalformat == GL_RGBA32F            ||
        /* Texel 8-, 16-, and 32-bit integer sized internal formats */
        internalformat == GL_R8I                ||
        internalformat == GL_RG8I               ||
        internalformat == GL_RGB8I              ||
        internalformat == GL_RGBA8I             ||
        internalformat == GL_R16I               ||
        internalformat == GL_RG16I              ||
        internalformat == GL_RGB16I             ||
        internalformat == GL_RGBA16I            ||
        internalformat == GL_R8UI               ||
        internalformat == GL_RG8UI              ||
        internalformat == GL_RGB8UI             ||
        internalformat == GL_RGBA8UI            ||
        internalformat == GL_R16UI              ||
        internalformat == GL_RG16UI             ||
        internalformat == GL_RGB16UI            ||
        internalformat == GL_RGBA16UI           ||
        /* Texel depth sized internal formats */
        internalformat == GL_DEPTH_COMPONENT16  ||
        internalformat == GL_DEPTH_COMPONENT24  ||
        internalformat == GL_DEPTH_COMPONENT32  ||
        internalformat == GL_DEPTH_COMPONENT32F ||
        internalformat == GL_DEPTH24_STENCIL8);
}

} /* gl */
} /* atto */
