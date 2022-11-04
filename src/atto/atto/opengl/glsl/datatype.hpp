/*
 * datatype.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GLSL_DATATYPE_H_
#define ATTO_OPENGL_GLSL_DATATYPE_H_

#include "atto/opengl/base.hpp"

namespace atto {
namespace gl {
namespace DataType {

/**
 * DataType::contains
 * @brief Does the map contain the key of the GL data type?
 */
bool contains(const GLenum data_type);

/**
 * DataType::name
 * @brief Return the name of the GL data type.
 */
std::string name(const GLenum data_type);

/**
 * DataType::length
 * @brief Return the length of the GL data type in units of the primitive
 * data type.
 */
GLuint length(const GLenum data_type);

/**
 * DataType::size
 * @brief Return the size of the underlying primitive data type in bytes.
 */
GLuint size(const GLenum data_type);

/**
 * DataType::is_double
 * @brief Is the underlying primitive data type GLdouble?
 */
bool is_double(const GLenum data_type);

/**
 * DataType::is_float
 * @brief Is the underlying primitive data type GLfloat?
 */
bool is_float(const GLenum data_type);

/**
 * DataType::is_int
 * @brief Is the underlying primitive data type GLint?
 */
bool is_int(const GLenum data_type);
/**
 * DataType::is_uint
 * @brief Is the underlying primitive data type GLuint?
 */
bool is_uint(const GLenum data_type);

} /* DataType */
} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GLSL_DATATYPE_H_ */
