/*
 * arithmetic.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_ARITHMETIC_H_
#define ATTO_MATH_GEOMETRY_ARITHMETIC_H_

#include "atto/math/geometry/vec2.hpp"
#include "atto/math/geometry/vec3.hpp"
#include "atto/math/geometry/vec4.hpp"

#include "atto/math/geometry/mat2.hpp"
#include "atto/math/geometry/mat3.hpp"
#include "atto/math/geometry/mat4.hpp"

namespace atto {
namespace math {

/** ---------------------------------------------------------------------------
 * abs
 * @brief Return the absolute value of the vector/matrix elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/abs.xhtml
 */
template<typename Type>
core_inline
vec2<Type> abs(const vec2<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec2<Type> result{};
    result(0) = std::abs(v(0));
    result(1) = std::abs(v(1));
    return result;
}

template<typename Type>
core_inline
vec3<Type> abs(const vec3<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec3<Type> result{};
    result(0) = std::abs(v(0));
    result(1) = std::abs(v(1));
    result(2) = std::abs(v(2));
    return result;
}

template<typename Type>
core_inline
vec4<Type> abs(const vec4<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec4<Type> result{};
    result(0) = std::abs(v(0));
    result(1) = std::abs(v(1));
    result(2) = std::abs(v(2));
    result(3) = std::abs(v(3));
    return result;
}

template<typename Type>
core_inline
mat2<Type> abs(const mat2<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat2<Type> result{};
    result(0,0) = std::abs(a(0,0));
    result(0,1) = std::abs(a(0,1));

    result(1,0) = std::abs(a(1,0));
    result(1,1) = std::abs(a(1,1));
    return result;
}

template<typename Type>
core_inline
mat3<Type> abs(const mat3<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat3<Type> result{};
    result(0,0) = std::abs(a(0,0));
    result(0,1) = std::abs(a(0,1));
    result(0,2) = std::abs(a(0,2));

    result(1,0) = std::abs(a(1,0));
    result(1,1) = std::abs(a(1,1));
    result(1,2) = std::abs(a(1,2));

    result(2,0) = std::abs(a(2,0));
    result(2,1) = std::abs(a(2,1));
    result(2,2) = std::abs(a(2,2));
    return result;
}

template<typename Type>
core_inline
mat4<Type> abs(const mat4<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat4<Type> result{};
    result(0,0) = std::abs(a(0,0));
    result(0,1) = std::abs(a(0,1));
    result(0,2) = std::abs(a(0,2));
    result(0,3) = std::abs(a(0,3));

    result(1,0) = std::abs(a(1,0));
    result(1,1) = std::abs(a(1,1));
    result(1,2) = std::abs(a(1,2));
    result(1,3) = std::abs(a(1,3));

    result(2,0) = std::abs(a(2,0));
    result(2,1) = std::abs(a(2,1));
    result(2,2) = std::abs(a(2,2));
    result(2,3) = std::abs(a(2,3));

    result(3,0) = std::abs(a(3,0));
    result(3,1) = std::abs(a(3,1));
    result(3,2) = std::abs(a(3,2));
    result(3,3) = std::abs(a(3,3));
    return result;
}


/** ---------------------------------------------------------------------------
 * sign
 * @brief Extract the sign of the vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/sign.xhtml
 */
template<typename Type>
core_inline
vec2<Type> sign(const vec2<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type zero = (Type) 0;
    static constexpr Type one = (Type) 1;

    vec2<Type> result{};
    result(0) = (v(0) < zero) ? -one : (v(0) > zero) ? one : zero;
    result(1) = (v(1) < zero) ? -one : (v(1) > zero) ? one : zero;
    return result;
}

template<typename Type>
core_inline
vec3<Type> sign(const vec3<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type zero = (Type) 0;
    static constexpr Type one = (Type) 1;

    vec3<Type> result{};
    result(0) = (v(0) < zero) ? -one : (v(0) > zero) ? one : zero;
    result(1) = (v(1) < zero) ? -one : (v(1) > zero) ? one : zero;
    result(2) = (v(2) < zero) ? -one : (v(2) > zero) ? one : zero;
    return result;
}

template<typename Type>
core_inline
vec4<Type> sign(const vec4<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type zero = (Type) 0;
    static constexpr Type one = (Type) 1;

    vec4<Type> result{};
    result(0) = (v(0) < zero) ? -one : (v(0) > zero) ? one : zero;
    result(1) = (v(1) < zero) ? -one : (v(1) > zero) ? one : zero;
    result(2) = (v(2) < zero) ? -one : (v(2) > zero) ? one : zero;
    result(3) = (v(3) < zero) ? -one : (v(3) > zero) ? one : zero;
    return result;
}

template<typename Type>
core_inline
mat2<Type> sign(const mat2<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type zero = (Type) 0;
    static constexpr Type one = (Type) 1;

    mat2<Type> result{};
    result(0,0) = (a(0,0) < zero) ? -one : (a(0,0) > zero) ? one : zero;
    result(0,1) = (a(0,1) < zero) ? -one : (a(0,1) > zero) ? one : zero;

    result(1,0) = (a(1,0) < zero) ? -one : (a(1,0) > zero) ? one : zero;
    result(1,1) = (a(1,1) < zero) ? -one : (a(1,1) > zero) ? one : zero;
    return result;
}

template<typename Type>
core_inline
mat3<Type> sign(const mat3<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type zero = (Type) 0;
    static constexpr Type one = (Type) 1;

    mat3<Type> result{};
    result(0,0) = (a(0,0) < zero) ? -one : (a(0,0) > zero) ? one : zero;
    result(0,1) = (a(0,1) < zero) ? -one : (a(0,1) > zero) ? one : zero;
    result(0,2) = (a(0,2) < zero) ? -one : (a(0,2) > zero) ? one : zero;

    result(1,0) = (a(1,0) < zero) ? -one : (a(1,0) > zero) ? one : zero;
    result(1,1) = (a(1,1) < zero) ? -one : (a(1,1) > zero) ? one : zero;
    result(1,2) = (a(1,2) < zero) ? -one : (a(1,2) > zero) ? one : zero;

    result(2,0) = (a(2,0) < zero) ? -one : (a(2,0) > zero) ? one : zero;
    result(2,1) = (a(2,1) < zero) ? -one : (a(2,1) > zero) ? one : zero;
    result(2,2) = (a(2,2) < zero) ? -one : (a(2,2) > zero) ? one : zero;
    return result;
}

template<typename Type>
core_inline
mat4<Type> sign(const mat4<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type zero = (Type) 0;
    static constexpr Type one = (Type) 1;

    mat4<Type> result{};
    result(0,0) = (a(0,0) < zero) ? -one : (a(0,0) > zero) ? one : zero;
    result(0,1) = (a(0,1) < zero) ? -one : (a(0,1) > zero) ? one : zero;
    result(0,2) = (a(0,2) < zero) ? -one : (a(0,2) > zero) ? one : zero;
    result(0,3) = (a(0,3) < zero) ? -one : (a(0,3) > zero) ? one : zero;

    result(1,0) = (a(1,0) < zero) ? -one : (a(1,0) > zero) ? one : zero;
    result(1,1) = (a(1,1) < zero) ? -one : (a(1,1) > zero) ? one : zero;
    result(1,2) = (a(1,2) < zero) ? -one : (a(1,2) > zero) ? one : zero;
    result(1,3) = (a(1,3) < zero) ? -one : (a(1,3) > zero) ? one : zero;

    result(2,0) = (a(2,0) < zero) ? -one : (a(2,0) > zero) ? one : zero;
    result(2,1) = (a(2,1) < zero) ? -one : (a(2,1) > zero) ? one : zero;
    result(2,2) = (a(2,2) < zero) ? -one : (a(2,2) > zero) ? one : zero;
    result(2,3) = (a(2,3) < zero) ? -one : (a(2,3) > zero) ? one : zero;

    result(3,0) = (a(3,0) < zero) ? -one : (a(3,0) > zero) ? one : zero;
    result(3,1) = (a(3,1) < zero) ? -one : (a(3,1) > zero) ? one : zero;
    result(3,2) = (a(3,2) < zero) ? -one : (a(3,2) > zero) ? one : zero;
    result(3,3) = (a(3,3) < zero) ? -one : (a(3,3) > zero) ? one : zero;
    return result;
}


/** ---------------------------------------------------------------------------
 * floor
 * @brief Find the nearest integer less than or equal to the vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/floor.xhtml
 */
template<typename Type>
core_inline
vec2<Type> floor(const vec2<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec2<Type> result{};
    result(0) = std::floor(v(0));
    result(1) = std::floor(v(1));
    return result;
}

template<typename Type>
core_inline
vec3<Type> floor(const vec3<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec3<Type> result{};
    result(0) = std::floor(v(0));
    result(1) = std::floor(v(1));
    result(2) = std::floor(v(2));
    return result;
}

template<typename Type>
core_inline
vec4<Type> floor(const vec4<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec4<Type> result{};
    result(0) = std::floor(v(0));
    result(1) = std::floor(v(1));
    result(2) = std::floor(v(2));
    result(3) = std::floor(v(3));
    return result;
}

template<typename Type>
core_inline
mat2<Type> floor(const mat2<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat2<Type> result{};
    result(0,0) = std::floor(a(0,0));
    result(0,1) = std::floor(a(0,1));

    result(1,0) = std::floor(a(1,0));
    result(1,1) = std::floor(a(1,1));
    return result;
}

template<typename Type>
core_inline
mat3<Type> floor(const mat3<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat3<Type> result{};
    result(0,0) = std::floor(a(0,0));
    result(0,1) = std::floor(a(0,1));
    result(0,2) = std::floor(a(0,2));

    result(1,0) = std::floor(a(1,0));
    result(1,1) = std::floor(a(1,1));
    result(1,2) = std::floor(a(1,2));

    result(2,0) = std::floor(a(2,0));
    result(2,1) = std::floor(a(2,1));
    result(2,2) = std::floor(a(2,2));
    return result;
}

template<typename Type>
core_inline
mat4<Type> floor(const mat4<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat4<Type> result{};
    result(0,0) = std::floor(a(0,0));
    result(0,1) = std::floor(a(0,1));
    result(0,2) = std::floor(a(0,2));
    result(0,3) = std::floor(a(0,3));

    result(1,0) = std::floor(a(1,0));
    result(1,1) = std::floor(a(1,1));
    result(1,2) = std::floor(a(1,2));
    result(1,3) = std::floor(a(1,3));

    result(2,0) = std::floor(a(2,0));
    result(2,1) = std::floor(a(2,1));
    result(2,2) = std::floor(a(2,2));
    result(2,3) = std::floor(a(2,3));

    result(3,0) = std::floor(a(3,0));
    result(3,1) = std::floor(a(3,1));
    result(3,2) = std::floor(a(3,2));
    result(3,3) = std::floor(a(3,3));
    return result;
}


/** ---------------------------------------------------------------------------
 * round
 * @brief Find the nearest integer to the vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/round.xhtml
 */
template<typename Type>
core_inline
vec2<Type> round(const vec2<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec2<Type> result{};
    result(0) = std::round(v(0));
    result(1) = std::round(v(1));
    return result;
}

template<typename Type>
core_inline
vec3<Type> round(const vec3<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec3<Type> result{};
    result(0) = std::round(v(0));
    result(1) = std::round(v(1));
    result(2) = std::round(v(2));
    return result;
}

template<typename Type>
core_inline
vec4<Type> round(const vec4<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec4<Type> result{};
    result(0) = std::round(v(0));
    result(1) = std::round(v(1));
    result(2) = std::round(v(2));
    result(3) = std::round(v(3));
    return result;
}

template<typename Type>
core_inline
mat2<Type> round(const mat2<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat2<Type> result{};
    result(0,0) = std::round(a(0,0));
    result(0,1) = std::round(a(0,1));

    result(1,0) = std::round(a(1,0));
    result(1,1) = std::round(a(1,1));
    return result;
}

template<typename Type>
core_inline
mat3<Type> round(const mat3<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat3<Type> result{};
    result(0,0) = std::round(a(0,0));
    result(0,1) = std::round(a(0,1));
    result(0,2) = std::round(a(0,2));

    result(1,0) = std::round(a(1,0));
    result(1,1) = std::round(a(1,1));
    result(1,2) = std::round(a(1,2));

    result(2,0) = std::round(a(2,0));
    result(2,1) = std::round(a(2,1));
    result(2,2) = std::round(a(2,2));
    return result;
}

template<typename Type>
core_inline
mat4<Type> round(const mat4<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat4<Type> result{};
    result(0,0) = std::round(a(0,0));
    result(0,1) = std::round(a(0,1));
    result(0,2) = std::round(a(0,2));
    result(0,3) = std::round(a(0,3));

    result(1,0) = std::round(a(1,0));
    result(1,1) = std::round(a(1,1));
    result(1,2) = std::round(a(1,2));
    result(1,3) = std::round(a(1,3));

    result(2,0) = std::round(a(2,0));
    result(2,1) = std::round(a(2,1));
    result(2,2) = std::round(a(2,2));
    result(2,3) = std::round(a(2,3));

    result(3,0) = std::round(a(3,0));
    result(3,1) = std::round(a(3,1));
    result(3,2) = std::round(a(3,2));
    result(3,3) = std::round(a(3,3));
    return result;
}


/** ---------------------------------------------------------------------------
 * ceil
 * @brief Find the nearest integer that is greater than or equal to the
 * vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/ceil.xhtml
 */
template<typename Type>
core_inline
vec2<Type> ceil(const vec2<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec2<Type> result{};
    result(0) = std::ceil(v(0));
    result(1) = std::ceil(v(1));
    return result;
}

template<typename Type>
core_inline
vec3<Type> ceil(const vec3<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec3<Type> result{};
    result(0) = std::ceil(v(0));
    result(1) = std::ceil(v(1));
    result(2) = std::ceil(v(2));
    return result;
}

template<typename Type>
core_inline
vec4<Type> ceil(const vec4<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec4<Type> result{};
    result(0) = std::ceil(v(0));
    result(1) = std::ceil(v(1));
    result(2) = std::ceil(v(2));
    result(3) = std::ceil(v(3));
    return result;
}

template<typename Type>
core_inline
mat2<Type> ceil(const mat2<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat2<Type> result{};
    result(0,0) = std::ceil(a(0,0));
    result(0,1) = std::ceil(a(0,1));

    result(1,0) = std::ceil(a(1,0));
    result(1,1) = std::ceil(a(1,1));
    return result;
}

template<typename Type>
core_inline
mat3<Type> ceil(const mat3<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat3<Type> result{};
    result(0,0) = std::ceil(a(0,0));
    result(0,1) = std::ceil(a(0,1));
    result(0,2) = std::ceil(a(0,2));

    result(1,0) = std::ceil(a(1,0));
    result(1,1) = std::ceil(a(1,1));
    result(1,2) = std::ceil(a(1,2));

    result(2,0) = std::ceil(a(2,0));
    result(2,1) = std::ceil(a(2,1));
    result(2,2) = std::ceil(a(2,2));
    return result;
}

template<typename Type>
core_inline
mat4<Type> ceil(const mat4<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat4<Type> result{};
    result(0,0) = std::ceil(a(0,0));
    result(0,1) = std::ceil(a(0,1));
    result(0,2) = std::ceil(a(0,2));
    result(0,3) = std::ceil(a(0,3));

    result(1,0) = std::ceil(a(1,0));
    result(1,1) = std::ceil(a(1,1));
    result(1,2) = std::ceil(a(1,2));
    result(1,3) = std::ceil(a(1,3));

    result(2,0) = std::ceil(a(2,0));
    result(2,1) = std::ceil(a(2,1));
    result(2,2) = std::ceil(a(2,2));
    result(2,3) = std::ceil(a(2,3));

    result(3,0) = std::ceil(a(3,0));
    result(3,1) = std::ceil(a(3,1));
    result(3,2) = std::ceil(a(3,2));
    result(3,3) = std::ceil(a(3,3));
    return result;
}


/** ---------------------------------------------------------------------------
 * clamp
 * @brief Constrain the vector elements to lie inside the range
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/clamp.xhtml
 */
template<typename Type>
core_inline
vec2<Type> clamp(const vec2<Type> &v, const Type lo, const Type hi)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec2<Type> result{};
    result(0) = std::min(std::max(v(0), lo), hi);
    result(1) = std::min(std::max(v(1), lo), hi);
    return result;
}

template<typename Type>
core_inline
vec3<Type> clamp(const vec3<Type> &v, const Type lo, const Type hi)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec3<Type> result{};
    result(0) = std::min(std::max(v(0), lo), hi);
    result(1) = std::min(std::max(v(1), lo), hi);
    result(2) = std::min(std::max(v(2), lo), hi);
    return result;
}

template<typename Type>
core_inline
vec4<Type> clamp(const vec4<Type> &v, const Type lo, const Type hi)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec4<Type> result{};
    result(0) = std::min(std::max(v(0), lo), hi);
    result(1) = std::min(std::max(v(1), lo), hi);
    result(2) = std::min(std::max(v(2), lo), hi);
    result(3) = std::min(std::max(v(3), lo), hi);
    return result;
}

template<typename Type>
core_inline
mat2<Type> clamp(const mat2<Type> &a, const Type lo, const Type hi)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat2<Type> result{};
    result(0,0) = std::min(std::max((a(0,0), lo), hi));
    result(0,1) = std::min(std::max((a(0,1), lo), hi));

    result(1,0) = std::min(std::max((a(1,0), lo), hi));
    result(1,1) = std::min(std::max((a(1,1), lo), hi));
    return result;
}

template<typename Type>
core_inline
mat3<Type> clamp(const mat3<Type> &a, const Type lo, const Type hi)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat3<Type> result{};
    result(0,0) = std::min(std::max((a(0,0), lo), hi));
    result(0,1) = std::min(std::max((a(0,1), lo), hi));
    result(0,2) = std::min(std::max((a(0,2), lo), hi));

    result(1,0) = std::min(std::max((a(1,0), lo), hi));
    result(1,1) = std::min(std::max((a(1,1), lo), hi));
    result(1,2) = std::min(std::max((a(1,2), lo), hi));

    result(2,0) = std::min(std::max((a(2,0), lo), hi));
    result(2,1) = std::min(std::max((a(2,1), lo), hi));
    result(2,2) = std::min(std::max((a(2,2), lo), hi));
    return result;
}

template<typename Type>
core_inline
mat4<Type> clamp(const mat4<Type> &a, const Type lo, const Type hi)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat4<Type> result{};
    result(0,0) = std::min(std::max((a(0,0), lo), hi));
    result(0,1) = std::min(std::max((a(0,1), lo), hi));
    result(0,2) = std::min(std::max((a(0,2), lo), hi));
    result(0,3) = std::min(std::max((a(0,3), lo), hi));

    result(1,0) = std::min(std::max((a(1,0), lo), hi));
    result(1,1) = std::min(std::max((a(1,1), lo), hi));
    result(1,2) = std::min(std::max((a(1,2), lo), hi));
    result(1,3) = std::min(std::max((a(1,3), lo), hi));

    result(2,0) = std::min(std::max((a(2,0), lo), hi));
    result(2,1) = std::min(std::max((a(2,1), lo), hi));
    result(2,2) = std::min(std::max((a(2,2), lo), hi));
    result(2,3) = std::min(std::max((a(2,3), lo), hi));

    result(3,0) = std::min(std::max((a(3,0), lo), hi));
    result(3,1) = std::min(std::max((a(3,1), lo), hi));
    result(3,2) = std::min(std::max((a(3,2), lo), hi));
    result(3,3) = std::min(std::max((a(3,3), lo), hi));
    return result;
}


/** ---------------------------------------------------------------------------
 * lerp
 * @brief Linearly interpolate between two vectors.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/mix.xhtml
 */
template<typename Type>
core_inline
vec2<Type> lerp(const vec2<Type> lo, const vec2<Type> hi, Type alpha)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type one = 1.0;
    vec2<Type> result{};
    result(0) = (one - alpha) * lo(0) + alpha * hi(0);
    result(1) = (one - alpha) * lo(1) + alpha * hi(1);
    return result;
}

template<typename Type>
core_inline
vec3<Type> lerp(const vec3<Type> lo, const vec3<Type> hi, Type alpha)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type one = 1.0;
    vec3<Type> result{};
    result(0) = (one - alpha) * lo(0) + alpha * hi(0);
    result(1) = (one - alpha) * lo(1) + alpha * hi(1);
    result(2) = (one - alpha) * lo(2) + alpha * hi(2);
    return result;
}

template<typename Type>
core_inline
vec4<Type> lerp(const vec4<Type> lo, const vec4<Type> hi, Type alpha)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type one = 1.0;
    vec4<Type> result{};
    result(0) = (one - alpha) * lo(0) + alpha * hi(0);
    result(1) = (one - alpha) * lo(1) + alpha * hi(1);
    result(2) = (one - alpha) * lo(2) + alpha * hi(2);
    result(3) = (one - alpha) * lo(3) + alpha * hi(3);
    return result;
}

template<typename Type>
core_inline
mat2<Type> lerp(const mat2<Type> lo, const mat2<Type> hi, Type alpha)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type one = 1.0;
    mat2<Type> result{};
    result(0,0) = (one - alpha) * lo(0,0) + alpha * hi(0,0);
    result(0,1) = (one - alpha) * lo(0,1) + alpha * hi(0,1);

    result(1,0) = (one - alpha) * lo(1,0) + alpha * hi(1,0);
    result(1,1) = (one - alpha) * lo(1,1) + alpha * hi(1,1);
    return result;
}

template<typename Type>
core_inline
mat3<Type> lerp(const mat3<Type> lo, const mat3<Type> hi, Type alpha)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type one = 1.0;
    mat3<Type> result{};
    result(0,0) = (one - alpha) * lo(0,0) + alpha * hi(0,0);
    result(0,1) = (one - alpha) * lo(0,1) + alpha * hi(0,1);
    result(0,2) = (one - alpha) * lo(0,2) + alpha * hi(0,2);

    result(1,0) = (one - alpha) * lo(1,0) + alpha * hi(1,0);
    result(1,1) = (one - alpha) * lo(1,1) + alpha * hi(1,1);
    result(1,2) = (one - alpha) * lo(1,2) + alpha * hi(1,2);

    result(2,0) = (one - alpha) * lo(2,0) + alpha * hi(2,0);
    result(2,1) = (one - alpha) * lo(2,1) + alpha * hi(2,1);
    result(2,2) = (one - alpha) * lo(2,2) + alpha * hi(2,2);
    return result;
}

template<typename Type>
core_inline
mat4<Type> lerp(const mat4<Type> lo, const mat4<Type> hi, Type alpha)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type one = 1.0;
    mat4<Type> result{};
    result(0,0) = (one - alpha) * lo(0,0) + alpha * hi(0,0);
    result(0,1) = (one - alpha) * lo(0,1) + alpha * hi(0,1);
    result(0,2) = (one - alpha) * lo(0,2) + alpha * hi(0,2);
    result(0,3) = (one - alpha) * lo(0,3) + alpha * hi(0,3);

    result(1,0) = (one - alpha) * lo(1,0) + alpha * hi(1,0);
    result(1,1) = (one - alpha) * lo(1,1) + alpha * hi(1,1);
    result(1,2) = (one - alpha) * lo(1,2) + alpha * hi(1,2);
    result(1,3) = (one - alpha) * lo(1,3) + alpha * hi(1,3);

    result(2,0) = (one - alpha) * lo(2,0) + alpha * hi(2,0);
    result(2,1) = (one - alpha) * lo(2,1) + alpha * hi(2,1);
    result(2,2) = (one - alpha) * lo(2,2) + alpha * hi(2,2);
    result(2,3) = (one - alpha) * lo(2,3) + alpha * hi(2,3);

    result(3,0) = (one - alpha) * lo(3,0) + alpha * hi(3,0);
    result(3,1) = (one - alpha) * lo(3,1) + alpha * hi(3,1);
    result(3,2) = (one - alpha) * lo(3,2) + alpha * hi(3,2);
    result(3,3) = (one - alpha) * lo(3,3) + alpha * hi(3,3);
    return result;
}

} /* math */
} /* atto */


/** ---------------------------------------------------------------------------
 * @brief Enable simd vectorized instructions.
 */
#ifdef ATTO_MATH_SIMD
#include "atto/math/geometry/arithmetic-simd.hpp"
#endif

#endif /* ATTO_MATH_GEOMETRY_ARITHMETIC_H_ */
