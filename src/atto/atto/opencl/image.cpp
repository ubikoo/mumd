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

#include "atto/opencl/image.hpp"

namespace atto {
namespace cl {

/** ---------------------------------------------------------------------------
 * convert_image_to_float
 * @brief Convert image to normalized OpenCL floating point data type.
 */
std::vector<cl_float> convert_image_to_float(const atto::gl::Image &image)
{
    core_assert(!image.is_empty(), "empty image bitmap");

    const uint32_t n_channels = image.bpp() / 8;
    const uint8_t *px = image.bitmap();
    std::vector<cl_float> data;

    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            for (uint32_t c = 0; c < n_channels; ++c) {
                cl_float v = px[c] / 255.0f;
                data.emplace_back(v);
            }
            px += n_channels;
        }
    }

    return data;
}

/**
 * convert_image_to_float2
 * @brief Convert image to normalized OpenCL 2d-vector data type.
 */
std::vector<cl_float2> convert_image_to_float2(const atto::gl::Image &image)
{
    core_assert(!image.is_empty(), "empty image bitmap");

    const uint32_t n_channels = image.bpp() / 8;
    const uint8_t *px = image.bitmap();
    std::vector<cl_float2> data;

    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            cl_float2 v = {
                n_channels > 0 ? px[0] / 255.0f : 0.0f,
                n_channels > 1 ? px[1] / 255.0f : 0.0f};
            data.emplace_back(v);
            px += n_channels;
        }
    }

    return data;
}

/**
 * convert_image_to_float3
 * @brief Convert image to normalized OpenCL 3d-vector data type.
 */
std::vector<cl_float3> convert_image_to_float3(const atto::gl::Image &image)
{
    core_assert(!image.is_empty(), "empty image bitmap");

    const uint32_t n_channels = image.bpp() / 8;
    const uint8_t *px = image.bitmap();
    std::vector<cl_float3> data;

    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            cl_float3 v = {
                n_channels > 0 ? px[0] / 255.0f : 0.0f,
                n_channels > 1 ? px[1] / 255.0f : 0.0f,
                n_channels > 2 ? px[2] / 255.0f : 0.0f};
            data.emplace_back(v);
            px += n_channels;
        }
    }

    return data;
}

/**
 * convert_image_to_float4
 * @brief Convert image to normalized OpenCL 4d-vector data type.
 */
std::vector<cl_float4> convert_image_to_float4(const atto::gl::Image &image)
{
    core_assert(!image.is_empty(), "empty image bitmap");

    const uint32_t n_channels = image.bpp() / 8;
    const uint8_t *px = image.bitmap();
    std::vector<cl_float4> data;

    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            cl_float4 v = {
                n_channels > 0 ? px[0] / 255.0f : 0.0f,
                n_channels > 1 ? px[1] / 255.0f : 0.0f,
                n_channels > 2 ? px[2] / 255.0f : 0.0f,
                n_channels > 3 ? px[3] / 255.0f : 0.0f};
            data.emplace_back(v);
            px += n_channels;
        }
    }

    return data;
}

} /* cl */
} /* atto */
