/*
 * sampler.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_SAMPLER_H_
#define ATTO_OPENCL_SAMPLER_H_

#include "atto/opencl/base.hpp"

namespace atto {
namespace cl {
namespace Sampler {

/**
 * create
 * @brief Wrapper for clCreateBuffer. Creates a buffer memory object in the
 * specified context.
 */
cl_sampler create(
    const cl_context &context,
    cl_bool normalized_coords,
    cl_addressing_mode addressing_mode,
    cl_filter_mode filter_mode);

/**
 * release
 * @brief Wrapper for clReleaseSampler. Decrements the sampler reference count.
 */
cl_int release(const cl_sampler &sampler);

} /* Sampler */
} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_SAMPLER_H_ */
