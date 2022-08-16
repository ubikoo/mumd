/*
 * datatype.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/glsl/datatype.hpp"

namespace atto {
namespace gl {
namespace DataType {

/**
 * DataType::map
 * @brief Collection of key-value pairs of OpenGL data types and corresponding
 * attributes.
 * DataTypeAttribute is a plain helper structure containing the attributes of
 * a OpenGL data type:
 *      name:       name of the OpenGL enumeration data type
 *      length:     length in units of the primitive data type
 *      size:       size of the primitive data type in bytes
 *      index:      type_index of the primitive data type
 *
 * OpenGL data types are used to represent the types of:
 *  - Active uniform variables and corresponding GLSL objects in the shader
 *  program object.
 *  - Active attribute variables and corresponding GLSL objects in the shader
 *  program object.
 *
 * Basic data types are fundamental types and non-basic types are aggregates
 * of these fundamental types. Only fundamental data types are considered here.
 * The total number of enumeration data types is only a subset of the total
 * set of GLSL data types:
 *      - Vector
 *      GL_DOUBLE, GL_DOUBLE_VEC[2,3,4]
 *      GL_FLOAT, GL_FLOAT_VEC[2,3,4]
 *      GL_INT, GL_INT_VEC[2,3,4]
 *      GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC[2,3,4]
 *
 *      - Matrix
 *      GL_DOUBLE, GL_DOUBLE_VEC[2,3,4]
 *      GL_FLOAT, GL_FLOAT_VEC[2,3,4]
 *
 *      - Sampler
 *      GL_SAMPLER_[1,2,3]D,
 *      GL_SAMPLER_BUFFER, GL_SAMPLER_2D_RECT
 *      GL_INT_SAMPLER_[1,2,3]D,
 *      GL_INT_SAMPLER_BUFFER, GL_INT_SAMPLER_2D_RECT
 *      GL_UNSIGNED_INT_SAMPLER_[1,2,3]D,
 *      GL_UNSIGNED_INT_SAMPLER_BUFFER, GL_UNSIGNED_INT_SAMPLER_2D_RECT
 *
 * @see https://www.khronos.org/opengl/wiki/Data_Type_(GLSL)
 *      https://www.khronos.org/opengl/wiki/Sampler_(GLSL)
 */
struct DataTypeAttribute {
    std::string name;       /* name of the OpenGL enumeration data type */
    GLuint length;          /* length in units of the primitive data type */
    GLuint size;            /* size of the primitive data type in bytes */
    std::type_index index;  /* type_index of the primitive data type */
};

static const std::map<GLenum, DataTypeAttribute> map = {
    /* ---- Vector double, dvec2, dvec3, dvec4 ----------------------------- */
    {GL_DOUBLE,                     {"GL_DOUBLE",
                                     1,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    {GL_DOUBLE_VEC2,                {"GL_DOUBLE_VEC2",
                                     2,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    {GL_DOUBLE_VEC3,                {"GL_DOUBLE_VEC3",
                                     3,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    {GL_DOUBLE_VEC4,                {"GL_DOUBLE_VEC4",
                                     4,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    /* ---- Vector float, vec2, vec3, vec4 --------------------------------- */
    {GL_FLOAT,                      {"GL_FLOAT",
                                     1,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    {GL_FLOAT_VEC2,                 {"GL_FLOAT_VEC2",
                                     2,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    {GL_FLOAT_VEC3,                 {"GL_FLOAT_VEC3",
                                     3,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    {GL_FLOAT_VEC4,                 {"GL_FLOAT_VEC4",
                                     4,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    /* ---- Vector int, ivec2, ivec3, ivec4 -------------------------------- */
    {GL_INT,                        {"GL_INT",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_INT_VEC2,                   {"GL_INT_VEC2",
                                     2,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_INT_VEC3,                   {"GL_INT_VEC3",
                                     3,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_INT_VEC4,                   {"GL_INT_VEC4",
                                     4,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    /* ---- Vector unsigned int, uvec2, uvec3, uvec4 ----------------------- */
    {GL_UNSIGNED_INT,               {"GL_UNSIGNED_INT",
                                     1,
                                     sizeof(GLuint),
                                     std::type_index(typeid(GLuint))}},
    {GL_UNSIGNED_INT_VEC2,          {"GL_UNSIGNED_INT_VEC2",
                                     2,
                                     sizeof(GLuint),
                                     std::type_index(typeid(GLuint))}},
    {GL_UNSIGNED_INT_VEC3,          {"GL_UNSIGNED_INT_VEC3",
                                     3,
                                     sizeof(GLuint),
                                     std::type_index(typeid(GLuint))}},
    {GL_UNSIGNED_INT_VEC4,          {"GL_UNSIGNED_INT_VEC4",
                                     4,
                                     sizeof(GLuint),
                                     std::type_index(typeid(GLuint))}},
    /* ---- Matrix dmat2, dmat2x3, dmat2x4 --------------------------------- */
    {GL_DOUBLE_MAT2,                {"GL_DOUBLE_MAT2",
                                     4,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    {GL_DOUBLE_MAT2x3,              {"GL_DOUBLE_MAT2x3",
                                     6,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    {GL_DOUBLE_MAT2x4,              {"GL_DOUBLE_MAT2x4",
                                     8,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    /* ---- Matrix dmat3x2, dmat3, dmat3x4 --------------------------------- */
    {GL_DOUBLE_MAT3x2,              {"GL_DOUBLE_MAT3x2",
                                     6,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    {GL_DOUBLE_MAT3,                {"GL_DOUBLE_MAT3",
                                     9,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    {GL_DOUBLE_MAT3x4,              {"GL_DOUBLE_MAT3x4",
                                     12,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    /* ---- Matrix dmat4x2, dmat4x3, dmat4 --------------------------------- */
    {GL_DOUBLE_MAT4x2,              {"GL_DOUBLE_MAT4x2",
                                     8,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    {GL_DOUBLE_MAT4x3,              {"GL_DOUBLE_MAT4x3",
                                     12,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    {GL_DOUBLE_MAT4,                {"GL_DOUBLE_MAT4",
                                     16,
                                     sizeof(GLdouble),
                                     std::type_index(typeid(GLdouble))}},
    /* ---- Matrix mat2, mat2x3, mat2x4 ------------------------------------ */
    {GL_FLOAT_MAT2,                 {"GL_FLOAT_MAT2",
                                     4,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    {GL_FLOAT_MAT2x3,               {"GL_FLOAT_MAT2x3",
                                     6,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    {GL_FLOAT_MAT2x4,               {"GL_FLOAT_MAT2x4",
                                     8,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    /* ---- ---- Matrix mat3x2, mat3, mat3x4 ------------------------------- */
    {GL_FLOAT_MAT3x2,               {"GL_FLOAT_MAT3x2",
                                     6,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    {GL_FLOAT_MAT3,                 {"GL_FLOAT_MAT3",
                                     9,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    {GL_FLOAT_MAT3x4,               {"GL_FLOAT_MAT3x4",
                                     12,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    /* ---- Matrix mat4x2, mat4x3, mat4 ------------------------------------ */
    {GL_FLOAT_MAT4x2,               {"GL_FLOAT_MAT4x2",
                                     8,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    {GL_FLOAT_MAT4x3,               {"GL_FLOAT_MAT4x3",
                                     12,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    {GL_FLOAT_MAT4,                 {"GL_FLOAT_MAT4",
                                     16,
                                     sizeof(GLfloat),
                                     std::type_index(typeid(GLfloat))}},
    /* ---- Sampler data types --------------------------------------------- */
    {GL_SAMPLER_1D,                 {"GL_SAMPLER_1D",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_SAMPLER_2D,                 {"GL_SAMPLER_2D",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_SAMPLER_3D,                 {"GL_SAMPLER_3D",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_SAMPLER_BUFFER,             {"GL_SAMPLER_BUFFER",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_SAMPLER_2D_RECT,            {"GL_SAMPLER_2D_RECT",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},

    {GL_INT_SAMPLER_1D,             {"GL_INT_SAMPLER_1D",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_INT_SAMPLER_2D,             {"GL_INT_SAMPLER_2D",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_INT_SAMPLER_3D,             {"GL_INT_SAMPLER_3D",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_INT_SAMPLER_BUFFER,         {"GL_INT_SAMPLER_BUFFER",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_INT_SAMPLER_2D_RECT,        {"GL_INT_SAMPLER_2D_RECT",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},

    {GL_UNSIGNED_INT_SAMPLER_1D,    {"GL_UNSIGNED_INT_SAMPLER_1D",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_UNSIGNED_INT_SAMPLER_2D,    {"GL_UNSIGNED_INT_SAMPLER_2D",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},
    {GL_UNSIGNED_INT_SAMPLER_3D,    {"GL_UNSIGNED_INT_SAMPLER_3D",
                                     1,
                                     sizeof(GLint),
                                     std::type_index(typeid(GLint))}},

    {GL_UNSIGNED_INT_SAMPLER_BUFFER,    {"GL_UNSIGNED_INT_SAMPLER_BUFFER",
                                         1,
                                         sizeof(GLint),
                                         std::type_index(typeid(GLint))}},
    {GL_UNSIGNED_INT_SAMPLER_2D_RECT,   {"GL_UNSIGNED_INT_SAMPLER_2D_RECT",
                                         1,
                                         sizeof(GLint),
                                         std::type_index(typeid(GLint))}},
};

/**
 * DataType::contains
 * @brief Does the map contain the key of the GL data type?
 */
bool contains(const GLenum data_type)
{
    return (map.find(data_type) != map.end());
}

/**
 * DataType::name
 * @brief Return the name of the GL data type.
 */
std::string name(const GLenum data_type)
{
    if (!contains(data_type)) {
        return std::string();
    }
    return (map.at(data_type)).name;
}

/**
 * DataType::length
 * @brief Return the length of the GL data type in units of the primitive
 * data type.
 */
GLuint length(const GLenum data_type)
{
    if (!contains(data_type)) {
        return 0;
    }
    return (map.at(data_type)).length;
}

/**
 * DataType::size
 * @brief Return the size of the underlying primitive data type in bytes.
 */
GLuint size(const GLenum data_type)
{
    if (!contains(data_type)) {
        return 0;
    }
    return (map.at(data_type)).size;
}

/**
 * DataType::is_double
 * @brief Is the underlying primitive data type GLdouble?
 */
bool is_double(const GLenum data_type)
{
    if (!contains(data_type)) {
        return false;
    }
    return (map.at(data_type).index == std::type_index(typeid(GLdouble)));
}

/**
 * DataType::is_float
 * @brief Is the underlying primitive data type GLfloat?
 */
bool is_float(const GLenum data_type)
{
    if (!contains(data_type)) {
        return false;
    }
    return (map.at(data_type).index == std::type_index(typeid(GLfloat)));
}

/**
 * DataType::is_int
 * @brief Is the underlying primitive data type GLint?
 */
bool is_int(const GLenum data_type)
{
    if (!contains(data_type)) {
        return false;
    }
    return (map.at(data_type).index == std::type_index(typeid(GLint)));
}

/**
 * DataType::is_uint
 * @brief Is the underlying primitive data type GLuint?
 */
bool is_uint(const GLenum data_type)
{
    if (!contains(data_type)) {
        return false;
    }
    return (map.at(data_type).index == std::type_index(typeid(GLuint)));
}

} /* DataType */
} /* gl */
} /* atto */
