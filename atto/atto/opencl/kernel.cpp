/*
 * kernel.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opencl/kernel.hpp"

namespace atto {
namespace cl {
namespace Kernel {

/**
 * create
 * @brief Wrapper for clCreateKernel. Create a kernel object.
 */
cl_kernel create(const cl_program &program, const std::string &kernel_name)
{
    core_assert(!kernel_name.empty(), "empty kernel name string");

    cl_int err;
    cl_kernel kernel = clCreateKernel(program, kernel_name.c_str(), &err);
    core_assert(err == CL_SUCCESS, "clCreateKernel");
    core_assert(kernel != NULL, "null kernel");
    return kernel;
}

/**
 * release
 * @brief Wrapper for clReleaseKernel. Decrements the cl_kernel reference count.
 */
cl_int release(const cl_kernel &kernel)
{
    cl_int err = CL_INVALID_VALUE;
    if (kernel != NULL) {
        err = clReleaseKernel(kernel);
        core_assert(err == CL_SUCCESS, "clReleaseKernel");
    }
    return err;
}

/**
 * set_arg
 * @brief Set the value for a specific argument of the kernel.
 *
 * @param arg_index The argument index. Kernel arguments are referred by
 * indices that go from 0 for the leftmost argument to (n-1), where n is
 * the total number of arguments.
 *
 * @param arg_size Size of the argument value. Argument size is defined
 * by how the argument is declared in the kernel function:
 *  __local qualified: number of bytes required for the buffer used to
 *  store the local argument.
 *  memory object: size of the object, e.g. sizeof(cl_mem).
 *  sampler object: size of the object, e.g. sizeof(cl_sampler).
 *  regular type: size of the argument type, e.g. sizeof(cl_int).
 *
 * @param arg_value Pointer to the argument to be passed to the kernel.
 * The data pointed by arg_value is copied and the arg_value pointer
 * can then be reused by the application after clSetKernelArg returns.
 * The value specified is used by all API calls that enqueue kernel with
 * clEnqueueNDRangeKernel until the argument value is changed again by a
 * call to clSetKernelArg.
 */
cl_int set_arg(
    const cl_kernel &kernel,
    cl_uint arg_index,
    size_t arg_size,
    const void *arg_value)
{
    cl_int err = clSetKernelArg(kernel, arg_index, arg_size, arg_value);
    core_assert(err == CL_SUCCESS, "clSetKernelArg");
    return err;
}

} /* Kernel */
} /* cl */
} /* atto */
