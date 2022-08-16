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

#ifndef ATTO_OPENCL_BASE_H_
#define ATTO_OPENCL_BASE_H_

/**
 * Core/Math/OpenGL interface.
 */
#include "atto/core/core.hpp"
#include "atto/math/math.hpp"
#include "atto/opengl/opengl.hpp"

/**
 * OpenCL interface.
 */
#ifdef __APPLE__
/*
#include <OpenCL/cl.h>
#include <OpenCL/cl_gl.h>
#include <OpenCL/cl_gl_ext.h>
#include <OpenCL/cl_ext.h>
#include <OpenCL/gcl.h>
*/
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#endif /* ATTO_OPENCL_BASE_H_ */
