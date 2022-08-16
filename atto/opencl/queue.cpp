/*
 * queue.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opencl/queue.hpp"

namespace atto {
namespace cl {
namespace Queue {

/** ---------------------------------------------------------------------------
 * create
 * @brief Wrapper for clCreateCommandQueue. Create a command_queue on the
 * specified device in the context.
 * @note The cl_command_queue_properties argument specifies execution order.
 * If set to CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, there is no guarantee
 * kernel A finishes before kernel B starts execution.
 */
cl_command_queue create(
    const cl_context &context,
    const cl_device_id &device,
    cl_command_queue_properties properties)
{
    core_assert(
        properties == 0                                     ||
        properties == CL_QUEUE_PROFILING_ENABLE             ||
        properties == CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
        "invalid queue properties");

    cl_int err;
    cl_command_queue queue = clCreateCommandQueue(
        context,
        device,
        properties,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateCommandQueue");
    return queue;
}

/**
 * release
 * @brief Wrapper for clReleaseCommandQueue. Decrements the command_queue
 * reference count.
 */
cl_int release(const cl_command_queue &queue)
{
    cl_int err = CL_INVALID_VALUE;
    if (queue != NULL) {
        err = clReleaseCommandQueue(queue);
        core_assert(err == CL_SUCCESS, "clReleaseCommandQueue");
    }
    return err;
}

/** ---------------------------------------------------------------------------
 * enqueue_read_buffer
 * @brief Wrapper for clEnqueueReadBuffer. Enqueue commands to read from
 * a buffer object to host memory.
 */
cl_int enqueue_read_buffer(
    const cl_command_queue &queue,
    cl_mem &buffer,
    cl_bool blocking,
    size_t offset,
    size_t size,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueReadBuffer(
        queue,
        buffer,
        blocking,
        offset,
        size,
        ptr,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueReadBuffer");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_write_buffer
 * @brief Wrapper for clEnqueueWriteBuffer. Enqueue commands to write to a
 * buffer object from host memory.
 */
cl_int enqueue_write_buffer(
    const cl_command_queue &queue,
    cl_mem &buffer,
    cl_bool blocking,
    size_t offset,
    size_t size,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueWriteBuffer(
        queue,
        buffer,
        blocking,
        offset,
        size,
        ptr,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueWriteBuffer");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_copy_buffer
 * @brief Wrapper for clEnqueueCopyBuffer. Enqueues a command to copy from
 * one buffer object to another.
 */
cl_int enqueue_copy_buffer(
    const cl_command_queue &queue,
  	cl_mem &src_buffer,
  	cl_mem &dst_buffer,
  	size_t src_offset,
  	size_t dst_offset,
  	size_t size,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueCopyBuffer(
        queue,
        src_buffer,
        dst_buffer,
        src_offset,
        dst_offset,
        size,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueCopyBuffer");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_read_buffer_rect
 * @brief Wrapper for clEnqueueReadBufferRect. Enqueue commands to read from
 * a rectangular region from a buffer object to host memory.
 */
cl_int enqueue_read_buffer_rect(
    const cl_command_queue &queue,
  	cl_mem &buffer,
  	cl_bool blocking,
    const std::array<size_t,3> &buffer_origin,
    const std::array<size_t,3> &host_origin,
    const std::array<size_t,3> &region,
  	size_t buffer_row_pitch,
  	size_t buffer_slice_pitch,
  	size_t host_row_pitch,
  	size_t host_slice_pitch,
  	void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueReadBufferRect(
        queue,
        buffer,
        blocking,
        buffer_origin.data(),
        host_origin.data(),
        region.data(),
        buffer_row_pitch,
        buffer_slice_pitch,
        host_row_pitch,
        host_slice_pitch,
        ptr,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueReadBufferRect");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_write_buffer_rect
 * @brief Wrapper for clEnqueueWriteBufferRect. Enqueue commands to write
 * a rectangular region to a buffer object from host memory.
 */
cl_int enqueue_write_buffer_rect(
    const cl_command_queue &queue,
  	cl_mem &buffer,
  	cl_bool blocking,
    const std::array<size_t,3> &buffer_origin,
    const std::array<size_t,3> &host_origin,
    const std::array<size_t,3> &region,
  	size_t buffer_row_pitch,
  	size_t buffer_slice_pitch,
  	size_t host_row_pitch,
  	size_t host_slice_pitch,
  	void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueWriteBufferRect(
        queue,
        buffer,
        blocking,
        buffer_origin.data(),
        host_origin.data(),
        region.data(),
        buffer_row_pitch,
        buffer_slice_pitch,
        host_row_pitch,
        host_slice_pitch,
        ptr,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueWriteBufferRect");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_copy_buffer_rect
 * @brief Wrapper for clEnqueueCopyBufferRect. Enqueues a command to copy
 * a rectangular region from a buffer object to another buffer object.
 */
cl_int enqueue_copy_buffer_rect(
    const cl_command_queue &queue,
  	cl_mem &src_buffer,
  	cl_mem &dst_buffer,
    const std::array<size_t,3> &src_origin,
    const std::array<size_t,3> &dst_origin,
    const std::array<size_t,3> &region,
 	size_t src_row_pitch,
  	size_t src_slice_pitch,
  	size_t dst_row_pitch,
  	size_t dst_slice_pitch,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueCopyBufferRect(
        queue,
        src_buffer,
        dst_buffer,
        src_origin.data(),
        dst_origin.data(),
        region.data(),
        src_row_pitch,
        src_slice_pitch,
        dst_row_pitch,
        dst_slice_pitch,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueCopyBufferRect");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_fill_buffer
 * @brief Wrapper for clEnqueueFillBuffer. Enqueues a command to fill a buffer
 * object with a pattern of a given pattern size. Pattern type must be an
 * accepted OpenCL data type.
 */
cl_int enqueue_fill_buffer(
    const cl_command_queue &queue,
    cl_mem &buffer,
    const void *pattern,
    size_t pattern_size,
    size_t offset,
    size_t size,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueFillBuffer(
        queue,
        buffer,
        pattern,
        pattern_size,
        offset,
        size,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueFillBuffer");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/** ---------------------------------------------------------------------------
 * enqueue_copy_from
 * @brief Wrapper for clEnqueueReadBuffer. Copy data from the specified
 * buffer object on the device to the host memory.
 */
cl_int enqueue_copy_from(
    const cl_command_queue &queue,
    cl_mem &buffer,
    size_t size,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    return enqueue_read_buffer(
        queue,
        buffer,
        CL_TRUE,
        0,
        size,
        ptr,
        event_wait_list,
        event);
}

/**
 * enqueue_copy_to
 * @brief Wrapper for clEnqueueWriteBuffer. Copy an data from the host memory
 * to the specified buffer object on the device.
 */
cl_int enqueue_copy_to(
    const cl_command_queue &queue,
    cl_mem &buffer,
    size_t size,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    return enqueue_write_buffer(
        queue,
        buffer,
        CL_TRUE,
        0,
        size,
        ptr,
        event_wait_list,
        event);
}

/** ---------------------------------------------------------------------------
 * enqueue_read_image
 * @brief Wrapper for clEnqueueReadImage. Enqueue commands to read from an
 * image or image array object to host memory.
 */
cl_int enqueue_read_image(
    const cl_command_queue &queue,
    cl_mem &image,
 	cl_bool blocking,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
    size_t row_pitch,
    size_t slice_pitch,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueReadImage(
        queue,
        image,
        blocking,
        origin.data(),
        region.data(),
        row_pitch,
        slice_pitch,
        ptr,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueReadImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_write_image
 * @brief Wrapper for clEnqueueWriteImage. Enqueues a command to write to an
 * image or image array object from host memory.
 */
cl_int enqueue_write_image(
    const cl_command_queue &queue,
    cl_mem &image,
 	cl_bool blocking,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
    size_t row_pitch,
    size_t slice_pitch,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueWriteImage(
        queue,
        image,
        blocking,
        origin.data(),
        region.data(),
        row_pitch,
        slice_pitch,
        ptr,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueWriteImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_copy_image
 * @brief Wrapper for clEnqueueCopyImage. Enqueues a command to copy image
 * objects.
 */
cl_int enqueue_copy_image(
    const cl_command_queue &queue,
    cl_mem &src_image,
    cl_mem &dst_image,
    const std::array<size_t,3> &src_origin,
    const std::array<size_t,3> &dst_origin,
    const std::array<size_t,3> &region,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueCopyImage(
        queue,
        src_image,
        dst_image,
        src_origin.data(),
        dst_origin.data(),
        region.data(),
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueCopyImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_fill_image
 * @brief Wrapper for clEnqueueFillImage. Enqueues a command to fill an image
 * object with a specified color.
 * @param fill_color is the color to use to fill the image. This is a four
 * component RGBA floating-point color value if the image channel data type
 * is not an unnormalized signed or unsigned data type.
 */
cl_int enqueue_fill_image(
    const cl_command_queue &queue,
    cl_mem &image,
    const cl_float4 fill_color,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueFillImage(
        queue,
        image,
        static_cast<const void *>(&fill_color),
        origin.data(),
        region.data(),
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueFillImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_fill_image
 * @brief Wrapper for clEnqueueFillImage. Enqueues a command to fill an image
 * object with a specified color.
 * @param fill_color is the color to use to fill the image. This is a four
 * component RGBA signed integer color value if the image channel data type
 * is an unnormalized signed integer type.
 */
cl_int enqueue_fill_image(
    const cl_command_queue &queue,
    cl_mem &image,
    const cl_int4 fill_color,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueFillImage(
        queue,
        image,
        static_cast<const void *>(&fill_color),
        origin.data(),
        region.data(),
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueFillImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_fill_image
 * @brief Wrapper for clEnqueueFillImage. Enqueues a command to fill an image
 * object with a specified color.
 * @param fill_color is the color to use to fill the image. This is a four
 * component RGBA unsigned integer color value if the image channel data type
 * is an unnormalized signed integer type.
 */
cl_int enqueue_fill_image(
    const cl_command_queue &queue,
    cl_mem &image,
    const cl_uint4 fill_color,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueFillImage(
        queue,
        image,
        static_cast<const void *>(&fill_color),
        origin.data(),
        region.data(),
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueFillImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/** ---------------------------------------------------------------------------
 * enqueue_copy_image_to_buffer
 * @brief Wrapper for clEnqueueCopyImageToBuffer. Enqueues a command to copy
 * an image object to a buffer object.
 */
cl_int enqueue_copy_image_to_buffer(
    const cl_command_queue &queue,
    cl_mem &src_image,
    cl_mem &dst_buffer,
    const std::array<size_t,3> &src_origin,
    const std::array<size_t,3> &region,
    size_t dst_offset,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueCopyImageToBuffer(
        queue,
        src_image,
        dst_buffer,
        src_origin.data(),
        region.data(),
        dst_offset,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueCopyImageToBuffer");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_copy_buffer_to_image
 * @brief Wrapper for clEnqueueCopyBufferToImage. Enqueues a command to copy
 * a buffer object to an image object.
 */
cl_int enqueue_copy_buffer_to_image(
    const cl_command_queue &queue,
    cl_mem &src_buffer,
    cl_mem &dst_image,
    size_t src_offset,
    const std::array<size_t,3> &dst_origin,
    const std::array<size_t,3> &region,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueCopyBufferToImage(
        queue,
        src_buffer,
        dst_image,
        src_offset,
        dst_origin.data(),
        region.data(),
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueCopyBufferToImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/** ---------------------------------------------------------------------------
 * enqueue_map_buffer
 * @brief Wrapper for clEnqueueMapBuffer. Enqueues a command to map a region
 * of the buffer object given by buffer into the host address space and
 * returns a pointer to this mapped region.
 */
void *enqueue_map_buffer(
    const cl_command_queue &queue,
    cl_mem &buffer,
    cl_bool blocking,
    cl_map_flags flags,
  	size_t offset,
  	size_t size,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event,
    cl_int *errcode)
{
    cl_event tmp;
    cl_int err;
    void *result = clEnqueueMapBuffer(
        queue,
        buffer,
        blocking,
        flags,
        offset,
        size,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL,
        &err);
    core_assert(err == CL_SUCCESS, "clEnqueueMapBuffer");

    if (errcode != NULL) {
        *errcode = err;
    }

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return result;
}

/**
 * enqueue_map_image
 * @brief Wrapper for clEnqueueMapImage. Enqueues a command to map a region
 * of an image object into the host address space and returns a pointer to
 * this mapped region.
 */
void *enqueue_map_image(
    const cl_command_queue &queue,
    cl_mem &image,
    cl_bool blocking,
    cl_map_flags flags,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
  	size_t *row_pitch,
  	size_t *slice_pitch,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event,
    cl_int *errcode)
{
    cl_event tmp;
    cl_int err;
    void *result = clEnqueueMapImage(
        queue,
        image,
        blocking,
        flags,
        origin.data(),
        region.data(),
        row_pitch,
        slice_pitch,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL,
        &err);
    core_assert(err == CL_SUCCESS, "clEnqueueMapImage");

    if (errcode != NULL) {
        *errcode = err;
    }

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return result;
}

/**
 * enqueue_unmap_mem_object
 * @brief Wrapper for clEnqueueUnmapMemObject. Enqueues a command to unmap
 * a previously mapped region of a memory object.
 */
cl_int enqueue_unmap_mem_object(
    const cl_command_queue &queue,
    cl_mem &mem_object,
    void *mapped_ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueUnmapMemObject(
        queue,
        mem_object,
        mapped_ptr,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueUnmapMemObject");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_migrate_mem_objects
 * @brief Wrapper for clEnqueueMigrateMemObjects. Enqueues a command to
 * indicate which device a set of memory objects should be associated with.
 */
cl_int enqueue_migrate_mem_objects(
    const cl_command_queue &queue,
    const std::vector<cl_mem> &mem_objects,
    cl_mem_migration_flags flags,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueMigrateMemObjects(
        queue,
        static_cast<cl_uint>(mem_objects.size()),
        mem_objects.data(),
        flags,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueMigrateMemObjects");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/** ---------------------------------------------------------------------------
 * enqueue_marker_with_waitlist
 * @brief Wrapper for clEnqueueMarkerWithWaitList. Enqueues a marker command
 * which waits for either a list of events to complete, or all previously
 * enqueued commands to complete.
 */
cl_int enqueue_marker_with_waitlist(
    const cl_command_queue &queue,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueMarkerWithWaitList(
        queue,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueMarkerWithWaitList");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_barrier_with_waitlist
 * @brief Wrapper for clEnqueueBarrierWithWaitList. Enqueues a barrier command
 * which waits for either a list of events to complete, or if the list is empty
 * it waits for all commands previously enqueued in command_queue to complete
 * before it completes.
 */
cl_int enqueue_barrier_with_waitlist(
    const cl_command_queue &queue,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueBarrierWithWaitList(
        queue,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueBarrierWithWaitList");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/** ---------------------------------------------------------------------------
 * enqueue_nd_range_kernel
 * @brief Wrapper for clEnqueueNDRangeKernel. Enqueues a command to execute
 * a kernel on a device.
 */
cl_int enqueue_nd_range_kernel(
    const cl_command_queue &queue,
    const cl_kernel &kernel,
    const NDRange &global_work_offset,
    const NDRange &global_work_size,
    const NDRange &local_work_size,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueNDRangeKernel(
        queue,
        kernel,
        global_work_size.dim(),
        global_work_offset.dim() != 0 ? global_work_offset.data() : NULL,
        global_work_size.data(),
        local_work_size.dim() != 0 ? local_work_size.data() : NULL,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueNDRangeKernel");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_task
 * @brief Wrapper for clEnqueueTask. Enqueues a command to execute a kernel
 * on a device.
 */
cl_int enqueue_task(
    const cl_command_queue &queue,
    const cl_kernel &kernel,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueTask(
        queue,
        kernel,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueTask");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/**
 * enqueue_native_kernel
 * @brief Wrapper for clEnqueueNativeKernel. Enqueues a command to execute
 * a native C/C++ function not compiled using the OpenCL compiler.
 */
cl_int enqueue_native_kernel(
    const cl_command_queue &queue,
    void (*user_func)(void *),
    std::pair<void *, size_t> &args,
    std::vector<cl_mem> *mem_objects,
    std::vector<const void *> *mem_locs,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    cl_event tmp;
    cl_int err = clEnqueueNativeKernel(
        queue,
        user_func,
        args.first,
        args.second,
        (mem_objects != NULL)
            ? static_cast<cl_uint>(mem_objects->size()) : 0,
        (mem_objects != NULL)
            ? mem_objects->data() : NULL,
        (mem_locs != NULL && !mem_locs->empty())
            ? mem_locs->data() : NULL,
        (event_wait_list != NULL)
            ? static_cast<cl_uint>(event_wait_list->size())
            : 0,
        (event_wait_list != NULL && !event_wait_list->empty())
            ? event_wait_list->data()
            : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueNativeKernel");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }

    return err;
}

/** ---------------------------------------------------------------------------
 * flush
 * @brief Wrapper for clFlush. Issues all previously queued OpenCL commands
 * in a command-queue to the device associated with the command-queue.
 */
cl_int flush(const cl_command_queue &queue)
{
    cl_int err = clFlush(queue);
    core_assert(err == CL_SUCCESS, "clFlush");
    return err;
}

/**
 * finish
 * @brief Wrapper for clFinish. Blocks until all previously queued OpenCL
 * commands in a command-queue are issued to the associated device and
 * have completed.
 */
cl_int finish(const cl_command_queue &queue)
{
    cl_int err = clFinish(queue);
    core_assert(err == CL_SUCCESS, "clFinish");
    return err;
}

} /* Queue */
} /* cl */
} /* atto */
