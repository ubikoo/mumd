/*
 * base.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_OPENCL_BASE_H_
#define TEST_OPENCL_BASE_H_

#include "atto/opencl/opencl.hpp"

namespace Params {
    /** OpenCL GPU device index. */
    static const cl_ulong device_index = 2;
    static const cl_ulong work_group_size_1d = 256;
    static const cl_ulong work_group_size_2d = 16;
} /* Params */

#endif /* TEST_OPENCL_BASE_H_ */
