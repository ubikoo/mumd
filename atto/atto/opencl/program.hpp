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

#ifndef ATTO_OPENCL_PROGRAM_H_
#define ATTO_OPENCL_PROGRAM_H_

#include <vector>
#include <fstream>
#include "atto/opencl/base.hpp"
#include "atto/opencl/device.hpp"
#include "atto/opencl/context.hpp"

namespace atto {
namespace cl {
namespace Program {

/**
 * load_source_from_file
 * @brief Load program source from the specified file.
 */
std::string load_source_from_file(const std::string &filename);

/**
 * create_from_source
 * @brief Wrapper for clCreateProgramWithSource. Create a program object
 * and the load the source code specified by text string.
 */
cl_program create_from_source(
    const cl_context &context,
    const std::string &source);

/**
 * create_from_source
 * @brief Wrapper for clCreateProgramWithSource. Create a program object
 * and the load the source code from the specified source filename.
 */
cl_program create_from_file(
    const cl_context &context,
    const std::string &filename);

/**
 * release
 * @brief Wrapper for clReleaseProgram. Decrements the cl_program reference
 * count.
 */
cl_int release(const cl_program &program);

/**
 * build
 * @brief Wrapper for clBuildProgram.
 */
cl_int build(
    const cl_program &program,
    const cl_device_id &device,
    const std::string &options);

/**
 * get_num_devices
 * @brief Wrapper for clGetProgramInfoReturn, query CL_PROGRAM_NUM_DEVICES.
 */
cl_uint get_num_devices(const cl_program &program);

/**
 * get_devices
 * @brief Wrapper for clGetProgramInfoReturn, query CL_PROGRAM_DEVICES.
 */
std::vector<cl_device_id> get_devices(const cl_program &program);

/**
 * get_source
 * @brief Wrapper for clGetProgramInfoReturn, query CL_PROGRAM_SOURCE.
 */
std::string get_source(const cl_program &program);

/**
 * get_num_kernels
 * @brief Wrapper for clGetProgramInfoReturn, query CL_PROGRAM_NUM_KERNELS.
 */
size_t get_num_kernels(const cl_program &program);

/**
 * get_kernel_names
 * @brief Wrapper for clGetProgramInfoReturn, query CL_PROGRAM_KERNEL_NAMES.
 */
std::string get_kernel_names(const cl_program &program);

} /* Program */
} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_PROGRAM_H_ */
