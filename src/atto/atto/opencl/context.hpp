/*
 * context.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_CONTEXT_H_
#define ATTO_OPENCL_CONTEXT_H_

#include <vector>
#include "atto/opencl/base.hpp"
#include "atto/opencl/platform.hpp"
#include "atto/opencl/device.hpp"

namespace atto {
namespace cl {
namespace Context {

/**
 * create
 * @brief Wrapper for clCreateContext. Create a context with the list of
 * devices on the specified platform.
 */
cl_context create(
    const cl_platform_id &platform,
    const std::vector<cl_device_id> &devices);

/**
 * create
 * @brief Wrapper for clCreateContext. Create a context with all devices of
 * the specified type in the first available platform.
 */
cl_context create(cl_device_type type);

/**
 * create_cl_gl_shared
 * @brief Wrapper for clCreateContext. Create a context based on the OpenGL
 * context in the specified device.
 */
cl_context create_cl_gl_shared(const cl_device_id &cl_gl_device);

/**
 * create_cl_gl_shared
 * @brief Wrapper for clCreateContext. Create a context based on the OpenGL
 * context and corresponding devices.
 */
cl_context create_cl_gl_shared(void);

/**
 * release
 * @brief Wrapper for clReleaseContext. Decrements the context reference count.
 */
cl_int release(const cl_context &context);

/**
 * get_devices
 * @brief Return a list of devices in the context.
 */
std::vector<cl_device_id> get_devices(const cl_context &context);

/**
 * get_device
 * @brief Return a list of devices in the context.
 */
cl_device_id get_device(const cl_context &context, const size_t index);

} /* Context */
} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_CONTEXT_H_ */
