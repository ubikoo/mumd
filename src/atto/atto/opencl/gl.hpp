/*
 * gl.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_GL_H_
#define ATTO_OPENCL_GL_H_

#include <vector>
#include "atto/opencl/base.hpp"
#include "atto/opencl/queue.hpp"

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
    GLuint buffer_object);

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
    GLuint texture_object);

/**
 * create_from_gl_renderbuffer
 * @brief Wrapper for clCreateFromGLRenderbuffer. Creates an OpenCL 2D image
 * object from an OpenGL renderbuffer object.
 */
cl_mem create_from_gl_renderbuffer(
    const cl_context &context,
    cl_mem_flags flags,
    GLuint renderbuffer);

/** ---------------------------------------------------------------------------
 * get_gl_object_info
 * @brief Wrapper for clGetGLObjectInfo. Query an OpenGL memory object used
 * to create an OpenCL memory object.
 */
cl_int get_gl_object_info(
    const cl_mem &mem_object,
    cl_gl_object_type *gl_object_type = NULL,
    GLuint *gl_object_name = NULL);

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
    size_t *param_value_size_ret = NULL);

/** ---------------------------------------------------------------------------
 * enqueue_acquire_gl_objects
 * @brief Wrapper for clEnqueueAcquireGLObjects. Acquire OpenCL memory objects
 * that have been created from OpenGL objects.
 */
cl_int enqueue_acquire_gl_objects(
    const cl_command_queue &queue,
    const cl_uint num_objects = 0,
    const cl_mem *mem_objects = NULL,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/**
 * enqueue_acquire_gl_objects
 * @brief Wrapper for clEnqueueAcquireGLObjects. Overloaded function using a
 * vector of mem objects.
 */
cl_int enqueue_acquire_gl_objects(
    const cl_command_queue &queue,
    const std::vector<cl_mem> *mem_objects = NULL,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/**
 * enqueue_release_gl_objects
 * @brief Wrapper for clEnqueueReleaseGLObjects. Release OpenCL memory objects
 * that have been created from OpenGL objects.
 */
cl_int enqueue_release_gl_objects(
    const cl_command_queue &queue,
    const cl_uint num_objects = 0,
    const cl_mem *mem_objects = NULL,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/**
 * enqueue_release_gl_objects
 * @brief Wrapper for clEnqueueReleaseGLObjects. Overloaded function using a
 * vector of mem objects.
 */
cl_int enqueue_release_gl_objects(
    const cl_command_queue &queue,
    const std::vector<cl_mem> *mem_objects = NULL,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

} /* gl */
} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_GL_H_ */
