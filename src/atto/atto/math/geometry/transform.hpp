/*
 * transform.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_TRANSFORM_H_
#define ATTO_MATH_GEOMETRY_TRANSFORM_H_

#include "atto/math/geometry/vec3.hpp"
#include "atto/math/geometry/mat4.hpp"

namespace atto {
namespace math {

/** ---------------------------------------------------------------------------
 * translate
 * @brief Translate this matrix by d.
 */
template<typename Type>
core_inline
mat4<Type> translate(const vec3<Type> d)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    mat4<Type> result(
        1.0, 0.0, 0.0, d(0),
        0.0, 1.0, 0.0, d(1),
        0.0, 0.0, 1.0, d(2),
        0.0, 0.0, 0.0, 1.0);
    return result;
}

template<typename Type>
core_inline
mat4<Type> translate(const mat4<Type> m, const vec3<Type> d)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return dot(translate(d), m);
}

template<typename Type>
core_inline
vec4<Type> translate(const vec4<Type> v, const vec3<Type> d)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return dot(translate(d), v);
}

template<typename Type>
core_inline
vec3<Type> translate(const vec3<Type> v, const vec3<Type> d)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return vec3<Type>(translate(vec4<Type>(v, (Type) 1), d));
}


/** ---------------------------------------------------------------------------
 * scale
 * @brief Scale this matrix by s.
 */
template<typename Type>
core_inline
mat4<Type> scale(const vec3<Type> s)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    mat4<Type> result(
        s(0), 0.0, 0.0, 0.0,
        0.0, s(1), 0.0, 0.0,
        0.0, 0.0, s(2), 0.0,
        0.0, 0.0,  0.0, 1.0);
    return result;
}

template<typename Type>
core_inline
mat4<Type> scale(const mat4<Type> m, const vec3<Type> s)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return dot(scale(s), m);
}

template<typename Type>
core_inline
vec4<Type> scale(const vec4<Type> v, const vec3<Type> s)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return dot(scale(s), v);
}

template<typename Type>
core_inline
vec3<Type> scale(const vec3<Type> v, const vec3<Type> s)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return vec3<Type>(scale(vec4<Type>(v, (Type) 1), s));
}


/** ---------------------------------------------------------------------------
 * rotate
 * @brief Return the matrix of rotation around an axis defined by n using
 * Rodrigues formula.
 */
template<typename Type>
core_inline
mat4<Type> rotate(vec3<Type> n, const Type theta)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type zero = (Type) 0;
    static constexpr Type one  = (Type) 1;

    /* Compute identity matrix */
    mat4<Type> I = mat4<Type>::eye;

    /* Compute cross product matrix */
    n = normalize(n);
    mat4<Type> R(
        zero, -n.z,  n.y, zero,
         n.z, zero, -n.x, zero,
        -n.y,  n.x, zero, zero,
        zero, zero, zero, zero);

    /* Compute rotation matrix */
    mat4<Type> result = mat4<Type>::eye;
    result += R * std::sin(theta);
    result += dot(R, R) * (one - std::cos(theta));
    return result;
}

template<typename Type>
core_inline
mat4<Type> rotate(const mat4<Type> m, vec3<Type> n, const Type theta)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return dot(rotate(n, theta), m);
}

template<typename Type>
core_inline
vec4<Type> rotate(const vec4<Type> v, vec3<Type> n, const Type theta)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return dot(rotate(n, theta), v);
}

template<typename Type>
core_inline
vec3<Type> rotate(const vec3<Type> v, vec3<Type> n, const Type theta)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return vec3<Type>(rotate(vec4<Type>(v, (Type) 1), n, theta));
}


/** ---------------------------------------------------------------------------
 * align
 * @brief Return the matrix that maps direction a to direction b.
 */
template<typename Type>
core_inline
mat4<Type> align(vec3<Type> a, vec3<Type> b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type one = (Type) 1;

    /* Compute the cross product */
    a = normalize(a);
    b = normalize(b);
    Type cos_theta = dot(a,b);
    vec3<Type> n = cross(a, b);

    /* Return identity if the direction vectors are already aligned. */
    if (isequal(cos_theta, -one)) {
        return mat4<Type>::eye * (-one);
    }

    if (isequal(cos_theta, one)) {
        return mat4<Type>::eye;
    }

    /* Return the rotation matrix using Rodrigues formula. */
    return rotate(n, std::acos(cos_theta));
}

template<typename Type>
core_inline
mat4<Type> align(const mat4<Type> m, vec3<Type> a, vec3<Type> b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return dot(align(a, b), m);
}

template<typename Type>
core_inline
vec4<Type> align(const vec4<Type> v, vec3<Type> a, vec3<Type> b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return dot(align(a, b), v);
}

template<typename Type>
core_inline
vec3<Type> align(const vec3<Type> v, vec3<Type> a, vec3<Type> b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return vec3<Type>(align(vec4<Type>(v, (Type) 1), a, b));
}


/** ---------------------------------------------------------------------------
 * lookat
 * @brief Compute a lookat local frame of reference given the eye position,
 * the ctr reference viewpoint and the updward directon vector up.
 *
 * The convention used here is one of a right-handed local basis set
 * where the z-axis is represented by the forward unit f-vector, with
 * origin at the eye and direction towards the ctr view position.
 * The x-axis is represented by the sideway unit s-vector and the y-axis
 * is represented by the upward unit u-vector in the local reference frame.
 */
template<typename Type>
core_inline
mat4<Type> lookat(
    const vec3<Type> &eye,
    const vec3<Type> &ctr,
    const vec3<Type> &up)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    /*
     * Compute the lookat local basis set s-u-f.
     */
    vec3<Type> f = normalize(ctr - eye);    /* f = ctr - eye */
    vec3<Type> s = normalize(cross(f, up)); /* s = f x up */
    vec3<Type> u = normalize(cross(s, f));  /* u = s x f */

    /*
     * Compute the lookat projection matrix and* corresponding
     * eye translation position.
     */
    mat4<Type> M(
         s(0),  s(1),  s(2), -eye(0),
         u(0),  u(1),  u(2), -eye(1),
        -f(0), -f(1), -f(2), -eye(2),
         0.0,   0.0,   0.0,    1.0);
    return M;
}

template<typename Type>
core_inline
mat4<Type> lookat(
    const mat4<Type> m,
    const vec3<Type> &eye,
    const vec3<Type> &ctr,
    const vec3<Type> &up)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return dot(lookat(eye, ctr, up), m);
}


/** ---------------------------------------------------------------------------
 * perspective
 * @brief Compute the perspective projection matrix.
 */
template<typename Type>
core_inline
mat4<Type> perspective(
    const Type fovy,
    const Type aspect,
    const Type znear,
    const Type zfar)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    static constexpr Type zero = (Type) 0;
    static constexpr Type one  = (Type) 1;
    static constexpr Type two  = (Type) 2;

    const Type tan_half_fovy = std::tan(fovy / two);

    mat4<Type> result{};
    result(0,0) =  one / (tan_half_fovy * aspect);
    result(0,1) =  zero;
    result(0,2) =  zero;
    result(0,3) =  zero;

    result(1,0) =  zero;
    result(1,1) =  one / tan_half_fovy;
    result(1,2) =  zero;
    result(1,3) =  zero;

    result(2,0) =  zero;
    result(2,1) =  zero;
    result(2,2) = -(zfar + znear) / (zfar - znear);
    result(2,3) = -(two * zfar * znear) / (zfar - znear);

    result(3,0) =  zero;
    result(3,1) =  zero;
    result(3,2) = -one;
    result(3,3) =  zero;
    return result;
}

template<typename Type>
core_inline
mat4<Type> perspective(
    const mat4<Type> m,
    const Type fovy,
    const Type aspect,
    const Type znear,
    const Type zfar)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return dot(perspective(fovy, aspect, znear, zfar), m);
}


/** ---------------------------------------------------------------------------
 * ortho
 * @brief Compute the orthographic projection matrix.
 */
template<typename Type>
core_inline
mat4<Type> ortho(
    const Type left,
    const Type right,
    const Type bottom,
    const Type top,
    const Type znear,
    const Type zfar)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat4<Type> result{};
    result(0,0) =  2.0 / (right - left);
    result(0,1) =  0.0;
    result(0,2) =  0.0;
    result(0,3) =  -(right + left) / (right - left);

    result(1,0) =  0.0;
    result(1,1) =  2.0 / (top - bottom);
    result(1,2) =  0.0;
    result(1,3) =  -(top + bottom) / (top - bottom);

    result(2,0) =  0.0;
    result(2,1) =  0.0;
    result(2,2) = -2.0 / (zfar - znear);
    result(2,3) = -(zfar + znear) / (zfar - znear);

    result(3,0) =  0.0;
    result(3,1) =  0.0;
    result(3,2) =  0.0;
    result(3,3) =  1.0;
    return result;
}

template<typename Type>
core_inline
mat4<Type> ortho(
    const mat4<Type> m,
    const Type left,
    const Type right,
    const Type bottom,
    const Type top,
    const Type znear,
    const Type zfar)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return dot(ortho(left, right, bottom, top, znear, zfar), m);
}

} /* math */
} /* atto */


/** ---------------------------------------------------------------------------
 * @brief Enable simd vectorized instructions.
 */
#ifdef ATTO_MATH_SIMD
#include "atto/math/geometry/transform.hpp"
#endif

#endif /* ATTO_MATH_GEOMETRY_TRANSFORM_H_ */
