/*
 * memory.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_MEMORY_H_
#define ATTO_OPENCL_MEMORY_H_

#include "atto/opencl/base.hpp"
#include "atto/opencl/device.hpp"
#include "atto/opencl/context.hpp"

namespace atto {
namespace cl {
namespace Memory {

/** ---------------------------------------------------------------------------
 * create_buffer
 * @brief Wrapper for clCreateBuffer. Creates a buffer memory object in the
 * specified context.
 */
cl_mem create_buffer(
    const cl_context &context,
    cl_mem_flags flags,
    size_t size,
    void *host_ptr = NULL);

/** ---------------------------------------------------------------------------
 * create_image1d
 * @brief Create a 1d-image of type CL_MEM_OBJECT_IMAGE1D.
 * Size of buffer that host_ptr points to >= row_pitch.
 */
cl_mem create_image1d(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    void *host_ptr = NULL);

/**
 * create_image1d_buffer
 * @brief Create a 1d-image buffer of type CL_MEM_OBJECT_IMAGE1D_BUFFER.
 * Size of buffer that host_ptr points to >= row_pitch.
 */
cl_mem create_image1d_buffer(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    const cl_mem &buffer,
    void *host_ptr = NULL);

/**
 * create_image1d_array
 * @brief Create a 1d-image buffer of type CL_MEM_OBJECT_IMAGE1D_ARRAY.
 * Size of buffer that host_ptr points to >= array_size * row_pitch.
 */
cl_mem create_image1d_array(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    size_t array_size,
    size_t row_pitch,
    void *host_ptr = NULL);

/**
 * create_image2d
 * @brief Create a 2d-image buffer of type CL_MEM_OBJECT_IMAGE2D.
 * Size of buffer that host_ptr points to >= height * row_pitch.
 */
cl_mem create_image2d(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    size_t height,
    size_t row_pitch,
    void *host_ptr = NULL);

/**
 * create_image2d_array
 * @brief Create a 2d-image buffer of type CL_MEM_OBJECT_IMAGE2D_ARRAY.
 * Size of buffer that host_ptr points to >= slice_pitch * array_size.
 */
cl_mem create_image2d_array(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    size_t height,
    size_t array_size,
    size_t row_pitch,
    size_t slice_pitch,
    void *host_ptr = NULL);

/**
 * create_image3d
 * @brief Create a 1d-image buffer of type CL_MEM_OBJECT_IMAGE3D.
 * Size of buffer that host_ptr points to >= slice_pitch * depth.
 */
cl_mem create_image3d(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    size_t height,
    size_t depth,
    size_t row_pitch,
    size_t slice_pitch,
    void *host_ptr = NULL);

/** ---------------------------------------------------------------------------
 * release
 * @brief Wrapper for clReleaseMemObject. Decrements the memory object
 * reference count.
 */
cl_int release(const cl_mem &memobj);

} /* Memory */
} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_MEMORY_H_ */
