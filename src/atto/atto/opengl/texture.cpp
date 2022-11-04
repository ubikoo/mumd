/*
 * texture.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/texture.hpp"

namespace atto {
namespace gl {

/**
 * create_texture1d
 * @brief Create a 1-dimensional texture bound to GL_TEXTURE_1D target and
 * allocate the data store with a size specified by size and internal format.
 *
 *  - Texture wrap parameters
 *  GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, and GL_TEXTURE_WRAP_R parameters
 *  control the way that texture coordinates outside the range [0.0, 1.0].
 *  The clamping mode in each dimension may be set to one of:
 *      GL_CLAMP_TO_EDGE,
 *      GL_CLAMP_TO_BORDER,
 *      GL_REPEAT,
 *      GL_MIRRORED_REPEAT.
 *
 *  - Texture minification and manification parameters
 *  Textures have the concept of completeness. A texture is not mipmap
 *  complete if it expects the existence of mipmaps and none is supplied.
 *  There are six minifying functions. Two use either the nearest texture
 *  elements or a weighted average of multiple texture elements to compute
 *  the texture value. The other four use mipmaps.
 *      - GL_NEAREST
 *      Nearest texture element(in Manhattan distance) to the texture
 *      coordinates.
 *
 *      - GL_LINEAR
 *      Bilinear weighted average of the four closest texture elements
 *      to the texture coordinates. These can include items wrapped or
 *      repeated from other parts of a texture, depending on the values
 *      of GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R.
 *
 *      - GL_NEAREST_MIPMAP_NEAREST
 *      Choose the mipmap that most closely matches the pixel size and
 *      use the GL_NEAREST criterion.
 *
 *      - GL_LINEAR_MIPMAP_NEAREST
 *      Choose the mipmap that most closely matches the pixel size and
 *      use the GL_LINEAR criterion.
 *
 *      - GL_NEAREST_MIPMAP_LINEAR
 *      Interpolate between the two mipmaps that most closely math the
 *      pixel size and use the GL_NEAREST criterion.
 *
 *      - GL_LINEAR_MIPMAP_LINEAR
 *      Interpolate between the two mipmaps that most closely math the
 *      pixel size and use the GL_LINEAR criterion.
 *
 * The default GL_TEXTURE_MIN_FILTER state is GL_NEAREST_MIPMAP_LINEAR.
 * Because the default GL_TEXTURE_MAX_LEVEL is set to 1000, by default
 * OpenGL expects mipmap levels to be defined.
 * Instead of generating mipmaps by default, set GL_TEXTURE_MIN_FILTER
 * to be GL_NEAREST and give the option of generating mipmaps or not.
 *
 * @note A texture is an OpenGL Object that contains one or more images
 * that all have the same image format. A texture can be used in two ways:
 *  - Source of a texture access from a shader
 *  - Render target
 * The texture has specific constraints on the images it can contain.
 * There are three defining characteristics of a texture, each of them
 * defining part of those constraints:
 *  - texture type:     arrangement of images within the texture
 *  - texture size:     size of the images in the texture
 *  - image format:     format that all of these images share
 *
 * There are a number of different types of textures. The three types of
 * textures use here are:
 *  - GL_TEXTURE_1D:    1-dimensional texture images. They have width,
 *                      but no height or depth.
 *  - GL_TEXTURE_2D:    2-dimensional texture images. They have width
 *                      and height but no depth.
 *  - GL_TEXTURE_3D:    3-dimensional texture images. They have width,
 *                      height and depth.
 *
 * @see https://www.khronos.org/opengl/wiki/Image_Format
 *      https://www.khronos.org/opengl/wiki/Texture
 *      https://www.khronos.org/opengl/wiki/Common_Mistakes
 *      https://learnopengl.com/Getting-started/Textures
 *      http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping
 *      http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture
 *      https://paroj.github.io/gltut/Texturing/Tutorial%2014.html
 */
GLuint create_texture1d(
    const GLint internalformat,
    const GLsizei width,
    const GLenum pixelformat,
    const GLenum pixeltype,
    const GLvoid *pixels)
{
    /* Check texture internal format and size dimensions. */
    core_assert(is_valid_texture_internalformat(internalformat),
        "invalid texture internal format");
    core_assert(width > 0, "invalid texture width");

    /*
     * Generate a new texture object name and bind it to the target point.
     * No texture objects are associated with the name set by glGenTextures
     * until they are bound bye a call to glBindTexture.
     */
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_1D, texture);
    core_assert(glIsTexture(texture), "failed to generate texture object");

    /*
     * Specifiy the texture data store with the pixel data. If pixels
     * data pointer is null, glTexImage1D() allocates the memory only.
     */
    glTexImage1D(
        GL_TEXTURE_1D,
        0,                  /* level of detail - 0 is base image */
        internalformat,     /* texture internal format */
        width,              /* texture width */
        0,                  /* border parameter - must be 0 (legacy) */
        pixelformat,        /* format of the pixel data */
        pixeltype,          /* type of the pixel data */
        pixels);            /* pointer to the pixel data */

    /*
     * Unbind the texture from the target point.
     */
    glBindTexture(GL_TEXTURE_1D, 0);
    return texture;
}

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
    const GLvoid *pixels)
{
    /* Check texture internal format and size dimensions. */
    core_assert(is_valid_texture_internalformat(internalformat),
        "invalid texture internal format");
    core_assert(width > 0, "invalid texture width");
    core_assert(height > 0, "invalid texture height");

    /*
     * Generate a new texture object name and bind it to the target point.
     * No texture objects are associated with the name set by glGenTextures
     * until they are bound bye a call to glBindTexture.
     */
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    core_assert(glIsTexture(texture), "failed to generate texture object");

    /*
     * Specifiy the texture data store with the pixel data. If pixels
     * data pointer is null, glTexImage2D() allocates the memory only.
     */
    glTexImage2D(
        GL_TEXTURE_2D,
        0,                  /* level of detail - 0 is base image */
        internalformat,     /* texture internal format */
        width,              /* texture width */
        height,             /* texture height */
        0,                  /* border parameter - must be 0 (legacy) */
        pixelformat,        /* format of the pixel data */
        pixeltype,          /* type of the pixel data(GLubyte) */
        pixels);            /* pointer to the pixel data */

    /*
     * Unbind the texture from the target point.
     */
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

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
    const GLvoid *pixels)
{
    /* Check texture internal format and size dimensions. */
    core_assert(is_valid_texture_internalformat(internalformat),
        "invalid texture internal format");
    core_assert(width > 0, "invalid texture width");
    core_assert(height > 0, "invalid texture height");
    core_assert(depth > 0, "invalid texture depth");

    /*
     * Generate a new texture object name and bind it to the target point.
     * No texture objects are associated with the name set by glGenTextures
     * until they are bound bye a call to glBindTexture.
     */
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);
    core_assert(glIsTexture(texture), "failed to generate texture object");

    /*
     * Specifiy the texture data store with the pixel data. If pixels
     * data pointer is null, glTexImage3D() allocates the memory only.
     */
    glTexImage3D(
        GL_TEXTURE_3D,
        0,                  /* level of detail - 0 is base image */
        internalformat,     /* texture internal format */
        width,              /* texture width */
        height,             /* texture height */
        depth,              /* texture depth */
        0,                  /* border parameter - must be 0 (legacy) */
        pixelformat,        /* format of the pixel data */
        pixeltype,          /* type of the pixel data(GLubyte) */
        pixels);            /* pointer to the pixel data */

    /*
     * Unbind the texture from the target point and return.
     */
    glBindTexture(GL_TEXTURE_3D, 0);
    return texture;
}

/**
 * create_texture_buffer
 * @brief Create a 1-dimensional texture bound to GL_TEXTURE_BUFFER target
 * and attach the storage for the buffer object to the newly active buffer
 * texture.
 * If buffer is zero, any buffer object attached to the buffer texture is
 * detached and no new buffer object is attached. If buffer is non-zero,
 * it must be the name of an existing buffer object.
 */
GLuint create_texture_buffer(GLint internalformat, GLuint buffer)
{
    core_assert(
        internalformat == GL_R8       ||
        internalformat == GL_R16      ||
        internalformat == GL_R16F     ||
        internalformat == GL_R32F     ||
        internalformat == GL_R8I      ||
        internalformat == GL_R16I     ||
        internalformat == GL_R32I     ||
        internalformat == GL_R8UI     ||
        internalformat == GL_R16UI    ||
        internalformat == GL_R32UI    ||
        internalformat == GL_RG8      ||
        internalformat == GL_RG16     ||
        internalformat == GL_RG16F    ||
        internalformat == GL_RG32F    ||
        internalformat == GL_RG8I     ||
        internalformat == GL_RG16I    ||
        internalformat == GL_RG32I    ||
        internalformat == GL_RG8UI    ||
        internalformat == GL_RG16UI   ||
        internalformat == GL_RG32UI   ||
        internalformat == GL_RGBA8    ||
        internalformat == GL_RGBA16   ||
        internalformat == GL_RGBA16F  ||
        internalformat == GL_RGBA32F  ||
        internalformat == GL_RGBA8I   ||
        internalformat == GL_RGBA16I  ||
        internalformat == GL_RGBA32I  ||
        internalformat == GL_RGBA8UI  ||
        internalformat == GL_RGBA16UI ||
        internalformat == GL_RGBA32UI,
        "invalid texture buffer internal format");
    core_assert(glIsBuffer(buffer), "invalid texture buffer object");

    /*
     * Generate a new texture object name and bind it to GL_TEXTURE_BUFFER.
     */
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_BUFFER, texture);
    core_assert(glIsTexture(texture), "failed to generate texture object");

    /*
     * Attach the storage for the buffer object to the active buffer texture,
     * and specify the internal format for the texel array found in the
     * attached buffer object.
     */
    glTexBuffer(GL_TEXTURE_BUFFER, internalformat, buffer);

    /*
     * Unbind the texture from the target point and return.
     */
    glBindTexture(GL_TEXTURE_BUFFER, 0);
    return texture;
}

/**
 * destroy_texture
 * @brief Delete a texture object.
 */
void destroy_texture(const GLuint &texture)
{
    glDeleteTextures(1, &texture);
}

/**
 * set_texture_mipmap
 * @brief Generate texture mipmaps and set the indices of the lowest
 * GL_TEXTURE_BASE_LEVEL and highest GL_TEXTURE_MAX_LEVEL mipmap levels.
 * Initially, GL_TEXTURE_BASE_LEVEL is 0 and GL_TEXTURE_MAX_LEVEL is 1000.
 * @see http://docs.gl/gl4/glTexParameter
 */
void set_texture_mipmap(
    const GLenum target,
    const GLboolean mipmap,
    const GLint base_level,
    const GLint max_level)
{
    if (mipmap) {
        glGenerateMipmap(target);
    }
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, base_level);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, max_level);
}

/**
 * set_texture_filter
 * @brief Set texture  minifying and magnifying functions.
 *  - The minifying function GL_TEXTURE_MIN_FILTER can be either:
 *      GL_NEAREST,
 *      GL_LINEAR,
 *      GL_NEAREST_MIPMAP_NEAREST,
 *      GL_LINEAR_MIPMAP_NEAREST,
 *      GL_NEAREST_MIPMAP_LINEAR, or
 *      GL_LINEAR_MIPMAP_LINEAR.
 *  - The magnifying function GL_TEXTURE_MAG_FILTER can be either:
 *      GL_NEAREST, or GL_LINEAR,
 *  Initially, GL_TEXTURE_MIN_FILTER and GL_TEXTURE_MAG_FILTER are set to
 *  GL_NEAREST.
 *
 * @see http://docs.gl/gl4/glTexParameter
 */
void set_texture_filter(
    const GLenum target,
    const GLint filter_min,
    const GLint filter_mag)
{
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter_mag);
}

/**
 * set_texture_wrap
 * @brief Set wrap parameter for the texture coordinates GL_TEXTURE_WRAP_S,
 * GL_TEXTURE_WRAP_T and GL_TEXTURE_WRAP_R to either:
 *      GL_CLAMP_TO_EDGE,
 *      GL_CLAMP_TO_BORDER,
 *      GL_MIRRORED_REPEAT,
 *      GL_REPEAT, or
 *      GL_MIRROR_CLAMP_TO_EDGE.
 * Initially, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T and GL_TEXTURE_WRAP_R are
 * set to GL_REPEAT.
 * @see http://docs.gl/gl4/glTexParameter
 */
void set_texture_wrap(
    const GLenum target,
    const GLint wrap_s,
    const GLint wrap_t,
    const GLint wrap_r)
{
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, wrap_r);
}

/**
 * get_texture_width
 * @brief Return the width for 0th level of detail (base image)
 * of a texture currently bound a specified target.
 */
GLsizei get_texture_width(const GLenum target)
{
    GLsizei width;
    glGetTexLevelParameteriv(
        target,
        0,                      /* 0th level-of-detail */
        GL_TEXTURE_WIDTH,
        (GLint *) &width);
    return width;
}

/**
 * get_texture_height
 * @brief Return the height for 0th level of detail (base image)
 * of a texture currently bound a specified target.
 */
GLsizei get_texture_height(const GLenum target)
{
    GLsizei height;
    glGetTexLevelParameteriv(
        target,
        0,                      /* 0th level-of-detail */
        GL_TEXTURE_HEIGHT,
        (GLint *) &height);
    return height;
}

/**
 * get_texture_depth
 * @brief Return the depth for 0th level of detail (base image)
 * of a texture currently bound a specified target.
 */
GLsizei get_texture_depth(const GLenum target)
{
    GLsizei depth;
    glGetTexLevelParameteriv(
        target,
        0,                      /* 0th level-of-detail */
        GL_TEXTURE_DEPTH,
        (GLint *) &depth);
    return depth;
}

/**
 * get_texture_internalformat
 * @brief Return the internal format for 0th level of detail
 * (base image) of a texture currently bound a specified target.
 */
GLint get_texture_internalformat(const GLenum target)
{
    GLint internalformat;
    glGetTexLevelParameteriv(
        target,
        0,                      /* 0th level-of-detail */
        GL_TEXTURE_INTERNAL_FORMAT,
        &internalformat);
    return internalformat;
}

/**
 * is_valid_texture_internalformat
 * @brief Is the texture internal format a valid GL texture format?
 */
bool is_valid_texture_internalformat(const GLint internalformat)
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

/**
 * active_bind_texture
 * @brief Bind the texture to the target at the specified texture unit.
 */
void active_bind_texture(GLenum target, GLenum texunit, GLuint texture)
{
    core_assert(
        target == GL_TEXTURE_1D ||
        target == GL_TEXTURE_2D ||
        target == GL_TEXTURE_3D,
        "invalid texture target");
    glActiveTexture(GL_TEXTURE0 + texunit);
    glBindTexture(target, texture);
}

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
    GLuint buffer)
{
    core_assert(target == GL_TEXTURE_BUFFER, "invalid texture buffer target");
    glActiveTexture(GL_TEXTURE0 + texunit);
    glBindTexture(target, texture);
    glTexBuffer(target, internalformat, buffer);
}

} /* gl */
} /* atto */
