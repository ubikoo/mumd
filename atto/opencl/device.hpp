/*
 * device.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_DEVICE_H_
#define ATTO_OPENCL_DEVICE_H_

#include <vector>
#include "atto/opencl/base.hpp"
#include "atto/opencl/platform.hpp"

namespace atto {
namespace cl {
namespace Device {

/**
 * get_device_ids
 * @brief Wrapper for clGetDeviceIDs. Create a vector of all devices
 * of a given type currently available in the specified platform.
 */
std::vector<cl_device_id> get_device_ids(
    const cl_platform_id &platform,
    cl_device_type type);

/**
 * get_device_ids
 * @brief Wrapper for clGetDeviceIDs. Create a vector of all devices
 * of a given type currently available in the first available platform.
 */
std::vector<cl_device_id> get_device_ids(cl_device_type type);

/**
 * release
 * @brief Decrements the device reference count.
 */
cl_int release(const cl_device_id &device);

/**
 * get_info_string
 * @brief Wrapper for clGetDeviceInfo. Return a string with device information.
 */
std::string get_info_string(const cl_device_id &device);

} /* Device */
} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_DEVICE_H_ */
