/*
 * base.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_BASE_H_
#define ATTO_OPENGL_BASE_H_

/**
 * Core/Math interface.
 */
#include "atto/core/core.hpp"
#include "atto/math/math.hpp"

/**
 * OpenGL/GLFW headers.
 * @brief Define GLFW_INCLUDE_NONE before the GLFW header to explicitly
 * disable inclusion of the OpenGL environment header and use the header
 * provided by glad.
 * @see https://www.glfw.org/docs/3.3
 */
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <OpenGL/CGLCurrent.h>
#include <GLFW/glfw3.h>

/**
 * Assimp headers.
 * @brief Define C++ importer interface, output data structure and post
 * processing flags.
 */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#endif /* ATTO_OPENGL_BASE_H_ */
