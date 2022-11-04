/*
 * attribute.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/glsl/attribute.hpp"

namespace atto {
namespace gl {

/** ---------------------------------------------------------------------------
 * buffer_offset
 * @brief Function implementation of the OpenGL offset macro:
 *      #define BUFFER_OFFSET(offset) ((void *)(offset))
 * @note OpenGL Programming Guide: The Official Guide to Learning OpenGL:
 *  "While there a long history of OpenGL lore on why one might do this,
 *  we use this macro to make the point that we’re specifying an offset
 *  into a buffer object, rather than a pointer to a block of memory as
 *  glVertexAttribPointer()’s prototype would suggest.",
 */
static inline GLvoid *buffer_offset(GLsizeiptr offset)
{
    return reinterpret_cast<GLvoid *>(offset);
}

/** ---------------------------------------------------------------------------
 * attribute_enable
 * @brief Enable a generic vertex attribute specified by its location index
 * in the shader program object.
 *
 * By default, all client-side capabilities are disabled, including generic
 * vertex attribute arrays. If enabled, values in a generic vertex attribute
 * array can be accessed and used for rendering when calls are made to vertex
 * array commands such as: glDrawArrays, glDrawElements, glDrawRangeElements,
 * glMultiDrawElements, glMultiDrawArrays, etc.
 */
bool enable_attribute(const GLint location)
{
    if (location == -1) {
        core_debug("invalid attribute location");
        return false;
    }
    glEnableVertexAttribArray(location);
    return true;
}

/**
 * enable_attribute
 * @brief Enable a generic vertex attribute with the specified name.
 */
bool enable_attribute(const GLuint &program, const std::string &name)
{
    bool ret = enable_attribute(glGetAttribLocation(program, name.c_str()));
    if (!ret) {
        core_debug(core::str_format("invalid attribute: %s", name.c_str()));
    }
    return ret;
}

/**
 * disable_attribute
 * @brief Disable a generic vertex attribute specified by its location index
 * in the shader program object.
 */
bool disable_attribute(const GLint location)
{
    if (location == -1) {
        core_debug("invalid attribute location");
        return false;
    }
    glDisableVertexAttribArray(location);
    return true;
}

/**
 * disable_attribute
 * @brief Enable a generic vertex attribute with the specified name.
 */
bool disable_attribute(const GLuint &program, const std::string &name)
{
    bool ret = disable_attribute(glGetAttribLocation(program, name.c_str()));
    if (!ret) {
        core_debug(core::str_format("invalid attribute: %s", name.c_str()));
    }
    return ret;
}

/** ---------------------------------------------------------------------------
 * attribute_pointer
 * @brief Wrapper for glVertexAttribPointer, glEnableVertexAttribArray.
 *
 * Given a vertex buffer object bound to GL_ARRAY_BUFFER, specify the location
 * on the buffer and the data format of the generic vertex attribute at the
 * location index in the vertex shader.
 * Vertex buffer data is of type vec{1,2,3,4}{d,f,i,ui} (cf. DataType).
 *
 * The specified interpretation is stored in the vertex array object that is
 * currently bound to then be used when a draw call is made.
 * The attribute is disabled by default and needs to be explicitly enabled
 * using glEnableVertexAttribArray with its location in the shader program.
 * After enabling, glVertexAttribPointer specifies how OpenGL interprets the
 * vertex buffer data:
 *      glVertexAttribPointer(GLuint index,
 *                            GLint length,
 *                            GLenum type,
 *                            GLboolean normalized,
 *                            GLsizei stride,
 *                            const GLvoid * pointer);
 *      index:      location index of the vertex attribute to be modified.
 *      length:     number of components per vertex attribute - 1,2,3,4.
 *      type:       data type of each component in the array.
 *      normalized: if normalized, map values stored in an integer format to
 *                  [-1,1] (signed) or [0,1] (unsigned). Otherwise, convert to
 *                  floats directly without normalization.
 *      stride:     Byte offset between consecutive vertex attribute.
 *                  If stride is 0, the generic vertex attribute are assumed
 *                  to be tightly packed in the array. Initial value is 0.
 *      pointer:    Byte offset of the first component of the first generic
 *                  vertex attribute in the array in the data store of the
 *                  buffer currently bound to the GL_ARRAY_BUFFER target.
 *                  Initial value is 0.
 *
 * The constants accepted by glVertexAttribPointer are:
 *      GL_BYTE and GL_UNSIGNED_BYTE,
 *      GL_SHORT and GL_UNSIGNED_SHORT,
 *      GL_INT and GL_UNSIGNED_INT,
 *      GL_HALF_FLOAT and GL_FLOAT,
 *      GL_DOUBLE and GL_FIXED.
 *      The initial value is GL_FLOAT.
 *
 * DataType recognises only the OpenGL enumerated constants:
 *      GL_INT,
 *      GL_UNSIGNED_INT,
 *      GL_FLOAT,
 *      GL_DOUBLE.
 *
 * Parameter description:
 *      @param stride:      byte offset between consecutive attributes.
 *      @param offset:      byte offset of the first element in the buffer.
 *
 * @note Base and local offset represent an arbitrary set of vertex attribute
 * within the same buffer. For example, consider two arrays of vertex attribute
 * with the following representation:
 *
 *      array 0 is [RGB XYZ, RGB XYZ, RGB XYZ, ...] with N vertices
 *      array 1 is [RGB RGB RGB..., XYZ XYZ XYZ...] with M vertices
 *
 * They can share the same buffer object data store, with the following XYZ
 * attribute offsets:
 *
 *      array 0 baseoffset = (0) bytes, i.e. beginning of the buffer.
 *      array 0 localoffset = (3x8) bytes, i.e. after first RGB tuple.
 *
 *      array 1 baseoffset = (6x8xN) bytes, i.e. after array 0 data.
 *      array 1 localoffset = (3x8xM) bytes, i.e. after M RGB tuples.
 *
 * @see https://www.khronos.org/opengl/wiki/GLAPI/glVertexAttribPointer
 *      https://www.khronos.org/opengl/wiki/GLAPI/glEnableVertexAttribArray
 *      https://stackoverflow.com/questions/28014864
 *      https://stackoverflow.com/questions/12427880
 *      https://stackoverflow.com/questions/8220484
 */
bool attribute_pointer(
    const GLint location,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset,
    const GLboolean normalized)
{
    if (location == -1) {
        core_debug("invalid attribute location");
        return false;
    }

    GLint vertex_size = DataType::length(type);
    GLenum vertex_type = DataType::is_double(type) ? GL_DOUBLE       :
                         DataType::is_float(type)  ? GL_FLOAT        :
                         DataType::is_int(type)    ? GL_INT          :
                         DataType::is_uint(type)   ? GL_UNSIGNED_INT : 0;
    if (vertex_type == 0) {
        core_debug("invalid vertex type");
        return false;
    }

    glVertexAttribPointer(
        location,
        vertex_size,
        vertex_type,
        normalized,
        stride,
        buffer_offset(offset));
    return true;
}

/**
 * attribute_pointer
 * @brief Specify the location on the buffer and the data format of the generic
 * vertex attribute with the specified name in the shader program object.
 */
bool attribute_pointer(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset,
    const GLboolean normalized)
{
    bool ret = attribute_pointer(
        glGetAttribLocation(program, name.c_str()),
        type,
        stride,
        offset,
        normalized);
    if (!ret) {
        core_debug(core::str_format("invalid attribute: %s", name.c_str()));
    }
    return ret;
}

/**
 * attribute_pointer_i
 * @brief Wrapper for glVertexAttribIPointer.
 *
 * Set the buffer object currently bound to GL_ARRAY_BUFFER from which to
 * fetch the vertex data and specify the format of what a single generic
 * vertex attribute looks like.
 * glVertexAttribIPointer always leaves values as integer types and accepts
 * only the integer types:
 *      GL_BYTE​,
 *      GL_UNSIGNED_BYTE​,
 *      GL_SHORT​,
 *      GL_UNSIGNED_SHORT​,
 *      GL_INT,​ and
 *      GL_UNSIGNED_INT​.
 * Vertex buffer data is of the type vec{1,2,3,4}{i,ui} defined in DataType.
 */
bool attribute_pointer_i(
    const GLint location,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset)
{
    if (location == -1) {
        core_debug("invalid attribute location");
        return false;
    }

    GLint vertex_size = DataType::length(type);
    GLenum vertex_type = DataType::is_int(type)  ? GL_INT  :
                         DataType::is_uint(type) ? GL_UNSIGNED_INT : 0;
    if (vertex_type == 0) {
        core_debug("invalid vertex type");
        return false;
    }

    glVertexAttribIPointer(
        location,
        vertex_size,
        vertex_type,
        stride,
        buffer_offset(offset));
    return true;
}

/**
 * attribute_pointer_i
 * @brief Specify the data format of a generic vertex attribute with the
 * given name in the shader program object.
 */
bool attribute_pointer_i(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset)
{
    bool ret = attribute_pointer_i(
        glGetAttribLocation(program, name.c_str()),
        type,
        stride,
        offset);
    if (!ret) {
        core_debug(core::str_format("invalid attribute: %s", name.c_str()));
    }
    return ret;
}

/**
 * attribute_pointer_d
 * @brief Wrapper for glVertexAttribLPointer.
 *
 * Set the vertex buffer object from which to fetch the vertex data and
 * specify the format of what a single unit of data looks like.
 * glVertexAttribLPointer is only for non-normalized doubles.
 * Vertex buffer data is of the type vec{1,2,3,4}d defined in DataType.
 */
bool attribute_pointer_d(
    const GLint location,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset)
{
    if (location == -1) {
        core_debug("invalid attribute location");
        return false;
    }

    GLint vertex_size = DataType::length(type);
    GLenum vertex_type = DataType::is_double(type) ? GL_DOUBLE : 0;
    if (vertex_type == 0) {
        core_debug("invalid vertex type");
        return false;
    }

    glVertexAttribLPointer(
        location,
        vertex_size,
        vertex_type,
        stride,
        buffer_offset(offset));
    return true;
}

/**
 * attribute_pointer_d
 * @brief Specify the data format of a generic vertex attribute with the
 * given name in the shader program object.
 */
bool attribute_pointer_d(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset)
{
    bool ret = attribute_pointer_d(
        glGetAttribLocation(program, name.c_str()),
        type,
        stride,
        offset);
    if (!ret) {
        core_debug(core::str_format("invalid attribute: %s", name.c_str()));
    }
    return ret;
}

/** ---------------------------------------------------------------------------
 * attribute_value
 * @brief Wrapper for glVertexAttrib{1,2,3,4}{d,f}v and glVertexAttrib4{i,ui}v.
 *
 * Specify all values of a generic vertex attribute. Generic attributes are
 * defined as four-component values organised into an array size of
 * GL_MAX_VERTEX_ATTRIBS.
 *
 * glVertexAttrib family of entry points allows the modification of individual
 * elements of this array, specified by the index of the element and the value
 * for the element.
 * These commands can be used to specify one, two, three, or four components
 * of the generic vertex attribute specified by index:
 *
 *  - 1 in the name indicates that one value is passed to modify the 1st
 *  component of the generic vertex attribute. 2nd and 3rd components are
 *  set to 0 and 4th component is set to 1.
 *  - 2 in the name indicates that two values are passed to modify the 1st
 *  and 2nd components of the generic vertex attribute. 3rd component is set
 *  to 0 and 4th component is set to 1.
 *  - 3 in the name indicates that three values are passed to modify the 1st,
 *  2nd and 3rd components of the generic vertex attribute. 4th component is
 *  set to 1.
 *  - 4 in the name indicates that four values are passed to modify the 1st,
 *  2nd, 3rd, and 4th components of the generic vertex attribute.
 *
 * The letters {s,f,i,d,ub,us,ui} indicate whether the arguments are of type
 * short, float, int, double, unsigned byte, unsigned short, and unsigned int.
 * The {v} suffix indicates that the commands take a pointer to an array of
 * such values.
 *
 * Additional capitalized letters can indicate further alterations to the
 * default behavior of the glVertexAttrib function:
 *  - I indicates arguments are extended to full signed or unsigned integers.
 *  - L indicates arguments are full 64-bit quantities and are passed directly
 *  to shader inputs declared as 64-bit double precision types.
 */
bool attribute_value(
    const GLint location,
    const GLenum type,
    const void *data)
{
    if (location == -1) {
        core_debug("invalid attribute location");
        return false;
    }

    if (data == nullptr) {
        core_debug("invalid attribute data");
        return false;
    }

    switch (type) {
    /* Vector double, dvec2, dvec3, dvec4 */
    case GL_DOUBLE:
        glVertexAttrib1dv(location, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC2:
        glVertexAttrib2dv(location, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC3:
        glVertexAttrib3dv(location, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC4:
        glVertexAttrib4dv(location, static_cast<const GLdouble *>(data));
        break;

    /* Vector float, fvec2, fvec3, fvec4 */
    case GL_FLOAT:
        glVertexAttrib1fv(location, static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_VEC2:
        glVertexAttrib2fv(location, static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_VEC3:
        glVertexAttrib3fv(location, static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_VEC4:
        glVertexAttrib4fv(location, static_cast<const GLfloat *>(data));
        break;

    /* Vector ivec4 */
    case GL_INT_VEC4:
        glVertexAttrib4iv(location, static_cast<const GLint *>(data));
        break;

    /* Vector uvec4 */
    case GL_UNSIGNED_INT_VEC4:
        glVertexAttrib4uiv(location, static_cast<const GLuint *>(data));
        break;

    /* Invalid attribute type */
    default:
        core_debug(core::str_format("invalid attribute type: %d\n", type));
        return false;
        break;
    }
    return true;
}

/**
 * set_attribute
 * @brief Set the value of a generic vertex attribute with the given name in
 * the shader program object.
 */
bool attribute_value(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data)
{
    bool ret = attribute_value(
        glGetAttribLocation(program, name.c_str()), type, data);
    if (!ret) {
        core_debug(core::str_format("invalid attribute: %s", name.c_str()));
    }
    return ret;
}

/**
 * attribute_value_i
 * @brief Wrapper for glVertexAttribI{1,2,3,4}{i,ui}v.
 * Specify all values of a generic vertex attribute.
 * glVertexAttribI* extends the arguments to signed or unsigned integers.
 */
bool attribute_value_i(
    const GLint location,
    const GLenum type,
    const void *data)
{
    if (location == -1) {
        core_debug("invalid attribute location");
        return false;
    }

    if (data == nullptr) {
        core_debug("invalid attribute data");
        return false;
    }

    switch (type) {
    /* Vector int, ivec2, ivec3, ivec4 */
    case GL_INT:
        glVertexAttribI1iv(location, static_cast<const GLint *>(data));
        break;
    case GL_INT_VEC2:
        glVertexAttribI2iv(location, static_cast<const GLint *>(data));
        break;
    case GL_INT_VEC3:
        glVertexAttribI3iv(location, static_cast<const GLint *>(data));
        break;
    case GL_INT_VEC4:
        glVertexAttribI4iv(location, static_cast<const GLint *>(data));
        break;

    /* Vector unsigned int, uvec2, uvec3, uvec4 */
    case GL_UNSIGNED_INT:
        glVertexAttribI1uiv(location, static_cast<const GLuint *>(data));
        break;
    case GL_UNSIGNED_INT_VEC2:
        glVertexAttribI2uiv(location, static_cast<const GLuint *>(data));
        break;
    case GL_UNSIGNED_INT_VEC3:
        glVertexAttribI3uiv(location, static_cast<const GLuint *>(data));
        break;
    case GL_UNSIGNED_INT_VEC4:
        glVertexAttribI4uiv(location, static_cast<const GLuint *>(data));
        break;

    /* Invalid attribute type */
    default:
        core_debug(core::str_format("invalid attribute type: %d\n", type));
        return false;
        break;
    }
    return true;
}

/**
 * attribute_value_i
 * @brief Set the value of a generic vertex attribute with the given name in
 * the shader program object.
 */
bool attribute_value_i(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data)
{
    bool ret = attribute_value_i(
        glGetAttribLocation(program, name.c_str()), type, data);
    if (!ret) {
        core_debug(core::str_format("invalid attribute: %s", name.c_str()));
    }
    return ret;
}

/**
 * attribute_value_d
 * @brief Wrapper for glVertexAttribL{1,2,3,4}dv.
 * Specify the all values of a generic vertex attribute.
 * glVertexAttribL* extends the arguments to full 64-bit quantities,
 * passed to shader inputs declared as 64-bit double precision types.
 */
bool attribute_value_d(
    const GLint location,
    const GLenum type,
    const void *data)
{
    if (location == -1) {
        core_debug("invalid attribute location");
        return false;
    }

    if (data == nullptr) {
        core_debug("invalid attribute data");
        return false;
    }

    switch (type) {
    /* Vector double, dvec2, dvec3, dvec4 */
    case GL_DOUBLE:
        glVertexAttribL1dv(location, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC2:
        glVertexAttribL2dv(location, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC3:
        glVertexAttribL3dv(location, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC4:
        glVertexAttribL4dv(location, static_cast<const GLdouble *>(data));
        break;

    /* Invalid attribute type */
    default:
        core_debug(core::str_format("invalid attribute type: %d\n", type));
        return false;
        break;
    }
    return true;
}

/**
 * set_attribute_d
 * @brief Set the value of a generic vertex attribute with the given name in
 * the shader program object.
 */
bool attribute_value_d(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data)
{
    bool ret = attribute_value_d(
        glGetAttribLocation(program, name.c_str()), type, data);
    if (!ret) {
        core_debug(core::str_format("invalid attribute: %s", name.c_str()));
    }
    return ret;
}

/** ---------------------------------------------------------------------------
 * attribute_divisor
 * @brief Wrapper for glVertexAttribDivisor.
 * Modify the rate at which the generic vertex attribute at the specified
 * location advances during instanced rendering.
 */
bool attribute_divisor(const GLint location, const GLuint divisor)
{
    if (location == -1) {
        core_debug("invalid attribute location");
        return false;
    }
    glVertexAttribDivisor(location, divisor);
    return true;
}

/**
 * attribute_divisor
 * @brief Wrapper for glVertexAttribDivisor.
 * Modify the rate at which the generic vertex attribute with the given name in
 * the shader program object advances during instanced rendering.
 */
bool attribute_divisor(
    const GLuint &program,
    const std::string &name,
    const GLuint divisor)
{
    bool ret = attribute_divisor(
        glGetAttribLocation(program, name.c_str()), divisor);
    if (!ret) {
        core_debug(core::str_format("invalid attribute: %s", name.c_str()));
    }
    return ret;
}

} /* gl */
} /* atto */
