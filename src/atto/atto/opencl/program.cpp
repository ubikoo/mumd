/*
 * program.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opencl/program.hpp"

namespace atto {
namespace cl {
namespace Program {

/**
 * load_source_from_file
 * @brief Load program source from the specified file.
 */
std::string load_source_from_file(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file) {
        core_throw("failed to open program source file");
    }

    std::stringstream ss(std::ios::out);
    ss << file.rdbuf();
    file.close();

    return ss.str();
}

/**
 * create_from_source
 * @brief Wrapper for clCreateProgramWithSource. Create a program object
 * and the load the source code specified by text string.
 */
cl_program create_from_source(
    const cl_context &context,
    const std::string &source)
{
    core_assert(!source.empty(), "empty source string");

    cl_int err;
    const char *src = source.c_str();
    size_t length = source.length();
    cl_program program = clCreateProgramWithSource(
        context,        /* OpenCL context */
        1,              /* one null-terminated character string */
        &src,           /* pointer to the character string */
        &length,        /* length of the character string */
        &err);
    core_assert(err == CL_SUCCESS, "clCreateProgramWithSource");
    core_assert(program != NULL, "null cl_program");

    return program;
}

/**
 * create_from_file
 * @brief Wrapper for clCreateProgramWithSource. Create a program object
 * and the load the source code from the specified source filename.
 */
cl_program create_from_file(
    const cl_context &context,
    const std::string &filename)
{
    return create_from_source(context, load_source_from_file(filename));
}

/**
 * release
 * @brief Wrapper for clReleaseProgram. Decrements the cl_program reference
 * count.
 */
cl_int release(const cl_program &program)
{
    cl_int err = CL_INVALID_VALUE;
    if (program != NULL) {
        err = clReleaseProgram(program);
        core_assert(err == CL_SUCCESS, "clReleaseProgram");
    }
    return err;
}

/**
 * build
 * @brief Wrapper for clBuildProgram. Build the program object for the
 * specified device. The options parameter specifies a string of build options,
 * including preprocessor defines and various optimizations and code generation
 * options.
 */
cl_int build(
    const cl_program &program,
    const cl_device_id &device,
    const std::string &options)
{
    cl_int err = clBuildProgram(
        program,
        1,
        &device,
        options.c_str(),
        NULL,               /* no callback, wait until completion */
        NULL);              /* don't pass callback user data */

    if (err != CL_SUCCESS) {
        size_t infolen;
        clGetProgramBuildInfo(
            program,
            device,
            CL_PROGRAM_BUILD_LOG,
            0,
            NULL,
            &infolen);

        std::string infolog(infolen+1, '\0');
        clGetProgramBuildInfo(
            program,
            device,
            CL_PROGRAM_BUILD_LOG,
            infolen,
            (void *) &infolog[0],
            NULL);

        core_assert(err == CL_SUCCESS,
            std::string("failed to link program:\n\n") + infolog);
    }

    return err;
}

/**
 * get_num_devices
 * @brief Wrapper for clGetProgramInfoReturn, query CL_PROGRAM_NUM_DEVICES.
 * Return the number of devices associated with program.
 */
cl_uint get_num_devices(const cl_program &program)
{
    cl_uint num_devices;

    cl_int err = clGetProgramInfo(
        program,
  	    CL_PROGRAM_NUM_DEVICES,
        sizeof(cl_uint),
        &num_devices,
        NULL);
    core_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_NUM_DEVICES");

    return num_devices;
}

/**
 * get_devices
 * @brief Wrapper for clGetProgramInfoReturn, query CL_PROGRAM_DEVICES.
 * Return the list of devices associated with the program object.
 */
std::vector<cl_device_id> get_devices(const cl_program &program)
{
    cl_int err;

    size_t param_value_size;
    err = clGetProgramInfo(
        program,
  	    CL_PROGRAM_DEVICES,
        0,
        NULL,
        &param_value_size);
    core_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_DEVICES");

    size_t num_devices = param_value_size / sizeof(cl_device_id);
    core_assert(num_devices > 0, "invalid num_devices");

    std::vector<cl_device_id> devices(num_devices);
    err = clGetProgramInfo(
        program,
  	    CL_PROGRAM_DEVICES,
        param_value_size,
        (void *) &devices[0],
        NULL);
    core_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_DEVICES");

    return devices;
}

/**
 * get_source
 * @brief Wrapper for clGetProgramInfoReturn, query CL_PROGRAM_SOURCE.
 * Return the program source code specified by clCreateProgramWithSource.
 * The source string returned is a concatenation of all source strings
 * specified to clCreateProgramWithSource with a null terminator.
 * The concatenation strips any nulls in the original source strings.
 */
std::string get_source(const cl_program &program)
{
    cl_int err;

    size_t param_value_size;
    err = clGetProgramInfo(
        program,
        CL_PROGRAM_SOURCE,
        0,
        NULL,
        &param_value_size);
    core_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_SOURCE");

    size_t num_source_chars = param_value_size / sizeof(char);
    core_assert(num_source_chars > 0, "invalid num_source_chars");

    std::string source(num_source_chars, '\0');
    err = clGetProgramInfo(
        program,
        CL_PROGRAM_SOURCE,
        param_value_size,
        (void *) &source[0],
        NULL);
    core_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_SOURCE");

    return source;
}

/**
 * get_num_kernels
 * @brief Wrapper for clGetProgramInfoReturn, query CL_PROGRAM_NUM_KERNELS.
 * Returns the number of kernels declared in program that can be created
 * with clCreateKernel.
 * This information is only available after a successful program executable
 * has been built for at least one device.
 */
size_t get_num_kernels(const cl_program &program)
{
    cl_uint num_kernels;

    cl_int err = clGetProgramInfo(
        program,
  	    CL_PROGRAM_NUM_KERNELS,
        sizeof(size_t),
        &num_kernels,
        NULL);
    core_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_NUM_KERNELS");

    return num_kernels;
}

/**
 * get_kernel_names
 * @brief Wrapper for clGetProgramInfoReturn, query CL_PROGRAM_KERNEL_NAMES.
 * Returns a semi-colon separated list of kernel names in program that can
 * be created with clCreateKernel.
 * This information is only available after a successful program executable
 * has been built for at least one device.
 */
std::string get_kernel_names(const cl_program &program)
{
    cl_int err;

    size_t param_value_size;
    err = clGetProgramInfo(
        program,
        CL_PROGRAM_KERNEL_NAMES,
        0,
        NULL,
        &param_value_size);
    core_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_KERNEL_NAMES");

    size_t num_kernel_names_chars = param_value_size / sizeof(char);
    core_assert(num_kernel_names_chars > 0, "invalid num_kernel_names_chars");

    std::string kernel_names(num_kernel_names_chars, '\0');
    err = clGetProgramInfo(
        program,
        CL_PROGRAM_KERNEL_NAMES,
        param_value_size,
        (void *) &kernel_names[0],
        NULL);
    core_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_KERNEL_NAMES");

    return kernel_names;
}

} /* Program */
} /* cl */
} /* atto */
