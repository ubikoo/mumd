/*
 * gl.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opencl/gl.hpp"

namespace atto {
namespace cl {
namespace gl {

/** ---------------------------------------------------------------------------
 * create_from_gl_buffer
 * @brief Wrapper for clCreateFromGLBuffer. Creates an OpenCL buffer object
 * from an OpenGL buffer object.
 */
cl_mem create_from_gl_buffer(
    const cl_context &context,
    cl_mem_flags flags,
    GLuint buffer_object)
{
    core_assert(
        flags == CL_MEM_READ_ONLY  ||
        flags == CL_MEM_WRITE_ONLY ||
        flags == CL_MEM_READ_WRITE,
        "invalid buffer usage flags");

    cl_int err;
    cl_mem buffer = clCreateFromGLBuffer(
        context,
        flags,
        buffer_object,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateFromGLBuffer");
    return buffer;
}

/**
 * create_from_gl_texture
 * @brief Wrapper for clCreateFromGLTexture. Creates an OpenCL image object,
 * image array object, or image buffer object from an OpenGL texture object,
 * texture array object, texture buffer object, or a single face of an OpenGL
 * cubemap texture object.
 */
cl_mem create_from_gl_texture(
    const cl_context &context,
  	cl_mem_flags flags,
    GLenum texture_target,
    GLint miplevel,
    GLuint texture_object)
{
    core_assert(
        flags == CL_MEM_READ_ONLY  ||
        flags == CL_MEM_WRITE_ONLY ||
        flags == CL_MEM_READ_WRITE,
        "invalid buffer usage flags");
    core_assert(
        texture_target == GL_TEXTURE_1D ||
        texture_target == GL_TEXTURE_2D ||
        texture_target == GL_TEXTURE_3D ||
        texture_target == GL_TEXTURE_BUFFER,
        "invalid texture target");
    core_assert(miplevel == 0, "invalid mipmap level");

    cl_int err;
    cl_mem image = clCreateFromGLTexture(
        context,
        flags,
        texture_target,
        miplevel,
        texture_object,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateFromGLBuffer");
    return image;
}

/**
 * create_from_gl_renderbuffer
 * @brief Wrapper for clCreateFromGLRenderbuffer. Creates an OpenCL 2D image
 * object from an OpenGL renderbuffer object.
 */
cl_mem create_from_gl_renderbuffer(
    const cl_context &context,
    cl_mem_flags flags,
    GLuint renderbuffer)
{
    core_assert(
        flags == CL_MEM_READ_ONLY  ||
        flags == CL_MEM_WRITE_ONLY ||
        flags == CL_MEM_READ_WRITE,
        "invalid buffer usage flags");

    cl_int err;
    cl_mem image = clCreateFromGLRenderbuffer(
        context,
        flags,
        renderbuffer,
        &err);
    core_assert(err == CL_SUCCESS, "clCreateFromGLBuffer");
    return image;
}

/** ---------------------------------------------------------------------------
 * get_gl_object_info
 * @brief Wrapper for clGetGLObjectInfo. Query an OpenGL memory object used
 * to create an OpenCL memory object.
 */
cl_int get_gl_object_info(
    const cl_mem &mem_object,
    cl_gl_object_type *gl_object_type,
    GLuint *gl_object_name)
{
    cl_int err = clGetGLObjectInfo(mem_object, gl_object_type, gl_object_name);
    core_assert(err == CL_SUCCESS, "clGetGLObjectInfo");
    return err;
}

/**
 * get_gl_texture_info
 * @brief Wrapper for clGetGLTextureInfo. Returns additional information
 * about the GL texture object associated with a memory object.
 */
cl_int get_gl_texture_info(
    const cl_mem &mem_object,
    cl_gl_texture_info param_name,
    size_t param_value_size,
    void *param_value,
    size_t *param_value_size_ret)
{
    core_assert(
        param_name == CL_GL_TEXTURE_TARGET ||
        param_name == CL_GL_MIPMAP_LEVEL,
        "invalid GL texture param name");

    cl_int err = clGetGLTextureInfo(
        mem_object,
        param_name,
        param_value_size,
        param_value,
        param_value_size_ret);
    core_assert(err == CL_SUCCESS, "clGetGLObjectInfo");
    return err;
}

/** ---------------------------------------------------------------------------
 * enqueue_acquire_gl_objects
 * @brief Wrapper for clEnqueueAcquireGLObjects. Acquire OpenCL memory objects
 * that have been created from OpenGL objects.
 * @return CL_INVALID_VALUE if num_objects is zero and mem_objects is not a
 * NULL value or if num_objects > 0 and mem_objects is NULL.
 */
cl_int enqueue_acquire_gl_objects(
    const cl_command_queue &queue,
    const cl_uint num_objects,
    const cl_mem *mem_objects,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    /*
     * Ensure that any OpenGL commands that might affect the shared OpenGL
     * memory objects have completed before acquiring them.
     */
    glFinish();

    /* Acquire the shared OpenGL memory objects. */
    cl_event tmp;
    cl_int err = clEnqueueAcquireGLObjects(
        queue,
        (mem_objects != NULL) ? num_objects : 0,
        (mem_objects != NULL && num_objects > 0) ? mem_objects : NULL,
        (event_wait_list != NULL) ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        (event_wait_list != NULL && event_wait_list->size() > 0) ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueAcquireGLObjects");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * enqueue_acquire_gl_objects
 * @brief Wrapper for clEnqueueAcquireGLObjects. Overloaded function using a
 * vector of mem objects.
 */
cl_int enqueue_acquire_gl_objects(
    const cl_command_queue &queue,
    const std::vector<cl_mem> *mem_objects,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    return enqueue_acquire_gl_objects(
        queue,
        (mem_objects != NULL) ? static_cast<cl_uint>(mem_objects->size()) : 0,
        (mem_objects != NULL && mem_objects->size() > 0) ? mem_objects->data() : NULL,
        event_wait_list,
        event);
}

/**
 * enqueue_release_gl_objects
 * @brief Wrapper for clEnqueueReleaseGLObjects. Release OpenCL memory objects
 * that have been created from OpenGL objects.
 * @return CL_INVALID_VALUE if num_objects is zero and mem_objects is not a
 * NULL value or if num_objects > 0 and mem_objects is NULL.
 */
cl_int enqueue_release_gl_objects(
    const cl_command_queue &queue,
    const cl_uint num_objects,
    const cl_mem *mem_objects,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    /*
     * Ensure that any OpenCL commands that might affect the shared OpenGL
     * memory objects are finished before releasing them.
     */
    Queue::finish(queue);

    /* Release the shared OpenGL memory objects. */
    cl_event tmp;
    cl_int err = clEnqueueReleaseGLObjects(
        queue,
        (mem_objects != NULL) ? num_objects : 0,
        (mem_objects != NULL && num_objects > 0) ? mem_objects : NULL,
        (event_wait_list != NULL) ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        (event_wait_list != NULL && event_wait_list->size() > 0) ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    core_assert(err == CL_SUCCESS, "clEnqueueReleaseGLObjects");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * enqueue_release_gl_objects
 * @brief Wrapper for clEnqueueReleaseGLObjects. Overloaded function using a
 * vector of mem objects.
 */
cl_int enqueue_release_gl_objects(
    const cl_command_queue &queue,
    const std::vector<cl_mem> *mem_objects,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    return enqueue_release_gl_objects(
        queue,
        (mem_objects != NULL) ? static_cast<cl_uint>(mem_objects->size()) : 0,
        (mem_objects != NULL && mem_objects->size() > 0) ? mem_objects->data() : NULL,
        event_wait_list,
        event);
}

} /* gl */
} /* cl */
} /* atto */
