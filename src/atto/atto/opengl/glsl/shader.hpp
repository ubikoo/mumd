/*
 * shader.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GLSL_SHADER_H_
#define ATTO_OPENGL_GLSL_SHADER_H_

#include "atto/opengl/base.hpp"

namespace atto {
namespace gl {

/** ---- Shader ---------------------------------------------------------------
 * @brief Shader represents a shader stage of a given type in the
 * rendering pipeline.
 * It contains the type and source of a shader which are used to create
 * a shader program object. It recognises the four stage types:
 *      GL_VERTEX_SHADER,
 *      GL_GEOMETRY_SHADER,
 *      GL_FRAGMENT_SHADER and
 *      GL_COMPUTE_SHADER (GL version >= 4.3).
 * Each stage has a set of inputs and outputs, which are passed from
 * prior stages and on to subsequent ones (programmable or not).
 * The stages - vertex, geometry and fragment - have a fixed, ordered
 * position in the rendering pipeline.
 * Compute shader is a stage used entirely for computing arbitrary
 * information. While it can do rendering, it is generally used for
 * tasks not directly related to drawing triangles or pixels.
 *
 * @see https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language
 *      https://www.khronos.org/opengl/wiki/Shader#Stages
 *      https://www.khronos.org/opengl/wiki/Compute_Shader
 */
struct Shader {
    /* Shader stage type and source string. */
    GLenum m_type;              /* shader type */
    std::string m_source;       /* shader source */

    const GLenum &type(void) const { return m_type; }
    const std::string &source(void) const { return m_source; }

    /* Concatenate the source to the shader source string. */
    void append(const std::string &source) { m_source += source; }

    /* Constructor/destructor. */
    explicit Shader(const GLenum type) : m_type(type) {}
    explicit Shader(const GLenum type, const std::string &source)
        : m_type(type)
        , m_source(source) {}
    ~Shader() = default;

    /* Copy constructor/assignment. */
    Shader(const Shader &other)
        : m_type(other.m_type)
        , m_source(other.m_source) {}
    Shader &operator=(const Shader &other) {
        if (this == &other) {
            return *this;
        }
        m_type = other.m_type;
        m_source = other.m_source;
        return *this;
    }
};

/**
 * create_shader
 * @brief Create a new shader object.
 */
GLuint create_shader(const Shader &stage);

/**
 * create_shader
 * @brief Create a new shader stage of a given type from the specified file.
 */
GLuint create_shader(const GLenum type, const std::string &filename);

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GLSL_SHADER_H_ */
