/*
 * variable.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/glsl/variable.hpp"

namespace atto {
namespace gl {

/**
 * get_active_uniforms
 * @brief Retrieve all active uniforms in a shader program object.
 *
 * Call glGetProgramiv with parameter GL_ACTIVE_UNIFORMS to get the total
 * number of active uniform variables in the shader program object.
 * Call glGetProgramiv with parameter GL_ACTIVE_UNIFORM_MAX_LENGTH to get
 * the length of the longest active uniform name in the shader program.
 * Call glGetActiveUniform for each active uniform variable index to query
 * its name, type and length.
 *
 * The name argument is null terminated with length <= max_length.
 * The type argument is a symbolic constant defining the data type.
 * The count argument is the array size of the uniform variable in units
 * of the type returned in type.
 *
 * Call glGetUniformLocation to get the location of the uniform variable
 * name in the shader program object. This function returns -1 if name
 * does not correspond to an active uniform variable in program.
 *
 * Possible data types are (cf. glGetActiveUniform):
 *      GL_FLOAT, GL_FLOAT_VEC{2,3,4}
 *      GL_DOUBLE, GL_DOUBLE_VEC{2,3,4}
 *
 *      GL_INT, GL_INT_VEC{2,3,4}
 *      GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC{2,3,4}
 *
 *      GL_FLOAT_MAT{2,2x3,2x4}
 *      GL_FLOAT_MAT{3x2,3,3x4}
 *      GL_FLOAT_MAT{4x2,4x3,4}
 *
 *      GL_DOUBLE_MAT{2,2x3,2x4}
 *      GL_DOUBLE_MAT{3x2,3,3x4}
 *      GL_DOUBLE_MAT{4x2,4x3,4}
 *
 *      GL_SAMPLER_{1D,2D,3D}
 *      GL_INT_SAMPLER_{1D,2D,3D}
 *      GL_UNSIGNED_INT_SAMPLER_{1D,2D,3D}
 */
std::vector<Variable> get_active_uniforms(const GLuint &program)
{
    std::vector<Variable> uniforms;
    if (program == 0) {
        return uniforms;
    }

    GLint n_uniforms = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &n_uniforms);
    if (n_uniforms == 0) {
        return uniforms;
    }

    GLint max_length;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);

    GLint count;
    GLenum type;
    std::vector<GLchar> name(max_length);
    for (GLuint i = 0; i < static_cast<GLuint>(n_uniforms); ++i) {
        glGetActiveUniform(
            program,
            i,
            static_cast<GLsizei>(max_length),
            nullptr,  /* don't return num of chars written */
            &count,
            &type,
            name.data());

        GLint location = glGetUniformLocation(program, name.data());
        core_assert(location != -1, "uniform name is inactive or invalid");

        uniforms.emplace_back(
            Variable{std::string(name.data()), location, count, type});
    }

    return uniforms;
}

/**
 * get_active_attributes
 * @brief Retrieve all active attributes in a shader program object.
 *
 * Call glGetProgramiv with parameter GL_ACTIVE_ATTRIBUTES to get the total
 * number of active attribute variables in the shader program object.
 * Call glGetProgramiv with parameter GL_ACTIVE_ATTRIBUTE_MAX_LENGTH to get
 * the length of the longest active attribute name in the shader program.
 * Call glGetActiveAttrib for each active attribute variable index to query
 * its name, type and size.
 *
 * The name argument is null terminated with length <= max_length.
 * The type argument is a symbolic constant defining the data type.
 * The count argument is the array size of the attribute variable in units
 * of the type returned in type.
 *
 * Call glGetAttribLocation to get the location of the attribute variable
 * name in the shader program object. This function returns -1 if name does
 * not correspond to an active attribute variable in program.
 *
 * Possible data types are of the form (cf. glGetActiveAttrib):
 *      GL_FLOAT, GL_FLOAT_VEC{2,3,4}
 *      GL_DOUBLE, GL_DOUBLE_VEC{2,3,4}
 *
 *      GL_INT, GL_INT_VEC{2,3,4}
 *      GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC{2,3,4}
 *
 *      GL_FLOAT_MAT{2,2x3,2x4}
 *      GL_FLOAT_MAT{3x2,3,3x4}
 *      GL_FLOAT_MAT{4x2,4x3,4}
 *
 *      GL_DOUBLE_MAT{2,2x3,2x4}
 *      GL_DOUBLE_MAT{3x2,3,3x4}
 *      GL_DOUBLE_MAT{4x2,4x3,4}
 */
std::vector<Variable> get_active_attributes(const GLuint &program)
{
    std::vector<Variable> attributes;
    if (program == 0) {
        return attributes;
    }

    GLint n_attributes = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &n_attributes);
    if (n_attributes == 0) {
        return attributes;
    }

    GLint max_length;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);

    GLint count;
    GLenum type;
    std::vector<GLchar> name(max_length);
    for (GLuint i = 0; i < static_cast<GLuint>(n_attributes); ++i) {
        glGetActiveAttrib(
            program,
            i,
            static_cast<GLsizei>(max_length),
            nullptr,  /* don't return num of chars written */
            &count,
            &type,
            name.data());

        GLint location = glGetAttribLocation(program, name.data());
        core_assert(location != -1, "attribute name is inactive or invalid");

        attributes.emplace_back(
            Variable{std::string(name.data()), location, count, type});
    }

    return attributes;
}

} /* gl */
} /* atto */
