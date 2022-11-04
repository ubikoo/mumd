/*
 * shader.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/glsl/shader.hpp"

namespace atto {
namespace gl {

/**
 * create_shader
 * @brief Create a new shader object, set shader source and compile.
 * Check the compilation status by querying the shader infolog.
 *
 * In order to be linked in a program object, the newly created
 * shader must first be attached using the function glAttachShader.
 *
 * All operations that can be performed on a shader object are valid
 * whether or not the shader object is attached to a program object.
 *
 * It is permissible to attach a shader object to a program object
 * before source code has been loaded into the shader object or before
 * the shader object has been compiled.
 *
 * It is permissible to attach multiple shader objects of the same
 * type because each may contain a portion of the complete shader.
 *
 * It is also permissible to attach a shader object to more than one
 * program object. If a shader object is deleted while it is attached
 * to a program object, it will be flagged for deletion, and deletion
 * will not occur until glDetachShader is called to detach it from all
 * program objects to which it is attached.
 *
 * @see glShaderSource
 *      glCompileShader
 *      glAttachShader
 *      glDetachShader
 */
GLuint create_shader(const Shader &stage)
{
    /*
     * Create a new shader object. Ensure glCreateShader returns a
     * non-zero value by which the newly created empty shader can
     * be referenced.
     */
    GLuint shader = glCreateShader(stage.type());
    core_assert(glIsShader(shader), "failed to create shader object");

    /*
     * Set the shader source and compile.
     */
    core_assert(!stage.source().empty(), "invalid shader source");
    const GLchar *source = static_cast<const GLchar *>(stage.source().c_str());
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    /*
     * Query shader compilation status for errors. GL_COMPILE_STATUS returns
     * GL_TRUE if shader compilation was successful, and GL_FALSE otherwise.
     */
    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        /* Get the number of characters in the shader information log. */
        GLint infolen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infolen);
        std::vector<GLchar> infolog(infolen+1, '\0');

        /*
         * Get the information log for the specified program object.
         */
        glGetShaderInfoLog(shader, infolen, nullptr, infolog.data());
        core_throw(source + core::str_format(
            "\nfailed to compile shader:\n\n%s\n", infolog.data()));
    }

    return shader;
}

/**
 * create_shader
 * @brief Create a new shader stage of a given type from the specified file.
 */
GLuint create_shader(const GLenum type, const std::string &filename)
{
    try {
        std::vector<std::string> buffer;

        core::FileIn fp(filename);
        fp.readlines(buffer);
        core_assert(!fp.is_error(),
            core::str_format("failed to read %s", filename.c_str()));

        std::string source;
        for (auto &line : buffer) {
            source.append(line + "\n");
        }

        return create_shader(Shader(type, source));
    } catch (std::exception &e) {
        core_throw(e.what());
    }
}

} /* gl */
} /* atto */
