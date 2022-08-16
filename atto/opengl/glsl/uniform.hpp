/*
 * uniform.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GLSL_UNIFORM_H_
#define ATTO_OPENGL_GLSL_UNIFORM_H_

#include "atto/opengl/base.hpp"
#include "atto/opengl/glsl/datatype.hpp"

namespace atto {
namespace gl {

/**
 * set_uniform
 * @brief Update the uniform in the current shader program object using the
 * corresponding glUniform* function.
 */
bool set_uniform(const GLint location, const GLenum type, const void *data);

/**
 * set_uniform
 * @brief Update the uniform with the specified name in the current shader
 * program object using the corresponding glUniform* function.
 */
bool set_uniform(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data);

/**
 * set_uniform_matrix
 * @brief Update the uniform matrix in the current shader program object
 * using the corresponding glUniform* function.
 */
bool set_uniform_matrix(
    const GLint location,
    const GLenum type,
    const GLboolean transpose,
    const void *data);

/**
 * set_uniform_matrix
 * @brief Update the uniform matrix in the current shader program object
 * using the corresponding glUniform* function.
 */
bool set_uniform_matrix(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const GLboolean transpose,
    const void *data);

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GLSL_UNIFORM_H_ */
