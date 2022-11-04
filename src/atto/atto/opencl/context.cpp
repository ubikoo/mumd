/*
 * context.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opencl/context.hpp"

namespace atto {
namespace cl {
namespace Context {

/**
 * callback
 * @brief Context error callback function.
 */
static void CL_CALLBACK callback(
    const char *error_info,
    const void *private_info,
    size_t cb,
    void *user_data)
{
    std::cerr << core::to_string("OpenCL context error: ", error_info) << "\n";
}

/**
 * create
 * @brief Wrapper for clCreateContext. Create an OpenCL context with the list
 * of devices on the specified platform.
 * Contexts are used by OpenCL runtime for managing objects such as command
 * queues, memory, program and kernel objects and for executing kernels on
 * one or more devices specified in the context.
 * @note Context properties can be NULL in which case the platform that is
 * selected is implementation-defined.
 */
cl_context create(
    const cl_platform_id &platform,
    const std::vector<cl_device_id> &devices)
{
    core_assert(!devices.empty(), "empty device list");

    static const cl_context_properties context_properties[] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties) platform,
        (cl_context_properties) NULL};

    cl_int err;
    cl_context context = clCreateContext(
        context_properties,         /* specify the platform to use */
        (cl_uint) devices.size(),   /* number of device ids */
        devices.data(),             /* pointer to a list of device ids */
        &callback,                  /* register log callback function */
        NULL,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateContext");
    return context;
}

/**
 * create
 * @brief Create a context with all devices of the specified type in the first
 * available platform.
 */
cl_context create(cl_device_type type)
{
    std::vector<cl_platform_id> platforms = Platform::get_platform_ids();
    std::vector<cl_device_id> devices = Device::get_device_ids(
        platforms[0], type);
    return create(platforms[0], devices);
}

/**
 * create_cl_gl_shared
 * @brief Wrapper for clCreateContext. Create a context based on the OpenGL
 * context in the specified device.
 */
cl_context create_cl_gl_shared(const cl_device_id &cl_gl_device)
{
    /* Get the Core OpenGL context object and sharegroup. */
    CGLContextObj cgl_context = CGLGetCurrentContext();
    CGLShareGroupObj cgl_sharegroup = CGLGetShareGroup(cgl_context);

    static const cl_context_properties gl_context_properties[] = {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties) cgl_sharegroup,
        (cl_context_properties) NULL};

    /* Create the OpenCL context based on the OpenGL context. */
    cl_int err;
    cl_context gl_context = clCreateContext(
        gl_context_properties,      /* specify the platform to use */
        1,                          /* number of device ids */
        &cl_gl_device,              /* pointer to a list of device ids */
        &callback,                  /* register log callback function */
        NULL,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateContext");
    return gl_context;
}

/**
 * create_cl_gl_shared
 * @brief Wrapper for clCreateContext. Create a context based on the OpenGL
 * context and corresponding devices.
 */
cl_context create_cl_gl_shared(void)
{
    /* Get the Core OpenGL context object and sharegroup. */
    CGLContextObj cgl_context = CGLGetCurrentContext();
    CGLShareGroupObj cgl_sharegroup = CGLGetShareGroup(cgl_context);

    static const cl_context_properties gl_context_properties[] = {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties) cgl_sharegroup,
        (cl_context_properties) NULL};

    /* Create the OpenCL context based on the OpenGL context. */
    cl_int err;
    cl_context gl_context = clCreateContext(
        gl_context_properties,      /* specify the platform to use */
        0,                          /* number of device ids */
        0,                          /* pointer to a list of device ids */
        &callback,                  /* register log callback function */
        NULL,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateContext");
    return gl_context;
}

/**
 * release
 * @brief Wrapper for clReleaseContext. Decrements the context reference count.
 */
cl_int release(const cl_context &context)
{
    cl_int err = CL_INVALID_VALUE;
    if (context != NULL) {
        err = clReleaseContext(context);
        core_assert(err == CL_SUCCESS, "clReleaseContext");
    }
    return err;
}

/**
 * get_devices
 * @brief Return a list of devices in the context.
 */
std::vector<cl_device_id> get_devices(const cl_context &context)
{
    cl_int err;

    /* Get the list of devices in the context. */
    size_t size;
    err = clGetContextInfo(
        context,
        CL_CONTEXT_DEVICES,
        0,
        NULL,
        &size);
    core_assert(err == CL_SUCCESS, "clGetContextInfo");

    size_t n_devices = size / sizeof(cl_device_id);
    std::vector<cl_device_id> devices(n_devices);
    err = clGetContextInfo(
        context,
        CL_CONTEXT_DEVICES,
        n_devices * sizeof(cl_device_id),
        (void *) devices.data(),
        NULL);
    core_assert(err == CL_SUCCESS, "clGetContextInfo");

    return devices;
}

/**
 * get_device
 * @brief Return the device with the specfied index in the context.
 */
cl_device_id get_device(const cl_context &context, const size_t index)
{
    std::vector<cl_device_id> devices = get_devices(context);
    core_assert(index < devices.size(), "device index out of range");
    return devices[index];
}

} /* Context */
} /* cl */
} /* atto */
