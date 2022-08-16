/*
 * variable.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GLSL_VARIABLE_H_
#define ATTO_OPENGL_GLSL_VARIABLE_H_

#include <vector>
#include "atto/opengl/base.hpp"

namespace atto {
namespace gl {

/** ---- Variable -------------------------------------------------------------
 * @brief Variable is a helper structure specifying the properties of an active
 * variable in a shader program object. Active variables represent both active
 * uniform variables and active attribute variables in the shader.
 *
 * The properties of an active uniform are queried by:
 *      GLint glGetUniformLocation(GLuint program, GLchar *name);
 *      void glGetActiveUniform(GLuint program,
 *                              GLuint index,
 *                              GLsizei max_bufsize,
 *                              GLsizei *bufsize = nullptr,
 *                              GLint *count,
 *                              GLenum *type,
 *                              GLchar *name);
 *
 * The properties of an active attribute are queried by:
 *      GLint glGetAttribLocation(GLuint program, GLchar *name);
 *      void glGetActiveAttrib(GLuint program,
 *                             GLuint index,
 *                             GLsizei max_bufsize,
 *                             GLsizei *bufsize = nullptr,
 *                             GLint *count,
 *                             GLenum *type,
 *                             GLchar *name)
 *
 * The Variable class maintains the following properties of an active variable:
 *  name:       Name of the active variable in the shader program object.
 *  location:   Active variable location in the shader program object.
 *  count:     Array size of the active variable in units of OpenGL type.
 *              Variables other than arrays will have a count of 1.
 *              If the variable is an array, count will be the highest array
 *              element index used, plus one, as determined by the compiler.
 *  type:       OpenGL type of the active variable (see DataType).
 *
 * @note If an active variable is an array, it should be set manually using
 * its name in the shader program object.
 *
 * @see glGetUniformLocation
 *      glGetActiveUniform
 *      glGetAttribLocation
 *      glGetActiveAttrib
 *      https://stackoverflow.com/questions/19894839
 */
struct Variable {
    std::string name;       /* variable name in the shader program */
    GLint location;         /* variable location in the shader program */
    GLsizei count;          /* variable count in units of OpenGL type */
    GLenum type;            /* variable enumerated OpenGL type */
};

/**
 * get_active_uniforms
 * @brief Retrieve all active uniforms in a shader program object.
 */
std::vector<Variable> get_active_uniforms(const GLuint &program);

/**
 * get_active_attributes
 * @brief Retrieve all active attributes in a shader program object.
 */
std::vector<Variable> get_active_attributes(const GLuint &program);

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GLSL_VARIABLE_H_ */
