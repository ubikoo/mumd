/*
 * image.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_IMAGE_H_
#define ATTO_OPENCL_IMAGE_H_

#include <vector>
#include "atto/opencl/base.hpp"

namespace atto {
namespace cl {

/** ---------------------------------------------------------------------------
 * convert_image_to_float
 * @brief Convert image to normalized OpenCL floating point data type.
 */
std::vector<cl_float> convert_image_to_float(const atto::gl::Image &image);

/**
 * convert_image_to_float2
 * @brief Convert image to normalized OpenCL 2d-vector data type.
 */
std::vector<cl_float2> convert_image_to_float2(const atto::gl::Image &image);

/**
 * convert_image_to_float3
 * @brief Convert image to normalized OpenCL 3d-vector data type.
 */
std::vector<cl_float3> convert_image_to_float3(const atto::gl::Image &image);

/**
 * convert_image_to_float4
 * @brief Convert image to normalized OpenCL 4d-vector data type.
 */
std::vector<cl_float4> convert_image_to_float4(const atto::gl::Image &image);

} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_IMAGE_H_ */
