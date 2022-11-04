/*
 * algebra-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_ALGEBRA_SIMD_H_
#define ATTO_MATH_GEOMETRY_ALGEBRA_SIMD_H_

#include "atto/math/geometry/simd.hpp"

namespace atto {
namespace math {

/** ---------------------------------------------------------------------------
 * dot
 * @brief Return the 2-dimensional dot product.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/dot.xhtml
 */
template<>
core_inline
double dot(const vec2<double> &v, const vec2<double> &w)
{
    const __m128d a = simd_load(v);
    const __m128d b = simd_load(w);
    return _mm_cvtsd_f64(simd128_dot_(a, b));
}

template<>
core_inline
vec2<double> dot(const mat2<double> &a, const vec2<double> &v)
{
    /*
     * a0 = {a0, a1}
     * a1 = {a2, a3}
     * b  = {b0, b1}
     */
    __m128d a0 = simd_load(a, 0);
    __m128d a1 = simd_load(a, 1);
    __m128d b  = simd_load(v);
    /*
     * c0 = {a0*b0 + a1*b1, a0*b0 + a1*b1}
     * c1 = {a2*b0 + a3*b1, a2*b0 + a3*b1}
     *
     * _mm_unpackhi_pd(__m128d a, __m128d b)
     * dst[63:0]   := a[127:64]
     * dst[127:64] := b[127:64]
     */
    __m128d c0 = simd128_dot_(a0, b);
    __m128d c1 = simd128_dot_(a1, b);
    __m128d mul = _mm_unpackhi_pd(c0, c1);

    vec2<double> result{};
    simd_store(result, mul);
    return result;
}

template<>
core_inline
mat2<double> dot(const mat2<double> &a, const mat2<double> &b)
{
    /*
     * b0 = {b0, b1}
     * b1 = {b2, b3}
     */
    __m128d b0 = simd_load(b, 0);
    __m128d b1 = simd_load(b, 1);

    __m128d mul[2];
    for (size_t i = 0; i < 2; ++i) {
        /*
         * a0 = {a_n, a_n},    n = 0, 2
         * a1 = {a_m, a_m},    m = 1, 3
         */
        __m128d a0 = _mm_set1_pd(a(i,0));
        __m128d a1 = _mm_set1_pd(a(i,1));
        /*
         * c0 = {a_n * b0, a_n * b1}
         * c1 = {a_m * b2, a_m * b3}
         */
        __m128d c0 = _mm_mul_pd(a0, b0);
        __m128d c1 = _mm_mul_pd(a1, b1);
        /*
         * mul = {a_n * b0 + a_m * b2,
         *         a_n * b1 + a_m * b3}
         */
        mul[i] = _mm_add_pd(c0, c1);
    }

    mat2<double> result{};
    simd_store(result, 0, mul[0]);
    simd_store(result, 1, mul[1]);
    return result;
}


/**
 * dot
 * @brief Return the 3-dimensional dot product.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/dot.xhtml
 */
template<>
core_inline
double dot(const vec3<double> &v, const vec3<double> &w)
{
    const __m256d a = simd_load(v);
    const __m256d b = simd_load(w);
    return _mm256_cvtsd_f64(simd256_dot_(a, b));
}

template<>
core_inline
vec3<double> dot(const mat3<double> &a, const vec3<double> &v)
{
    /*
     * a0 = {a0, a1, a2}
     * a1 = {a3, a4, a5}
     * a2 = {a6, a7, a8}
     * b  = {b0, b1, b2}
     */
    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);
    __m256d b  = simd_load(v);
    /*
     * c0 = {a0*b0 + a1*b1 + a2*b2}
     * c1 = {a3*b0 + a4*b1 + a2*b2}
     * c2 = {a6*b0 + a7*b1 + a8*b2}
     *
     * _mm256_unpackhi_pd(__m256d a, __m256d b)
     * dst[63:0]    := a[127:64]
     * dst[127:64]  := b[127:64]
     * dst[191:128] := a[255:192]
     * dst[255:192] := b[255:192]
     */
    __m256d c0   = simd256_dot_(a0, b);
    __m256d c1   = simd256_dot_(a1, b);
    __m256d c2   = simd256_dot_(a2, b);
    __m256d zero = _mm256_set1_pd(0.0);
    __m256d mul  = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(c0, c1),
        _mm256_unpackhi_pd(c2, zero),
        core_extension(0b00100000));

    vec3<double> result{};
    simd_store(result, mul);
    return result;
}

template<>
core_inline
mat3<double> dot(const mat3<double> &a, const mat3<double> &b)
{
    /*
     * b0 = {b0, b1, b2}
     * b1 = {b3, b4, b5}
     * b2 = {b6, b7, b8}
     */
    __m256d b0 = simd_load(b, 0);
    __m256d b1 = simd_load(b, 1);
    __m256d b2 = simd_load(b, 2);

    __m256d mul[3];
    for (size_t i = 0; i < 3; ++i) {
        /*
         * a0 = {a_n, a_n, a_n, a_n}   n = 0, 3, 6
         * a1 = {a_m, a_m, a_m, a_m}   m = 1, 4, 7
         * a2 = {a_l, a_l, a_l, a_l}   l = 2, 5,
         */
        __m256d a0 = _mm256_set1_pd(a(i, 0));
        __m256d a1 = _mm256_set1_pd(a(i, 1));
        __m256d a2 = _mm256_set1_pd(a(i, 2));
        /*
         * c0 = {a_n * b0, a_n * b1, a_n * b2}
         * c1 = {a_m * b3, a_m * b4, a_m * b5}
         * c2 = {a_l * b6, a_l * b7, a_l * b8}
         */
        __m256d c0 = _mm256_mul_pd(a0, b0);
        __m256d c1 = _mm256_mul_pd(a1, b1);
        __m256d c2 = _mm256_mul_pd(a2, b2);
        /*
         * mul = {a_n * b0 + a_m * b3 + a_l * b6,
         *         a_n * b1 + a_m * b4 + a_l * b7,
         *         a_n * b2 + a_m * b5 + a_l * b8}
         */
         mul[i] = _mm256_set1_pd(0.0);
         mul[i] = _mm256_add_pd(mul[i], c0);
         mul[i] = _mm256_add_pd(mul[i], c1);
         mul[i] = _mm256_add_pd(mul[i], c2);
    }

    mat3<double> result{};
    simd_store(result, 0, mul[0]);
    simd_store(result, 1, mul[1]);
    simd_store(result, 2, mul[2]);
    return result;
}

/**
 * dot
 * @brief Return the 4-dimensional dot product.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/dot.xhtml
 */
template<>
core_inline
double dot(const vec4<double> &v, const vec4<double> &w)
{
    const __m256d a = simd_load(v);
    const __m256d b = simd_load(w);
    return _mm256_cvtsd_f64(simd256_dot_(a, b));
}

template<>
core_inline
vec4<double> dot(const mat4<double> &a, const vec4<double> &v)
{
    /*
     * a0 = {a0,  a1,  a2,  a3}
     * a1 = {a4,  a5,  a6,  a7}
     * a2 = {a8,  a9,  a10, a11}
     * a3 = {a12, a13, a14, a15}
     */
    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);
    __m256d a3 = simd_load(a, 3);
    __m256d b  = simd_load(v);
    /*
     * c0 = {a0*b0  + a1*b1  + a2*b2  + a3*b3}
     * c1 = {a4*b0  + a5*b1  + a6*b2  + a7*b3}
     * c2 = {a8*b0  + a9*b1  + a10*b2 + a11*b3}
     * c3 = {a12*b0 + a13*b1 + a14*b2 + a15*b3}
     *
     * _mm256_unpackhi_pd(__m256d a, __m256d b)
     * dst[63:0]    := a[127:64]
     * dst[127:64]  := b[127:64]
     * dst[191:128] := a[255:192]
     * dst[255:192] := b[255:192]
     */
    __m256d c0  = simd256_dot_(a0, b);
    __m256d c1  = simd256_dot_(a1, b);
    __m256d c2  = simd256_dot_(a2, b);
    __m256d c3  = simd256_dot_(a3, b);
    __m256d mul = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(c0, c1),
        _mm256_unpackhi_pd(c2, c3),
        core_extension(0b00100000));

    vec4<double> result{};
    simd_store(result, mul);
    return result;
}

template<>
core_inline
mat4<double> dot(const mat4<double> &a, const mat4<double> &b)
{
    /*
     * b0 = {b0,  b1,  b2,  b3}
     * b1 = {b4,  b5,  b6,  b7}
     * b2 = {b8,  b9,  b10, b11}
     * b3 = {b12, b13, b14, b15}
     */
    __m256d b0 = simd_load(b, 0);
    __m256d b1 = simd_load(b, 1);
    __m256d b2 = simd_load(b, 2);
    __m256d b3 = simd_load(b, 3);

    __m256d mul[4];
    for (size_t i = 0; i < 4; ++i) {
        /*
         * a0 = {a_n, a_n, a_n, a_n}   n = 0, 4, 8,  12
         * a1 = {a_m, a_m, a_m, a_m}   m = 1, 5, 9,  13
         * a2 = {a_l, a_l, a_l, a_l}   l = 2, 6, 10, 14
         * a3 = {a_k, a_k, a_k, a_k}   k = 3, 7, 11, 15
         */
        __m256d a0 = _mm256_set1_pd(a(i, 0));
        __m256d a1 = _mm256_set1_pd(a(i, 1));
        __m256d a2 = _mm256_set1_pd(a(i, 2));
        __m256d a3 = _mm256_set1_pd(a(i, 3));
        /*
         * c0 = {a_n * b0,  a_n * b1,  a_n * b2,  a_n * b3}
         * c1 = {a_m * b4,  a_m * b5,  a_m * b6,  a_m * b7}
         * c2 = {a_l * b8,  a_l * b9,  a_l * b10, a_l * b11}
         * c3 = {a_k * b12, a_k * b13, a_k * b14, a_k * b15}
         */
        __m256d c0 = _mm256_mul_pd(a0, b0);
        __m256d c1 = _mm256_mul_pd(a1, b1);
        __m256d c2 = _mm256_mul_pd(a2, b2);
        __m256d c3 = _mm256_mul_pd(a3, b3);
        /*
         * mul = {a_n * b0 + a_m * b4 + a_l * b8  + a_k * b12,
         *         a_n * b1 + a_m * b5 + a_l * b9  + a_k * b13,
         *         a_n * b2 + a_m * b6 + a_l * b10 + a_k * b14,
         *         a_n * b3 + a_m * b7 + a_l * b11 + a_k * b15}
         */
         mul[i] = _mm256_set1_pd(0.0);
         mul[i] = _mm256_add_pd(mul[i], c0);
         mul[i] = _mm256_add_pd(mul[i], c1);
         mul[i] = _mm256_add_pd(mul[i], c2);
         mul[i] = _mm256_add_pd(mul[i], c3);
    }

    mat4<double> result{};
    simd_store(result, 0, mul[0]);
    simd_store(result, 1, mul[1]);
    simd_store(result, 2, mul[2]);
    simd_store(result, 3, mul[3]);
    return result;
}


/** ---------------------------------------------------------------------------
 * norm
 * @brief Return the norm of the specified vector.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/length.xhtml
 */
template<>
core_inline
double norm(const vec2<double> &v)
{
    const __m128d a = simd_load(v);
    return _mm_cvtsd_f64(simd128_norm_(a));
}

template<>
core_inline
double norm(const vec3<double> &v)
{
    const __m256d a = simd_load(v);
    return _mm256_cvtsd_f64(simd256_norm_(a));
}

template<>
core_inline
double norm(const vec4<double> &v)
{
    const __m256d a = simd_load(v);
    return _mm256_cvtsd_f64(simd256_norm_(a));
}


/** ---------------------------------------------------------------------------
 * normalize
 * @brief Return the normalized vector.
 * @see
 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/normalize.xhtml
 */
template<>
core_inline
vec2<double> normalize(const vec2<double> &v)
{
    const __m128d a = simd_load(v);
    vec2<double> result{};
    simd_store(result, simd128_normalize_(a));
    return result;
}

template<>
core_inline
vec3<double> normalize(const vec3<double> &v)
{
    const __m256d a = simd_load(v);
    vec3<double> result{};
    simd_store(result, simd256_normalize_(a));
    return result;
}

template<>
core_inline
vec4<double> normalize(const vec4<double> &v)
{
    const __m256d a = simd_load(v);
    vec4<double> result{};
    simd_store(result, simd256_normalize_(a));
    return result;
}


/** ---------------------------------------------------------------------------
 * distance
 * @brief Return the distance between two vectors.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/distance.xhtml
 */
template<>
core_inline
double distance(const vec2<double> &a, const vec2<double> &b)
{
    return norm(a-b);
}

template<>
core_inline
double distance(const vec3<double> &a, const vec3<double> &b)
{
    return norm(a-b);
}

template<>
core_inline
double distance(const vec4<double> &a, const vec4<double> &b)
{
    return norm(a-b);
}


/** ---------------------------------------------------------------------------
 * cross
 * @brief Return the cross product of two vectors.
 *
 * c = a x b
 *   = {a1*b2 - a2*b1,
 *      a2*b0 - a0*b2,
 *      a0*b1 - a1*b0}
 *   = {a1*b2, a2*b0, a0*b1} -
 *     {b1*a2, b2*a0, b0*a1}
 *   = {a1, a2, a0} * {b2, b0, b1} -
 *     {b1, b2, b0} * {a2, a0, a1}
 *   = {0, a0, a2, a1} * {0, b1, b0, b2} -
 *     {0, b0, b2, b1} * {0, a1, a0, a2}
 *
 * The cross product member function is interpreted as a projection
 * operation of this vector on the rhs vector.
 * The this vector acts as a projection matrix rotating the rhs vector
 * in a direction mutually perpendicular to the original vectors,
 *
 *     |  0 -a2   a1| |b0|
 * c = | a2   0  -a0|*|b1|
 *     |-a1  a0    0| |b2|
 *   = {a1*b2 - a2*b1,
 *      a2*b0 - a0*b2,
 *      a0*b1 - a1*b0}
 *
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/cross.xhtml
 */
template<>
core_inline
vec3<double> cross(const vec3<double> &a, const vec3<double> &b)
{
    /*
     * a0 = { 0, a2, a1, a0}
     * a1 = {a1, a0,  0, a2}
     * a2 = {a0,  0, a2, a1}
     * a3 = { 0, a0, a2, a1}(*)
     * a4 = {a1,  0, a2, a0}
     * a5 = { 0, a1, a0, a2}(*)
     */
    __m256d a0 = simd_load(a);
    __m256d a1 = _mm256_permute2f128_pd(a0, a0, core_extension(0b00000001));
    __m256d a2 = _mm256_shuffle_pd(a0, a1, core_extension(0b0101));
    __m256d a3 = _mm256_permute_pd(a2, core_extension(0b0110));
    __m256d a4 = _mm256_shuffle_pd(a0, a1, core_extension(0b1100));
    __m256d a5 = _mm256_permute_pd(a4, core_extension(0b0101));
    /*
     * b0 = { 0, b2, b1, b0}
     * b1 = {b1, b0,  0, b2}
     * b2 = {b0,  0, b2, b1}
     * b3 = { 0, b0, b2, b1} (*)
     * b4 = {b1,  0, b2, b0}
     * b5 = { 0, b1, b0, b2} (*)
     */
    __m256d b0 = simd_load(b);
    __m256d b1 = _mm256_permute2f128_pd(b0, b0, core_extension(0b00000001));
    __m256d b2 = _mm256_shuffle_pd(b0, b1, core_extension(0b0101));
    __m256d b3 = _mm256_permute_pd(b2, core_extension(0b0110));
    __m256d b4 = _mm256_shuffle_pd(b0, b1, core_extension(0b1100));
    __m256d b5 = _mm256_permute_pd(b4, core_extension(0b0101));
    /*
     * c = (a3 * b5) - (a5 * b3)
     */
    vec3<double> result{};
    simd_store(
        result,
        _mm256_sub_pd(
            _mm256_mul_pd(a3, b5),
            _mm256_mul_pd(a5, b3)));
    return result;
}


/** ---------------------------------------------------------------------------
 * transpose
 * @brief Return the transpose matrix
 */
template<>
core_inline
mat2<double> transpose(const mat2<double> &a)
{
    /*
     * r0 = {a1, a0} -> {a2, a0}
     * r1 = {a3, a2} -> {a3, a1}
     */
    __m128d row[2];
    row[0] = simd_load(a, 0);
    row[1] = simd_load(a, 1);
    simd128_transpose_(row);

    mat2<double> result{};
    simd_store(result, 0, row[0]);
    simd_store(result, 1, row[1]);
    return result;
}

template<>
core_inline
mat3<double> transpose(const mat3<double> &a)
{
    /*
     * r0 = { 0,  a2,  a1,  a0} -> { 0,  a8,  a4,  a0}
     * r1 = { 0,  a6,  a5,  a4} -> { 0,  a9,  a5,  a1}
     * r2 = { 0, a10,  a9,  a8} -> { 0, a10,  a6,  a2}
     * r3 = { 0,   0,   0,   0} -> { 0,   0,   0,   0}
     */
    __m256d row[4];
    row[0] = simd_load(a, 0);
    row[1] = simd_load(a, 1);
    row[2] = simd_load(a, 2);
    row[3] = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);
    simd256_transpose_(row);

    mat3<double> result{};
    simd_store(result, 0, row[0]);
    simd_store(result, 1, row[1]);
    simd_store(result, 2, row[2]);
    return result;
}

template<>
core_inline
mat4<double> transpose(const mat4<double> &a)
{
    /*
     * r0 = { a3,  a2,  a1,  a0} -> { a12,  a8,  a4,  a0}
     * r1 = { a7,  a6,  a5,  a4} -> { a13,  a9,  a5,  a1}
     * r2 = {a11, a10,  a9,  a8} -> { a14, a10,  a6,  a2}
     * r3 = {a15, a14, a13, a12} -> { a15, a11,  a7,  a3}
     */
    __m256d row[4];
    row[0] = simd_load(a, 0);
    row[1] = simd_load(a, 1);
    row[2] = simd_load(a, 2);
    row[3] = simd_load(a, 3);
    simd256_transpose_(row);

    mat4<double> result{};
    simd_store(result, 0, row[0]);
    simd_store(result, 1, row[1]);
    simd_store(result, 2, row[2]);
    simd_store(result, 3, row[3]);
    return result;
}


/** ---------------------------------------------------------------------------
 * determinant
 * @brief Compute the determinant of the specified matrix and return
 * the lower double-precision (64-bit) floating-point element.
 */
template<>
core_inline
double determinant(const mat2<double> &a)
{
    /*
     * Compute the determinant of a 2x2 matrix as a packed vector:
     *
     * det(a) = det{a0, a1,
     *              a2, a3}
     */
    __m128d m0 = simd_load(a, 0);
    __m128d m1 = simd_load(a, 1);
    __m256d det = simd256_det_(_mm256_set_m128d(m1, m0));

    return _mm256_cvtsd_f64(det);
}

template<>
core_inline
double determinant(const mat3<double> &a)
{
    /*
     * Compute the determinant of a 3x3 matrix as a packed vector:
     *
     * det(a) = det{a0, a1, a2,
     *              a3, a4, a5,
     *              a6, a7, a8}
     */
    __m256d a0 = _mm256_set1_pd(a(0,0));
    __m256d a1 = _mm256_set1_pd(a(0,1));
    __m256d a2 = _mm256_set1_pd(a(0,2));
    /*
     * Compute the (i,j) minors of matrix a
     *
     * m0 = det{a8, a7, a5, a4}
     * m1 = det{a6, a8, a3, a5}
     * m2 = det{a7, a6, a4, a3}
     */
    __m256d m0 = simd256_det_(_mm256_set_pd(a(2,2), a(2,1), a(1,2), a(1,1)));
    __m256d m1 = simd256_det_(_mm256_set_pd(a(2,0), a(2,2), a(1,0), a(1,2)));
    __m256d m2 = simd256_det_(_mm256_set_pd(a(2,1), a(2,0), a(1,1), a(1,0)));
    /*
     * det(a) =  a0 * det{a8, a7, a5, a4}
     *          +a1 * det{a6, a8, a3, a5}
     *          +a2 * det{a7, a6, a4, a3}
     *        = a0 * m0 +
     *          a1 * m1 +
     *          a2 * m2
     */
    __m256d det = _mm256_set1_pd(0.0);
    det = _mm256_add_pd(det, _mm256_mul_pd(a0, m0));
    det = _mm256_add_pd(det, _mm256_mul_pd(a1, m1));
    det = _mm256_add_pd(det, _mm256_mul_pd(a2, m2));

    return _mm256_cvtsd_f64(det);
}

template<>
core_inline
double determinant(const mat4<double> &a)
{
    /*
     * Compute the determinant of a 4x4 matrix as a packed vector:
     *
     * det(a) = det{a0,  a1,  a2,  a3,
     *              a4,  a5,  a6,  a7,
     *              a8,  a9,  a10, a11,
     *              a12, a13, a14, a15}
     *
     * Compute the (i,j) minors of a
     *
     * m0  = det{a5,  a4,  a1,  a0}
     * m1  = det{a15, a14, a11, a10}
     *
     * m2  = det{a4,  a6,  a0,  a2}
     * m3  = det{a15, a13, a11, a9}
     *
     * m4  = det{a7,  a4,  a3,  a0}
     * m5  = det{a14, a13, a10, a9}
     *
     * m6  = det{a6,  a5,  a2,  a1}
     * m7  = det{a15, a12, a11, a8}
     *
     * m8  = det{a5,  a7,  a1,  a3}
     * m9  = det{a14, a12, a10, a8}
     *
     * m10 = det{a7,  a6,  a3, a2}
     * m11 = det{a13, a12, a9, a8}
     */
    __m256d m0  = simd256_det_(_mm256_set_pd(a(1,1), a(1,0), a(0,1), a(0,0)));
    __m256d m1  = simd256_det_(_mm256_set_pd(a(3,3), a(3,2), a(2,3), a(2,2)));

    __m256d m2  = simd256_det_(_mm256_set_pd(a(1,0), a(1,2), a(0,0), a(0,2)));
    __m256d m3  = simd256_det_(_mm256_set_pd(a(3,3), a(3,1), a(2,3), a(2,1)));

    __m256d m4  = simd256_det_(_mm256_set_pd(a(1,3), a(1,0), a(0,3), a(0,0)));
    __m256d m5  = simd256_det_(_mm256_set_pd(a(3,2), a(3,1), a(2,2), a(2,1)));

    __m256d m6  = simd256_det_(_mm256_set_pd(a(1,2), a(1,1), a(0,2), a(0,1)));
    __m256d m7  = simd256_det_(_mm256_set_pd(a(3,3), a(3,0), a(2,3), a(2,0)));

    __m256d m8  = simd256_det_(_mm256_set_pd(a(1,1), a(1,3), a(0,1), a(0,3)));
    __m256d m9  = simd256_det_(_mm256_set_pd(a(3,2), a(3,0), a(2,2), a(2,0)));

    __m256d m10 = simd256_det_(_mm256_set_pd(a(1,3), a(1,2), a(0,3), a(0,2)));
    __m256d m11 = simd256_det_(_mm256_set_pd(a(3,1), a(3,0), a(2,1), a(2,0)));
    /*
     * det(a) = m0  * m1 +
     *          m2  * m3 +
     *          m4  * m5 +
     *          m6  * m7 +
     *          m8  * m9 +
     *          m10 * m11
     */
    __m256d det = _mm256_set1_pd(0.0);
    det = _mm256_add_pd(det, _mm256_mul_pd(m0,  m1));
    det = _mm256_add_pd(det, _mm256_mul_pd(m2,  m3));
    det = _mm256_add_pd(det, _mm256_mul_pd(m4,  m5));
    det = _mm256_add_pd(det, _mm256_mul_pd(m6,  m7));
    det = _mm256_add_pd(det, _mm256_mul_pd(m8,  m9));
    det = _mm256_add_pd(det, _mm256_mul_pd(m10, m11));

    return _mm256_cvtsd_f64(det);
}


/** ---------------------------------------------------------------------------
 * inverse
 * @brief Return the inverse of the matrix.
 */
template<>
core_inline
mat2<double> inverse(const mat2<double> &a)
{
    /*
     * Compute the inverse of matrix:
     *  a = {a0, a1,
     *       a2, a3}
     *
     * from its adjugate and corresponding determinant:
     *  inv(a) = adj(a) / det(a)
     *
     * where adj(a) = C(a)^t is given in terms of the cofactor matrix
     * and corresponding minors:
     *  C(a) = (-1)^(i+j)M_ij
     */
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d one  = _mm256_set1_pd(1.0);

    __m256d adj  = _mm256_set_pd(
         a(0,0), -a(1,0),
        -a(0,1),  a(1,1));
    __m256d det  = _mm256_div_pd(one, _mm256_set1_pd(determinant(a)));
    __m256d inv  = _mm256_mul_pd(adj, det);

    /*
     * Check for NaN and set the inverse to zero if the determinant is null.
     */
    __m256d mask = _mm256_or_pd(_mm256_cmplt_pd(det, zero),
                                _mm256_cmpgt_pd(det, zero));
    inv = _mm256_and_pd(inv, mask);

    mat2<double> result{};
    simd_store(result, 0, _mm256_extractf128_pd(inv, 0));
    simd_store(result, 1, _mm256_extractf128_pd(inv, 1));
    return result;
}

template<>
core_inline
mat3<double> inverse(const mat3<double> &a)
{
    /*
     * Compute the inverse of matrix:
     *  a = {a0, a1, a2,
     *       a3, a4, a5,
     *       a6, a7, a8}
     *
     * from its adjugate and corresponding determinant:
     *  inv(a) = adj(a) / det(a)
     *
     * where adj(a) = C(a)^t is given in terms of the cofactor matrix
     * and corresponding minors:
     *  C(a) = (-1)^(i+j)M_ij
     *
     * m0  = det{a4, a5, a7, a8}
     * m1  = det{a5, a3, a8, a6}
     * m2  = det{a3, a4, a6, a7}
     *
     * m3  = det{a2, a1, a8, a7}
     * m4  = det{a0, a2, a6, a8}
     * m5  = det{a1, a0, a7, a6}
     *
     * m6  = det{a1, a2, a4, a5}
     * m7  = det{a2, a0, a5, a3}
     * m8  = det{a0, a1, a3, a4}
     *
     * adj(a) = {m0, m3, m6,
     *           m1, m4, m7,
     *           m2, m5, m8}
     */
    __m256d m0 = simd256_det_(_mm256_set_pd(a(1,1), a(1,2), a(2,1), a(2,2)));
    __m256d m1 = simd256_det_(_mm256_set_pd(a(1,2), a(1,0), a(2,2), a(2,0)));
    __m256d m2 = simd256_det_(_mm256_set_pd(a(1,0), a(1,1), a(2,0), a(2,1)));
    __m256d m3 = simd256_det_(_mm256_set_pd(a(0,2), a(0,1), a(2,2), a(2,1)));
    __m256d m4 = simd256_det_(_mm256_set_pd(a(0,0), a(0,2), a(2,0), a(2,2)));
    __m256d m5 = simd256_det_(_mm256_set_pd(a(0,1), a(0,0), a(2,1), a(2,0)));
    __m256d m6 = simd256_det_(_mm256_set_pd(a(0,1), a(0,2), a(1,1), a(1,2)));
    __m256d m7 = simd256_det_(_mm256_set_pd(a(0,2), a(0,0), a(1,2), a(1,0)));
    __m256d m8 = simd256_det_(_mm256_set_pd(a(0,0), a(0,1), a(1,0), a(1,1)));
    /*
     * _mm256_unpackhi_pd(__m256d a, __m256d b)
     * dst[63:0]    := a[127:64]
     * dst[127:64]  := b[127:64]
     * dst[191:128] := a[255:192]
     * dst[255:192] := b[255:192]
     *
     * _mm256_permute2f128_pd(__m256d a, __m256d b, int mask)
     * SELECT4(src1, src2, control) {
     *  CASE(control[1:0])
     *      0:  tmp[127:0] := src1[127:0]
     *      1:  tmp[127:0] := src1[255:128]
     *      2:  tmp[127:0] := src2[127:0]
     *      3:  tmp[127:0] := src2[255:128]
     *  ESAC
     *  IF control[3]
     *      tmp[127:0] := 0
     *  FI
     *  RETURN tmp[127:0]
     * }
     * dst[127:0]   := SELECT4(a[255:0], b[255:0], mask[3:0])
     * dst[255:128] := SELECT4(a[255:0], b[255:0], mask[7:4])
     */
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d one  = _mm256_set1_pd(1.0);
    __m256d det = _mm256_div_pd(one, _mm256_set1_pd(determinant(a)));

    /*
     * inv0 = {0, m6, m3, m0}
     */
    __m256d inv0 = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(m0, m3),
        _mm256_unpackhi_pd(m6, zero),
        core_extension(0b00100000));
    inv0 = _mm256_mul_pd(inv0, det);

    /*
     * inv1 = {0, m7, m4, m1}
     */
    __m256d inv1 = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(m1, m4),
        _mm256_unpackhi_pd(m7, zero),
        core_extension(0b00100000));
    inv1 = _mm256_mul_pd(inv1, det);

    /*
     * inv2 = {0, m8, m5, m2}
     */
    __m256d inv2 = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(m2, m5),
        _mm256_unpackhi_pd(m8, zero),
        core_extension(0b00100000));
    inv2 = _mm256_mul_pd(inv2, det);

    /*
     * Check for NaN and set the inverse to zero if the determinant is null.
     */
    __m256d mask = _mm256_or_pd(
        _mm256_cmplt_pd(det, zero),
        _mm256_cmpgt_pd(det, zero));
    inv0 = _mm256_and_pd(inv0, mask);
    inv1 = _mm256_and_pd(inv1, mask);
    inv2 = _mm256_and_pd(inv2, mask);

    mat3<double> result{};
    simd_store(result, 0, inv0);
    simd_store(result, 1, inv1);
    simd_store(result, 2, inv2);
    return result;
}

template<>
core_inline
mat4<double> inverse(const mat4<double> &a)
{
    /*
     * Compute the inverse of matrix:
     *  a = {a0,  a1,  a2,  a3,
     *       a4,  a5,  a6,  a7,
     *       a8,  a9,  a10, a11,
     *       a12, a13, a14, a15}
     *
     * from its adjugate and corresponding determinant:
     *  inv(a) = adj(a) / det(a)
     *
     * where adj(a) = C(a)^t is given in terms of the cofactor matrix
     * and corresponding minors:
     *  C(a) = (-1)^(i+j)*M_ij
     *
     * m0  = det{a10, a11, a14, a15}
     * m1  = det{a11, a9,  a15, a13}
     * m2  = det{a9,  a10, a13, a14}
     * m3  = det{a11, a8,  a15, a12}
     * m4  = det{a8,  a10, a12, a14}
     * m5  = det{a8,  a9,  a12, a13}
     *
     * m6  = det{a2,  a3,  a6,  a7}
     * m7  = det{a3,  a1,  a7,  a5}
     * m8  = det{a1,  a2,  a5,  a6}
     * m9  = det{a3,  a0,  a7,  a4}
     * m10 = det{a0,  a2,  a4,  a6}
     * m11 = det{a0,  a1,  a4,  a5}
     *
     * adj0  =  a5  * m0 + a6  * m1  + a7  * m2
     * adj1  = -a1  * m0 - a2  * m1  - a3  * m2
     * adj2  =  a13 * m6 + a14 * m7  + a15 * m8
     * adj3  = -a9  * m6 - a10 * m7  - a11 * m8
     *
     * adj4  = -a4  * m0 - a6  * m3  - a7  * m4
     * adj5  =  a0  * m0 + a2  * m3  + a3  * m4
     * adj6  = -a12 * m6 - a14 * m9  - a15 * m10
     * adj7  =  a8  * m6 + a10 * m9  + a11 * m10
     *
     * adj8  = -a4  * m1 + a5  * m3  + a7  * m5
     * adj9  =  a0  * m1 - a1  * m3  - a3  * m5
     * adj10 = -a12 * m7 + a13 * m9  + a15 * m11
     * adj11 =  a8  * m7 - a9  * m9  - a11 * m11
     *
     * adj12 = -a4  * m2 + a5  * m4  - a6  * m5
     * adj13 =  a0  * m2 - a1  * m4  + a2  * m5
     * adj14 = -a12 * m8 + a13 * m10 - a14 * m11
     * adj15 =  a8  * m8 - a9  * m10 + a10 * m11
     */
    __m256d m0  = simd256_det_(_mm256_set_pd(a(2,2), a(2,3), a(3,2), a(3,3)));
    __m256d m1  = simd256_det_(_mm256_set_pd(a(2,3), a(2,1), a(3,3), a(3,1)));

    __m256d m2  = simd256_det_(_mm256_set_pd(a(2,1), a(2,2), a(3,1), a(3,2)));
    __m256d m3  = simd256_det_(_mm256_set_pd(a(2,3), a(2,0), a(3,3), a(3,0)));

    __m256d m4  = simd256_det_(_mm256_set_pd(a(2,0), a(2,2), a(3,0), a(3,2)));
    __m256d m5  = simd256_det_(_mm256_set_pd(a(2,0), a(2,1), a(3,0), a(3,1)));

    __m256d m6  = simd256_det_(_mm256_set_pd(a(0,2), a(0,3), a(1,2), a(1,3)));
    __m256d m7  = simd256_det_(_mm256_set_pd(a(0,3), a(0,1), a(1,3), a(1,1)));

    __m256d m8  = simd256_det_(_mm256_set_pd(a(0,1), a(0,2), a(1,1), a(1,2)));
    __m256d m9  = simd256_det_(_mm256_set_pd(a(0,3), a(0,0), a(1,3), a(1,0)));

    __m256d m10 = simd256_det_(_mm256_set_pd(a(0,0), a(0,2), a(1,0), a(1,2)));
    __m256d m11 = simd256_det_(_mm256_set_pd(a(0,0), a(0,1), a(1,0), a(1,1)));

    __m256d a0  = _mm256_set1_pd(a(0,0));
    __m256d a1  = _mm256_set1_pd(a(0,1));
    __m256d a2  = _mm256_set1_pd(a(0,2));
    __m256d a3  = _mm256_set1_pd(a(0,3));

    __m256d a4  = _mm256_set1_pd(a(1,0));
    __m256d a5  = _mm256_set1_pd(a(1,1));
    __m256d a6  = _mm256_set1_pd(a(1,2));
    __m256d a7  = _mm256_set1_pd(a(1,3));

    __m256d a8  = _mm256_set1_pd(a(2,0));
    __m256d a9  = _mm256_set1_pd(a(2,1));
    __m256d a10 = _mm256_set1_pd(a(2,2));
    __m256d a11 = _mm256_set1_pd(a(2,3));

    __m256d a12 = _mm256_set1_pd(a(3,0));
    __m256d a13 = _mm256_set1_pd(a(3,1));
    __m256d a14 = _mm256_set1_pd(a(3,2));
    __m256d a15 = _mm256_set1_pd(a(3,3));

    /*
     * adj0  =  a5  * m0 + a6  * m1  + a7  * m2
     * adj1  = -a1  * m0 - a2  * m1  - a3  * m2
     * adj2  =  a13 * m6 + a14 * m7  + a15 * m8
     * adj3  = -a9  * m6 - a10 * m7  - a11 * m8
     */
    __m256d adj0 = _mm256_set1_pd(0.0);
    adj0 = _mm256_add_pd(adj0, _mm256_mul_pd(a5, m0));
    adj0 = _mm256_add_pd(adj0, _mm256_mul_pd(a6, m1));
    adj0 = _mm256_add_pd(adj0, _mm256_mul_pd(a7, m2));

    __m256d adj1 = _mm256_set1_pd(0.0);
    adj1 = _mm256_sub_pd(adj1, _mm256_mul_pd(a1, m0));
    adj1 = _mm256_sub_pd(adj1, _mm256_mul_pd(a2, m1));
    adj1 = _mm256_sub_pd(adj1, _mm256_mul_pd(a3, m2));

    __m256d adj2 = _mm256_set1_pd(0.0);
    adj2 = _mm256_add_pd(adj2, _mm256_mul_pd(a13, m6));
    adj2 = _mm256_add_pd(adj2, _mm256_mul_pd(a14, m7));
    adj2 = _mm256_add_pd(adj2, _mm256_mul_pd(a15, m8));

    __m256d adj3 = _mm256_set1_pd(0.0);
    adj3 = _mm256_sub_pd(adj3, _mm256_mul_pd(a9,  m6));
    adj3 = _mm256_sub_pd(adj3, _mm256_mul_pd(a10, m7));
    adj3 = _mm256_sub_pd(adj3, _mm256_mul_pd(a11, m8));

    /*
     * adj4  = -a4  * m0 - a6  * m3  - a7  * m4
     * adj5  =  a0  * m0 + a2  * m3  + a3  * m4
     * adj6  = -a12 * m6 - a14 * m9  - a15 * m10
     * adj7  =  a8  * m6 + a10 * m9  + a11 * m10
     */
    __m256d adj4 = _mm256_set1_pd(0.0);
    adj4 = _mm256_sub_pd(adj4, _mm256_mul_pd(a4, m0));
    adj4 = _mm256_sub_pd(adj4, _mm256_mul_pd(a6, m3));
    adj4 = _mm256_sub_pd(adj4, _mm256_mul_pd(a7, m4));

    __m256d adj5 = _mm256_set1_pd(0.0);
    adj5 = _mm256_add_pd(adj5, _mm256_mul_pd(a0, m0));
    adj5 = _mm256_add_pd(adj5, _mm256_mul_pd(a2, m3));
    adj5 = _mm256_add_pd(adj5, _mm256_mul_pd(a3, m4));

    __m256d adj6 = _mm256_set1_pd(0.0);
    adj6 = _mm256_sub_pd(adj6, _mm256_mul_pd(a12, m6));
    adj6 = _mm256_sub_pd(adj6, _mm256_mul_pd(a14, m9));
    adj6 = _mm256_sub_pd(adj6, _mm256_mul_pd(a15, m10));

    __m256d adj7 = _mm256_set1_pd(0.0);
    adj7 = _mm256_add_pd(adj7, _mm256_mul_pd(a8,  m6));
    adj7 = _mm256_add_pd(adj7, _mm256_mul_pd(a10, m9));
    adj7 = _mm256_add_pd(adj7, _mm256_mul_pd(a11, m10));

    /*
     * adj8  = -a4  * m1 + a5  * m3  + a7  * m5
     * adj9  =  a0  * m1 - a1  * m3  - a3  * m5
     * adj10 = -a12 * m7 + a13 * m9  + a15 * m11
     * adj11 =  a8  * m7 - a9  * m9  - a11 * m11
     */
    __m256d adj8 = _mm256_set1_pd(0.0);
    adj8  = _mm256_sub_pd(adj8, _mm256_mul_pd(a4, m1));
    adj8  = _mm256_add_pd(adj8, _mm256_mul_pd(a5, m3));
    adj8  = _mm256_add_pd(adj8, _mm256_mul_pd(a7, m5));

    __m256d adj9 = _mm256_set1_pd(0.0);
    adj9  = _mm256_add_pd(adj9, _mm256_mul_pd(a0, m1));
    adj9  = _mm256_sub_pd(adj9, _mm256_mul_pd(a1, m3));
    adj9  = _mm256_sub_pd(adj9, _mm256_mul_pd(a3, m5));

    __m256d adj10 = _mm256_set1_pd(0.0);
    adj10 = _mm256_sub_pd(adj10, _mm256_mul_pd(a12, m7));
    adj10 = _mm256_add_pd(adj10, _mm256_mul_pd(a13, m9));
    adj10 = _mm256_add_pd(adj10, _mm256_mul_pd(a15, m11));

    __m256d adj11 = _mm256_set1_pd(0.0);
    adj11 = _mm256_add_pd(adj11, _mm256_mul_pd(a8,  m7));
    adj11 = _mm256_sub_pd(adj11, _mm256_mul_pd(a9,  m9));
    adj11 = _mm256_sub_pd(adj11, _mm256_mul_pd(a11, m11));

    /*
     * adj12 = -a4  * m2 + a5  * m4  - a6  * m5
     * adj13 =  a0  * m2 - a1  * m4  + a2  * m5
     * adj14 = -a12 * m8 + a13 * m10 - a14 * m11
     * adj15 =  a8  * m8 - a9  * m10 + a10 * m11
     */
    __m256d adj12 = _mm256_set1_pd(0.0);
    adj12 = _mm256_sub_pd(adj12, _mm256_mul_pd(a4, m2));
    adj12 = _mm256_add_pd(adj12, _mm256_mul_pd(a5, m4));
    adj12 = _mm256_sub_pd(adj12, _mm256_mul_pd(a6, m5));

    __m256d adj13 = _mm256_set1_pd(0.0);
    adj13 = _mm256_add_pd(adj13, _mm256_mul_pd(a0, m2));
    adj13 = _mm256_sub_pd(adj13, _mm256_mul_pd(a1, m4));
    adj13 = _mm256_add_pd(adj13, _mm256_mul_pd(a2, m5));

    __m256d adj14 = _mm256_set1_pd(0.0);
    adj14 = _mm256_sub_pd(adj14, _mm256_mul_pd(a12, m8));
    adj14 = _mm256_add_pd(adj14, _mm256_mul_pd(a13, m10));
    adj14 = _mm256_sub_pd(adj14, _mm256_mul_pd(a14, m11));

    __m256d adj15 = _mm256_set1_pd(0.0);
    adj15 = _mm256_add_pd(adj15, _mm256_mul_pd(a8,  m8));
    adj15 = _mm256_sub_pd(adj15, _mm256_mul_pd(a9,  m10));
    adj15 = _mm256_add_pd(adj15, _mm256_mul_pd(a10, m11));

    /*
     * _mm256_unpackhi_pd(__m256d a, __m256d b)
     * dst[63:0]    := a[127:64]
     * dst[127:64]  := b[127:64]
     * dst[191:128] := a[255:192]
     * dst[255:192] := b[255:192]
     *
     * _mm256_permute2f128_pd(__m256d a, __m256d b, int mask)
     * SELECT4(src1, src2, control) {
     *  CASE(control[1:0])
     *      0:  tmp[127:0] := src1[127:0]
     *      1:  tmp[127:0] := src1[255:128]
     *      2:  tmp[127:0] := src2[127:0]
     *      3:  tmp[127:0] := src2[255:128]
     *  ESAC
     *  IF control[3]
     *      tmp[127:0] := 0
     *  FI
     *  RETURN tmp[127:0]
     * }
     * dst[127:0]   := SELECT4(a[255:0], b[255:0], mask[3:0])
     * dst[255:128] := SELECT4(a[255:0], b[255:0], mask[7:4])
     */
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d one  = _mm256_set1_pd(1.0);
    __m256d det = _mm256_div_pd(one, _mm256_set1_pd(determinant(a)));

    /*
     * inv0 = {adj3, adj2, adj1, adj0}
     */
    __m256d inv0 = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(adj0, adj1),
        _mm256_unpackhi_pd(adj2, adj3),
        core_extension(0b00100000));
    inv0 = _mm256_mul_pd(inv0, det);

    /*
     * inv1 = {adj7, adj6, adj5, adj4}
     */
    __m256d inv1 = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(adj4, adj5),
        _mm256_unpackhi_pd(adj6, adj7),
        core_extension(0b00100000));
    inv1 = _mm256_mul_pd(inv1, det);

    /*
     * inv2 = {adj11, adj10, adj9, adj8}
     */
    __m256d inv2 = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(adj8,  adj9),
        _mm256_unpackhi_pd(adj10, adj11),
        core_extension(0b00100000));
    inv2 = _mm256_mul_pd(inv2, det);

    /*
     * inv3 = {adj15, adj14, adj13, adj12}
     */
    __m256d inv3 = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(adj12, adj13),
        _mm256_unpackhi_pd(adj14, adj15),
        core_extension(0b00100000));
    inv3 = _mm256_mul_pd(inv3, det);

    /*
     * Check for NaN and set the inverse to zero if the determinant is null.
     */
    __m256d mask = _mm256_or_pd(
        _mm256_cmplt_pd(det, zero),
        _mm256_cmpgt_pd(det, zero));
    inv0 = _mm256_and_pd(inv0, mask);
    inv1 = _mm256_and_pd(inv1, mask);
    inv2 = _mm256_and_pd(inv2, mask);
    inv3 = _mm256_and_pd(inv3, mask);

    mat4<double> result{};
    simd_store(result, 0, inv0);
    simd_store(result, 1, inv1);
    simd_store(result, 2, inv2);
    simd_store(result, 3, inv3);
    return result;
}

} /* math */
} /* atto */

#endif /* ATTO_MATH_GEOMETRY_ALGEBRA_SIMD_H_ */
