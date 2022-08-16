/*
 * vertex-array.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_VERTEX_ARRAY_H_
#define ATTO_OPENGL_VERTEX_ARRAY_H_

#include "atto/opengl/base.hpp"
#include "atto/opengl/buffer.hpp"

namespace atto {
namespace gl {

/**
 * create_vertex_array
 * @brief Create a vertex array object bound to a specified target.
 */
GLuint create_vertex_array(void);

/**
 * destroy_vertex_array
 * @brief Delete a vertex array object.
 */
void destroy_vertex_array(const GLuint &array);

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_VERTEX_ARRAY_H_ */
