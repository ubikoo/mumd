/*
 * opengl.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_H_
#define ATTO_OPENGL_H_

/**
 * OpenGL/GLSL interface
 * @brief OpenGL Objects are defined as structures containing state (ie data)
 * and are responsible for transmitting data to and from the GPU. Objects in
 * OpenGL can be separated into different categories:
 *
 *  - Regular Objects. These objects contain data:
 *      Buffer Objects
 *      Renderbuffer Objects
 *      Texture Objects
 *      Query Objects
 *      Sampler Objects
 *  - Container Objects. These objects contain no data - they are plain
 *  containers storing references to other objects:
 *      Framebuffer Objects
 *      Vertex Array Objects
 *      Transform Feedback Objects
 *      Program Pipeline Objects
 *  - Non-standard objects. They do not follow standard conventions for
 *  OpenGL objects:
 *      Shader and Program Objects
 *      Sync Objects
 *
 * @note GL Objects are not implemented in a C/C++ style, as pointers to data.
 * They are integer handles, called "names", that reference the actual data
 * stored in the device.
 * Functions that generate object names are of the form glGen*.
 * Functions that delete object names are of the form glDelete*.
 *
 * @see https://www.khronos.org/opengl/wiki/OpenGL_Object
 *      https://www.khronos.org/opengl/wiki/GLSL_Object
 */
#include "atto/opengl/base.hpp"
#include "atto/opengl/buffer.hpp"
#include "atto/opengl/error.hpp"
#include "atto/opengl/framebuffer.hpp"
#include "atto/opengl/imageformat.hpp"
#include "atto/opengl/renderbuffer.hpp"
#include "atto/opengl/texture.hpp"
#include "atto/opengl/vertex-array.hpp"

#include "atto/opengl/glsl/attribute.hpp"
#include "atto/opengl/glsl/datatype.hpp"
#include "atto/opengl/glsl/program.hpp"
#include "atto/opengl/glsl/shader.hpp"
#include "atto/opengl/glsl/uniform.hpp"
#include "atto/opengl/glsl/variable.hpp"

/**
 * Graphics interface
 * @brief Interface to the GLFW library and collection of objects for graphics
 * rendering. Handle the initialisation and termination of the GLFW library,
 * set/unset renderer event callbacks, etc, define the pure virtual drawable
 * class, etc.
 */
#include "atto/opengl/graphics/drawable.hpp"
#include "atto/opengl/graphics/event.hpp"
#include "atto/opengl/graphics/image.hpp"
#include "atto/opengl/graphics/mesh.hpp"
#include "atto/opengl/graphics/meshmodel.hpp"
#include "atto/opengl/graphics/renderer.hpp"
#include "atto/opengl/graphics/timer.hpp"

#endif /* ATTO_OPENGL_H_ */
