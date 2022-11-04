/*
 * kernel.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This Kernel is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_KERNEL_H_
#define ATTO_OPENCL_KERNEL_H_

#include <type_traits>
#include "atto/opencl/base.hpp"
#include "atto/opencl/program.hpp"

namespace atto {
namespace cl {
namespace Kernel {

/**
 * create
 * @brief Wrapper for clCreateKernel. Create a kernel object.
 */
cl_kernel create(const cl_program &program, const std::string &kernel_name);

/**
 * release
 * @brief Wrapper for clReleaseKernel. Decrements the cl_kernel reference count.
 */
cl_int release(const cl_kernel &kernel);

/**
 * set_arg
 * @brief Set the argument value for a specific argument of the kernel.
 */
cl_int set_arg(
    const cl_kernel &kernel,
    cl_uint arg_index,
    size_t arg_size,
    const void *arg_value);

} /* Kernel */
} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_KERNEL_H_ */
