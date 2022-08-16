/*
 * ndrange.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opencl/ndrange.hpp"

namespace atto {
namespace cl {

/**
 * NDRange::Null
 * @brief A zero-dimensional range.
 */
const NDRange NDRange::Null;

/**
 * NDRange::Roundup
 * @brief Round up global work size to the next multiple of local work size.
 */
size_t NDRange::Roundup(size_t global_work_size, size_t local_work_size)
{
    /*
     * Compute the remainder r of global_work_size mod local_work_size.
     * If r is zero, global_work_size is a multiple of local_work_size.
     * Otherwise,
     *  (global_work_size - r) is the largest multiple of local_work_size
     *  smaller than global_work_size, and
     *  (global_work_size - r) + local_work_size is the smallest multiple
     *  of local_work_size larger than global_work_size.
     */
    size_t r = global_work_size % local_work_size;
    return (r == 0) ? global_work_size : global_work_size + local_work_size - r;
}

/**
 * NDRange::Make
 * @brief Make a new NDRange with global work size rounded up to the next
 * multiple of local work size.
 */
NDRange NDRange::Make(size_t global_work_size, size_t local_work_size)
{
    return NDRange(NDRange::Roundup(global_work_size, local_work_size));
}

} /* cl */
} /* atto */

