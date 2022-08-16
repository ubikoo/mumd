/*
 * buffer.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/buffer.hpp"

namespace atto {
namespace gl {

/**
 * create_buffer
 * @brief Create a buffer object bound to a specified target with a data
 * store size in bytes and with a specified given usage.
 * Buffer usage can be classified down to two parts, frequency and nature
 * of access:
 *  GL_STREAM_DRAW,  GL_STREAM_READ,  GL_STREAM_COPY
 *  GL_STATIC_DRAW,  GL_STATIC_READ,  GL_STATIC_COPY
 *  GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY
 *
 *  STREAM      Data store contents will be modified once and
 *              used only a few times.
 *  STATIC      Data store contents will be modified once and
 *              used many times.
 *  DYNAMIC     Data store contents will be modified repeatedly
 *              and used many times.
 *
 *  DRAW        Data store contents are modified by the application,
 *              and used as the source for GL drawing and image
 *              specification commands.
 *  READ        Data store contents are modified by reading data from
 *              the GL, and used to return that data when queried by
 *              the application.
 *  COPY        Data store contents are modified by reading data from
 *              the GL,and used as the source for GL drawing and image
 *              specification commands.
 *
 * @note A buffer object contains an array of unformatted data allocated
 * by OpenGL and nothing more. The meaning of the data in the buffer is
 * defined when the buffer is bound to a target using glBindBuffer.
 * A buffer is only a vertex buffer object holding vertex array attributes
 * when is bound to a GL_ARRAY_BUFFER target.
 * The exact same buffer is interpreted as an element buffer object with
 * vertex indices if it is bound to a GL_ELEMENT_ARRAY_BUFFER target.
 * The exact same buffer is interpreted as a pixel buffer object if it is
 * bound to GL_PIXEL_PACK_BUFFER.

 * @see glBindBuffer
 *      glBufferData
 *      glBufferSubData
 *      glGetBufferSubData,
 *      glCopyBufferSubData
 *      glMapBuffer
 *      glMapBufferRange
 *      glFlushMappedBufferRange
 *      glUnmapBuffer
 *      glGetBufferPointerv
 *
 *      - General documentation on buffer objects:
 *      https://www.khronos.org/opengl/wiki/Buffer_Object
 *      http://www.songho.ca/opengl/gl_vbo.html
 *      http://www.songho.ca/opengl/gl_fbo.html
 *      http://www.songho.ca/opengl/gl_pbo.html
 *      http://www.songho.ca/opengl/gl_vertexarray.html
 *      https://stackoverflow.com/questions/748162
 *      http://antongerdelan.net/opengl/vertexbuffers.html
 *
 *      - OpenGL buffer object wrapping class:
 *      https://codereview.stackexchange.com/questions/149871
 *      https://codereview.stackexchange.com/questions/161050
 *
 *      - OpenGL direct state access:
 *      https://www.khronos.org/opengl/wiki/Direct_State_Access
 *      https://stackoverflow.com/questions/33363490
 *
 *      - OpenGL persistent buffer mapping and immutable storage:
 *      https://www.khronos.org/opengl/wiki/Buffer_Object_Streaming
 *      https://www.codeproject.com/Articles/872417/Persistent-Mapped-Buffers-in-OpenGL
 *      https://learnopengl.com/Advanced-OpenGL/Advanced-Data
 *      https://stackoverflow.com/questions/19639689
 *      https://stackoverflow.com/questions/47563995
 *      https://gamedev.stackexchange.com/questions/87074
 */
GLuint create_buffer(
    const GLenum target,
    const GLsizeiptr size,
    const GLenum usage)
{
    core_assert(size > 0, "invalid buffer data store size");

    /*
     * Generate a new buffer object name and bind it to the target point.
     * No buffer object is associated with the name set by glGenBuffers
     * until it is bound to the target by a call to glBindBuffer.
     */
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    core_assert(glIsBuffer(buffer), "failed to generate buffer object");

    /*
     * Create a new data store for a buffer object.
     * Use the buffer object currently bound to the target (glBufferData)
     * or use a buffer object associated with the name (glNamedBufferData).
     */
    glBufferData(target, size, NULL, usage);

    /*
     * Unbind the buffer from the target point and return.
     */
    glBindBuffer(target, 0);
    return buffer;
}

/**
 * destroy_buffer
 * @brief Delete a buffer object.
 */
void destroy_buffer(const GLuint &buffer)
{
    glDeleteBuffers(1, &buffer);
}

/**
 * get_buffer_access
 * @brief Return the access policy set while mapping the buffer object.
 */
GLenum get_buffer_access(const GLenum target)
{
    GLenum access;
    glGetBufferParameteriv(target, GL_BUFFER_ACCESS, (GLint *) &access);
    return access;
}

/**
 * get_buffer_size
 * @brief Return the parameters of the buffer currently bound to the target.
 */
GLsizeiptr get_buffer_size(const GLenum target)
{
    GLsizeiptr size;
    glGetBufferParameteriv(target, GL_BUFFER_SIZE,   (GLint *) &size);
    return size;
}

/**
 * get_buffer_usage
 * @brief Return the usage pattern of the buffer currently bound to the target.
 */
GLenum get_buffer_usage(const GLenum target)
{
    GLenum usage;
    glGetBufferParameteriv(target, GL_BUFFER_USAGE,  (GLint *) &usage);
    return usage;
}

/**
 * is_buffer_mapped
 * @brief Return the flag indicating whether the buffer is currently mapped.
 */
GLboolean is_buffer_mapped(const GLenum target)
{
    GLboolean mapped;
    glGetBufferParameteriv(target, GL_BUFFER_MAPPED, (GLint *) &mapped);
    return mapped;
}

} /* gl */
} /* atto */
