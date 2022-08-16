/*
 * attribute.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GLSL_ATTRIBUTE_H_
#define ATTO_OPENGL_GLSL_ATTRIBUTE_H_

#include "atto/opengl/base.hpp"
#include "atto/opengl/glsl/datatype.hpp"

namespace atto {
namespace gl {

/** ---------------------------------------------------------------------------
 * attribute_enable
 * @brief Enable a generic vertex attribute specified by its location index
 * in the shader program object.
 */
bool enable_attribute(const GLint location);

/**
 * enable_attribute
 * @brief Enable a generic vertex attribute with the specified name.
 */
bool enable_attribute(const GLuint &program, const std::string &name);

/**
 * disable_attribute
 * @brief Disable a generic vertex attribute specified by its location index
 * in the shader program object.
 */
bool disable_attribute(const GLint location);

/**
 * disable_attribute
 * @brief Enable a generic vertex attribute with the specified name.
 */
bool disable_attribute(const GLuint &program, const std::string &name);

/** ---------------------------------------------------------------------------
 * attribute_pointer
 * @brief Wrapper for glVertexAttribPointer, glEnableVertexAttribArray.
 */
bool attribute_pointer(
    const GLint location,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset,
    const GLboolean normalized);

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
    const GLboolean normalized);

/**
 * attribute_pointer_i
 * @brief Wrapper for glVertexAttribIPointer.
 */
bool attribute_pointer_i(
    const GLint location,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset);

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
    const GLsizeiptr offset);

/**
 * attribute_pointer_d
 * @brief Wrapper for glVertexAttribLPointer.
 */
bool attribute_pointer_d(
    const GLint location,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset);

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
    const GLsizeiptr offset);

/** ---------------------------------------------------------------------------
 * attribute_value
 * @brief Wrapper for glVertexAttrib{1,2,3,4}{d,f}v and glVertexAttrib4{i,ui}v.
 */
bool attribute_value(
    const GLint location,
    const GLenum type,
    const void *data);

/**
 * set_attribute
 * @brief Set the value of a generic vertex attribute with the given name in
 * the shader program object.
 */
bool attribute_value(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data);
/**
 * attribute_value_i
 * @brief Wrapper for glVertexAttribI{1,2,3,4}{i,ui}v.
 * Specify all values of a generic vertex attribute.
 * glVertexAttribI* extends the arguments to signed or unsigned integers.
 */
bool attribute_value_i(
    const GLint location,
    const GLenum type,
    const void *data);

/**
 * attribute_value_i
 * @brief Set the value of a generic vertex attribute with the given name in
 * the shader program object.
 */
bool attribute_value_i(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data);

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
    const void *data);

/**
 * set_attribute_d
 * @brief Set the value of a generic vertex attribute with the given name in
 * the shader program object.
 */
bool attribute_value_d(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data);

/** ---------------------------------------------------------------------------
 * attribute_divisor
 * @brief Wrapper for glVertexAttribDivisor.
 * Modify the rate at which the generic vertex attribute at the specified
 * location advances during instanced rendering.
 */
bool attribute_divisor(const GLint location, const GLuint divisor);

/**
 * attribute_divisor
 * @brief Wrapper for glVertexAttribDivisor.
 * Modify the rate at which the generic vertex attribute with the given name in
 * the shader program object advances during instanced rendering.
 */
bool attribute_divisor(
    const GLuint &program,
    const std::string &name,
    const GLuint divisor);

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GLSL_ATTRIBUTE_H_ */
