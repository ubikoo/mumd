/*
 * memory.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opencl/memory.hpp"

namespace atto {
namespace cl {
namespace Memory {

/** ---------------------------------------------------------------------------
 * create_buffer
 * @brief Wrapper for clCreateBuffer. Creates a buffer memory object in the
 * specified context.
 * @param flags Bitfield specifying the buffer memory usage and allocation.
 *  CL_MEM_READ_WRITE
 *  CL_MEM_WRITE_ONLY
 *  CL_MEM_READ_ONLY
 *  CL_MEM_USE_HOST_PTR
 *  CL_MEM_ALLOC_HOST_PTR
 *  CL_MEM_COPY_HOST_PTR
 * @param size Size in bytes of the buffer memory object to be allocated.
 * @param host_ptr Pointer to the buffer data that may already be allocated
 * by the host application.
 */
cl_mem create_buffer(
    const cl_context &context,
    cl_mem_flags flags,
    size_t size,
    void *host_ptr)
{
    cl_int err;
    cl_mem buffer = clCreateBuffer(
        context,
        flags,
        size,
        host_ptr,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateBuffer");
    return buffer;
}

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
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 1d-image. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE1D,          /* cl_mem_object_type image type */
        width,                          /* image width */
        0,                              /* image height */
        0,                              /* image depth */
        0,                              /* image array size */
        0,                              /* image row pitch */
        0,                              /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        0};                             /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

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
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 1d-image buffer. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE1D_BUFFER,   /* cl_mem_object_type image type */
        width,                          /* image width */
        0,                              /* image height */
        0,                              /* image depth */
        0,                              /* image array size */
        0,                              /* image row pitch */
        0,                              /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        buffer};                        /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

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
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 1d-image array. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE1D_ARRAY,    /* cl_mem_object_type image type */
        width,                          /* image width */
        0,                              /* image height */
        0,                              /* image depth */
        array_size,                     /* image array size */
        row_pitch,                      /* image row pitch */
        0,                              /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        0};                             /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

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
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 2d-image. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE2D,          /* cl_mem_object_type image type */
        width,                          /* image width */
        height,                         /* image height */
        0,                              /* image depth */
        0,                              /* image array size */
        row_pitch,                      /* image row pitch */
        0,                              /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        0};                             /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

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
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 2d-image array. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE2D_ARRAY,    /* cl_mem_object_type image type */
        width,                          /* image width */
        height,                         /* image height */
        0,                              /* image depth */
        array_size,                     /* image array size */
        row_pitch,                      /* image row pitch */
        slice_pitch,                    /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        0};                             /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

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
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 3d-image. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE3D,          /* cl_mem_object_type image type */
        width,                          /* image width */
        height,                         /* image height */
        depth,                          /* image depth */
        0,                              /* image array size */
        row_pitch,                      /* image row pitch */
        slice_pitch,                    /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        0};                             /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

/** ---------------------------------------------------------------------------
 * release
 * @brief Wrapper for clReleaseMemObject. Decrements the memory object
 * reference count.
 */
cl_int release(const cl_mem &memobj)
{
    cl_int err = CL_INVALID_VALUE;
    if (memobj != NULL) {
        err = clReleaseMemObject(memobj);
        core_assert(err == CL_SUCCESS, "clReleaseMemObject");
    }
    return err;
}

} /* Memory */
} /* cl */
} /* atto */
