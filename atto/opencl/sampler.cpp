/*
 * sampler.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opencl/sampler.hpp"

namespace atto {
namespace cl {
namespace Sampler {

/**
 * create
 * @brief Wrapper for clCreateBuffer. Creates a buffer memory object in the
 * specified context.
 * @param normalized_coords Determines if the image coordinates specified are
 * normalized (CL_TRUE) or not (CL_FALSE).
 * @param addressing_mode Specifies how out-of-range image coordinates are
 * handled when reading from an image. This can be set to
 *  CL_ADDRESS_MIRRORED_REPEAT,
 *  CL_ADDRESS_REPEAT,
 *  CL_ADDRESS_CLAMP_TO_EDGE,
 *  CL_ADDRESS_CLAMP,
 *  CL_ADDRESS_NONE.
 * @param filter_mode Specifies the type of filter that must be applied when
 * reading an image. This can be CL_FILTER_NEAREST or CL_FILTER_LINEAR.
 */
cl_sampler create(
    const cl_context &context,
    cl_bool normalized_coords,
    cl_addressing_mode addressing_mode,
    cl_filter_mode filter_mode)
{
    cl_int err;
    cl_sampler sampler = clCreateSampler(
        context,
        normalized_coords,
        addressing_mode,
        filter_mode,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateSampler");
    return sampler;
}

/**
 * release
 * @brief Wrapper for clReleaseSampler. Decrements the sampler reference count.
 */
cl_int release(const cl_sampler &sampler)
{
    cl_int err = CL_INVALID_VALUE;
    if (sampler != NULL) {
        err = clReleaseSampler(sampler);
        core_assert(err == CL_SUCCESS, " clReleaseSampler");
    }
    return err;
}

} /* Sampler */
} /* cl */
} /* atto */
