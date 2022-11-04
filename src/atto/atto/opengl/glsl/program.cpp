/*
 * program.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/glsl/program.hpp"

namespace atto {
namespace gl {

/**
 * create_program
 * @brief Create a shader program object from a set of shader objects.
 *
 * A shader program object contains the exection code for all shader stages.
 * It requires at least two shader stages - vertex and fragment. Each shader
 * stage is compiled and the resulting objects are attached to the program
 * and linked.
 * The sole purpose of shader objects is to be linked to a shader program
 * object, after which time, they are detached from the program and deleted.
 * Each shader object is assumed to be uniquely attached to a single program.
 *
 *  1. For each shader stage:
 *      glCreateShader -> glShaderSource -> glCompileShader
 *  2. Create a new program:
 *      glCreateProgram
 *  3. For each shader object:
 *      glAttachShader
 *  4. Link program:
 *      glLinkProgram
 *  5. For each shader object:
 *      glDetachShader -> glDeleteShader
 *  6. Setup map of active uniform variables (optional).
 *  7. Setup map of active attribute variables (optional).
 *
 * - Vertex shader attribute index
 * The vertex shader input attributes must have their location indices
 * manually specified via the syntax:
 *
 *      layout(location = attribute_index) in vec3 value;
 *
 * With this syntax, use of glBindAttribLocation is forgone entirely.
 * Attributes that take up multiple attribute slots will be given a
 * sequential block of that number of attributes in order starting
 * with the given attribute. For example, the layout specifier
 *
 *      layout(location = 2) in vec3 values[4];
 *
 * will allocate the attribute indices 2, 3, 4, and 5.
 * Program assumes that shader active attributes have their layout indices
 * manually specified and queries their locations via glGetAttribLocation.
 *
 * - Fragment shader buffer output
 * The fragment shader outputs must also have their location indices
 * specified via the syntax:
 *
 *      layout(location = output_index) out vec4 output_color;
 *
 * Here output_color is an output value in your vertex shader and an input
 * value in your fragment shader. With this syntax, use of
 * glBindFragDataLocation is forgone entirely.
 *
 * @note Consider tagging variables with an index, such that they correspond
 * by the index instead of by name. This changes the interface matching,
 * allowing outputs that aren't consumed by the next stage and so forth.
 * For example, given a vertex shader that provides the outputs:
 *
 *      layout(location = 0) out vec4 out_color;
 *      layout(location = 1) out vec2 out_position;
 *      layout(location = 2) out vec3 out_normal;
 *
 * This allows the consuming shader to use different names:
 *
 *      layout(location = 0) out vec4 inp_color;
 *      layout(location = 1) out vec2 inp_position;
 *      layout(location = 2) out vec3 inp_normal;
 *
 * This still results in an interface match.
 *
 * @see glGetProgram
 *      glLinkProgram
 *      glGetActiveUniform
 *      glGetActiveAttrib
 *
 *      http://antongerdelan.net/opengl/shaders.html
 *      https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language
 *      https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes
 *      https://www.khronos.org/opengl/wiki/Shader_Compilation
 *      https://www.khronos.org/opengl/wiki/Layout_Qualifier_(GLSL)
 *      https://learnopengl.com/Getting-started/Shaders
 *
 *      https://stackoverflow.com/questions/440144
 *      https://gamedev.stackexchange.com/questions/22216
 *      https://gamedev.stackexchange.com/questions/47910
 *      https://gamedev.stackexchange.com/questions/68482
 *
 *      - Example shader classes
 *      https://codereview.stackexchange.com/questions/198349
 *      https://codereview.stackexchange.com/questions/97257
 *
 *      - Shader compilation and deletion
 *      https://stackoverflow.com/questions/39923583
 *      https://stackoverflow.com/questions/9113154
 *      https://gamedev.stackexchange.com/questions/47910
 *
 *      - Uniform and Attribute information
 *      https://stackoverflow.com/questions/5566233
 *      https://stackoverflow.com/questions/4635913
 */
GLuint create_program(const std::vector<GLuint> &shaders)
{
    /* Create a new program object. */
    GLuint program = glCreateProgram();
    core_assert(glIsProgram(program), "failed to create program object");

    /* Attach the shader objects to the program and link. */
    for (auto &it : shaders) {
        glAttachShader(program, it);
    }
    glLinkProgram(program);
    GLint status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        /* Get the number of characters in the program information log. */
        GLsizei infolen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infolen);
        std::vector<GLchar> infolog(infolen+1, '\0');

        /* Get the information log for the specified program object. */
        glGetProgramInfoLog(program, infolen, nullptr, infolog.data());
        core_throw(core::str_format(
            "failed to link program:\n\n%s\n", infolog.data()));
    }

    /*
     * Detach the shader from the program before marking it for deletion.
     * glDeleteShader flags the shader object for deletion, after which
     * point the object will be deleted when no longer attached to any
     * program object.
     */
    for (auto &it : shaders) {
        glDetachShader(program, it);
        glDeleteShader(it);
    }

    /* Bind the program before return. */
    glUseProgram(program);

    return program;
}

/**
 * destroy_program
 * @brief Destroy a shader program object. Free the memory and invalidate
 * the identifier associated with the program object.
 *
 * If the program object is in use in the current rendering context, it
 * will be flagged for deletion.
 * If the program object has shader objects attached to it, these will be
 * automatically detached but not deleted unless they have already been
 * marked for deletion.
 * A value of 0 for program will be silently ignored.
 */
void destroy_program(const GLuint &program)
{
    /* Ignore if program name is 0. */
    if (program == 0) {
        return;
    }

    /* Bind the program and get the number of attached shaders. */
    glUseProgram(program);

    GLint n_shaders;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &n_shaders);
    if (n_shaders > 0) {
        /* Get the handles of each attached shader object. */
        GLsizei max_count = static_cast<GLsizei>(n_shaders);
        std::vector<GLuint> shaders(max_count);
        GLsizei count = 0;
        glGetAttachedShaders(program, max_count, &count, shaders.data());

        /* Detach and delete each shader object. */
        for (GLsizei i = 0; i < count; ++i) {
            glDetachShader(program, shaders[i]);
            glDeleteShader(shaders[i]);
        }
    }

    /* Delete the program. */
    glDeleteProgram(program);
}

/**
 * get_program_info
 * @brief Return an string containing the shader program object info.
 *
 * 1. Link status.
 * 2. Number of attached shaders.
 * 3. Number of active uniform variables and the name, location,
 *    count and type of each active uniform stored in the map.
 * 4. Number of active attribute variables and the name, location,
 *    count and type of each active attribute stored in the map.
 */
std::string get_program_info(const GLuint &program)
{
    GLint param;
    std::ostringstream ss;

    /* Return an empty vector if program is invalid */
    if (!program) {
        core_debug("invalid shader program object");
        return std::string();
    }

    /*
     * Print shader program header information.
     */
    glGetProgramiv(program, GL_LINK_STATUS, &param);
    ss << core::str_format("GL_LINK_STATUS = %d\n", param);
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &param);
    ss << core::str_format("GL_ATTACHED_SHADERS = %d\n", param);

    /*
     * Print shader program active uniforms information map.
     */
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &param);
    ss << core::str_format("GL_ACTIVE_UNIFORMS = %d\n", param);
    for (auto &it : get_active_uniforms(program)) {
        ss << core::str_format(
            "%16s (loc=%d): count %d, type %16s (%5d), length %2u, size %u\n",
            it.name.c_str(),
            it.location,
            it.count,
            DataType::name(it.type).c_str(),
            it.type,
            DataType::length(it.type),
            DataType::size(it.type));
    }

    /*
     * Print shader program active attributes information map.
     */
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &param);
    ss << core::str_format("GL_ACTIVE_ATTRIBUTES = %d\n", param);
    for (auto &it : get_active_attributes(program)) {
        ss << core::str_format(
            "%16s (loc=%d): count %d, type %16s (%5d), length %2u, size %u\n",
            it.name.c_str(),
            it.location,
            it.count,
            DataType::name(it.type).c_str(),
            it.type,
            DataType::length(it.type),
            DataType::size(it.type));
    }

    return ss.str();
}

} /* gl */
} /* atto */
