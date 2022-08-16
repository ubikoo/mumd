/*
 * program.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GLSL_PROGRAM_H_
#define ATTO_OPENGL_GLSL_PROGRAM_H_

#include <vector>
#include "atto/opengl/base.hpp"
#include "atto/opengl/glsl/shader.hpp"
#include "atto/opengl/glsl/variable.hpp"
#include "atto/opengl/glsl/uniform.hpp"
#include "atto/opengl/glsl/attribute.hpp"

namespace atto {
namespace gl {

/**
 * create_program
 * @brief Create a shader program object from a set of shader objects.
 */
GLuint create_program(const std::vector<GLuint> &shaders);

/**
 * destroy_program
 * @brief Delete a shader program object.
 */
void destroy_program(const GLuint &program);

/**
 * get_program_info
 * @brief Return an string containing the shader program object info.
 */
std::string get_program_info(const GLuint &program);

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GLSL_PROGRAM_H_ */
