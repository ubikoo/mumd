/*
 * platform.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_PLATFORM_H_
#define ATTO_OPENCL_PLATFORM_H_

#include <vector>
#include "atto/opencl/base.hpp"

namespace atto {
namespace cl {
namespace Platform {

/**
 * get
 * @brief Wrapper for clGetPlatformIDs. Return a vector of all
 * platforms currently available.
 */
std::vector<cl_platform_id> get_platform_ids(void);

/**
 * get_info_string
 * @brief Wrapper for clGetPlatformInfo. Return a string with common
 * platform information.
 */
std::string get_info_string(const cl_platform_id &platform);

} /* Platform */
} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_PLATFORM_H_ */
