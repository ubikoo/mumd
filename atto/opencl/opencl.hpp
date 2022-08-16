/*
 * opencl.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_H_
#define ATTO_OPENCL_H_

/**
 * OpenCL interface
 *
 * - Platform layer:
 *      Query platform
 *      Query devices
 *      Create command queue
 *
 * - Runtime layer:
 *      Create buffers
 *      Compile program
 *
 *      Build kernel_a
 *      Set kernel_a arguments
 *      Build kernel_b
 *      Set kernel_b arguments
 *
 *      Enqueue write buffer data
 *      Enqueue kernel execution
 *      Enqueue read buffer data
 */
#include "atto/opencl/base.hpp"
#include "atto/opencl/platform.hpp"
#include "atto/opencl/device.hpp"
#include "atto/opencl/context.hpp"

#include "atto/opencl/queue.hpp"
#include "atto/opencl/program.hpp"
#include "atto/opencl/kernel.hpp"
#include "atto/opencl/ndrange.hpp"
#include "atto/opencl/event.hpp"

#include "atto/opencl/memory.hpp"
#include "atto/opencl/sampler.hpp"

#include "atto/opencl/gl.hpp"
#include "atto/opencl/image.hpp"
#include "atto/opencl/math.hpp"

#endif /* ATTO_OPENCL_H_ */
