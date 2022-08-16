/*
 * arithmetic-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_ARITHMETIC_SIMD_H_
#define ATTO_MATH_GEOMETRY_ARITHMETIC_SIMD_H_

namespace atto {
namespace math {

/** ---------------------------------------------------------------------------
 * abs
 * @brief Return the absolute value of the vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/abs.xhtml
 *
 * |a| = max((0-a), a) = max(-a,a)
 */
template<>
core_inline
vec2<double> abs(const vec2<double> &a)
{
    const __m128d zero = _mm_set1_pd(0.0);

    __m128d v = simd_load(a);
    __m128d abs_v = _mm_max_pd(_mm_sub_pd(zero, v), v);

    vec2<double> result{};
    simd_store(result, abs_v);
    return result;
}

template<>
core_inline
vec3<double> abs(const vec3<double> &a)
{
    const __m256d zero = _mm256_set1_pd(0.0);

    __m256d v = simd_load(a);
    __m256d abs_v = _mm256_max_pd(_mm256_sub_pd(zero, v), v);

    vec3<double> result{};
    simd_store(result, abs_v);
    return result;
}

template<>
core_inline
vec4<double> abs(const vec4<double> &a)
{
    const __m256d zero = _mm256_set1_pd(0.0);

    __m256d v = simd_load(a);
    __m256d abs_v = _mm256_max_pd(_mm256_sub_pd(zero, v), v);

    vec4<double> result{};
    simd_store(result, abs_v);
    return result;
}

template<>
core_inline
mat2<double> abs(const mat2<double> &a)
{
    const __m128d zero = _mm_set1_pd(0.0);

    __m128d a0 = simd_load(a, 0);
    __m128d a1 = simd_load(a, 1);
    __m128d abs0 = _mm_max_pd(_mm_sub_pd(zero, a0), a0);
    __m128d abs1 = _mm_max_pd(_mm_sub_pd(zero, a1), a1);

    mat2<double> result{};
    simd_store(result, 0, abs0);
    simd_store(result, 1, abs1);
    return result;
}

template<>
core_inline
mat3<double> abs(const mat3<double> &a)
{
    const __m256d zero = _mm256_set1_pd(0.0);

    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);
    __m256d abs0 = _mm256_max_pd(_mm256_sub_pd(zero, a0), a0);
    __m256d abs1 = _mm256_max_pd(_mm256_sub_pd(zero, a1), a1);
    __m256d abs2 = _mm256_max_pd(_mm256_sub_pd(zero, a2), a2);

    mat3<double> result{};
    simd_store(result, 0, abs0);
    simd_store(result, 1, abs1);
    simd_store(result, 2, abs2);
    return result;
}

template<>
core_inline
mat4<double> abs(const mat4<double> &a)
{
    const __m256d zero = _mm256_set1_pd(0.0);

    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);
    __m256d a3 = simd_load(a, 3);
    __m256d abs0 = _mm256_max_pd(_mm256_sub_pd(zero, a0), a0);
    __m256d abs1 = _mm256_max_pd(_mm256_sub_pd(zero, a1), a1);
    __m256d abs2 = _mm256_max_pd(_mm256_sub_pd(zero, a2), a2);
    __m256d abs3 = _mm256_max_pd(_mm256_sub_pd(zero, a3), a3);

    mat4<double> result{};
    simd_store(result, 0, abs0);
    simd_store(result, 1, abs1);
    simd_store(result, 2, abs2);
    simd_store(result, 3, abs3);
    return result;
}


/** ---------------------------------------------------------------------------
 * sign
 * @brief Extract the sign of the vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/sign.xhtml
 */
template<>
core_inline
vec2<double> sign(const vec2<double> &v)
{
    const __m128d zero = _mm_set1_pd(0.0);
    const __m128d plus_one = _mm_set1_pd( 1.0);
    const __m128d minus_one = _mm_set1_pd(-1.0);

    __m128d a = simd_load(v);
    __m128d positive = _mm_and_pd(_mm_cmpgt_pd(a, zero), plus_one);
    __m128d negative = _mm_and_pd(_mm_cmplt_pd(a, zero), minus_one);
    __m128d sign_v = _mm_or_pd(positive, negative);

    vec2<double> result{};
    simd_store(result, sign_v);
    return result;
}

template<>
core_inline
vec3<double> sign(const vec3<double> &v)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d plus_one = _mm256_set1_pd( 1.0);
    const __m256d minus_one = _mm256_set1_pd(-1.0);

    __m256d a = simd_load(v);
    __m256d positive = _mm256_and_pd(_mm256_cmpgt_pd(a, zero), plus_one);
    __m256d negative = _mm256_and_pd(_mm256_cmplt_pd(a, zero), minus_one);
    __m256d sign_v = _mm256_or_pd(positive, negative);

    vec3<double> result{};
    simd_store(result, sign_v);
    return result;
}

template<>
core_inline
vec4<double> sign(const vec4<double> &v)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d plus_one = _mm256_set1_pd( 1.0);
    const __m256d minus_one = _mm256_set1_pd(-1.0);

    __m256d a = simd_load(v);
    __m256d positive = _mm256_and_pd(_mm256_cmpgt_pd(a, zero), plus_one);
    __m256d negative = _mm256_and_pd(_mm256_cmplt_pd(a, zero), minus_one);
    __m256d sign_v = _mm256_or_pd(positive, negative);

    vec4<double> result{};
    simd_store(result, sign_v);
    return result;
}

template<>
core_inline
mat2<double> sign(const mat2<double> &a)
{
    const __m128d zero = _mm_set1_pd(0.0);
    const __m128d plus_one = _mm_set1_pd( 1.0);
    const __m128d minus_one = _mm_set1_pd(-1.0);

    __m128d a0 = simd_load(a, 0);
    __m128d a1 = simd_load(a, 1);

    __m128d sign0 = _mm_or_pd(
        _mm_and_pd(_mm_cmpgt_pd(a0, zero), plus_one),
        _mm_and_pd(_mm_cmplt_pd(a0, zero), minus_one));
    __m128d sign1 = _mm_or_pd(
        _mm_and_pd(_mm_cmpgt_pd(a1, zero), plus_one),
        _mm_and_pd(_mm_cmplt_pd(a1, zero), minus_one));

    mat2<double> result{};
    simd_store(result, 0, sign0);
    simd_store(result, 1, sign1);
    return result;
}

template<>
core_inline
mat3<double> sign(const mat3<double> &a)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d plus_one = _mm256_set1_pd( 1.0);
    const __m256d minus_one = _mm256_set1_pd(-1.0);

    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);

    __m256d sign0 = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a0, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a0, zero), minus_one));
    __m256d sign1 = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a1, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a1, zero), minus_one));
    __m256d sign2 = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a2, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a2, zero), minus_one));

    mat3<double> result{};
    simd_store(result, 0, sign0);
    simd_store(result, 1, sign1);
    simd_store(result, 2, sign2);
    return result;
}

template<>
core_inline
mat4<double> sign(const mat4<double> &a)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d plus_one = _mm256_set1_pd( 1.0);
    const __m256d minus_one = _mm256_set1_pd(-1.0);

    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);
    __m256d a3 = simd_load(a, 3);

    __m256d sign0 = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a0, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a0, zero), minus_one));
    __m256d sign1 = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a1, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a1, zero), minus_one));
    __m256d sign2 = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a2, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a2, zero), minus_one));
    __m256d sign3 = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a3, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a3, zero), minus_one));

    mat4<double> result{};
    simd_store(result, 0, sign0);
    simd_store(result, 1, sign1);
    simd_store(result, 2, sign2);
    simd_store(result, 3, sign3);
    return result;
}


/** ---------------------------------------------------------------------------
 * floor
 * @brief Find the nearest integer less than or equal to the vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/floor.xhtml
 */
template<>
core_inline
vec2<double> floor(const vec2<double> &v)
{
    vec2<double> result{};
    simd_store(result, _mm_floor_pd(simd_load(v)));
    return result;
}

template<>
core_inline
vec3<double> floor(const vec3<double> &v)
{
    vec3<double> result{};
    simd_store(result, _mm256_floor_pd(simd_load(v)));
    return result;
}

template<>
core_inline
vec4<double> floor(const vec4<double> &v)
{
    vec4<double> result{};
    simd_store(result, _mm256_floor_pd(simd_load(v)));
    return result;
}

template<>
core_inline
mat2<double> floor(const mat2<double> &a)
{
    __m128d a0 = simd_load(a, 0);
    __m128d a1 = simd_load(a, 1);

    mat2<double> result{};
    simd_store(result, 0, _mm_floor_pd(a0));
    simd_store(result, 1, _mm_floor_pd(a1));
    return result;
}

template<>
core_inline
mat3<double> floor(const mat3<double> &a)
{
    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);

    mat3<double> result{};
    simd_store(result, 0, _mm256_floor_pd(a0));
    simd_store(result, 1, _mm256_floor_pd(a1));
    simd_store(result, 2, _mm256_floor_pd(a2));
    return result;
}

template<>
core_inline
mat4<double> floor(const mat4<double> &a)
{
    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);
    __m256d a3 = simd_load(a, 3);

    mat4<double> result{};
    simd_store(result, 0, _mm256_floor_pd(a0));
    simd_store(result, 1, _mm256_floor_pd(a1));
    simd_store(result, 2, _mm256_floor_pd(a2));
    simd_store(result, 3, _mm256_floor_pd(a3));
    return result;
}


/** ---------------------------------------------------------------------------
 * round
 * @brief Find the nearest integer to the vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/round.xhtml
 *
 * @fn dst = __m128d _mm_round_pd(__m128d src, int rounding)
 * Round the packed double-precision (64-bit) floating-point elements
 * in src using the rounding parameter, and store the results as packed
 * double-precision floating-point elements in dst.
 *
 * Rounding is done according to the rounding[3:0] parameter:
 *
 *  round to nearest, and suppress exceptions
 *      (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC)
 *  round down, and suppress exceptions
 *      (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)
 *  round up, and suppress exceptions
 *      (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)
 *  truncate, and suppress exceptions
 *      (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)
 */
template<>
core_inline
vec2<double> round(const vec2<double> &v)
{
    static constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;
    vec2<double> result{};
    simd_store(result, _mm_round_pd(simd_load(v), rounding));
    return result;
}

template<>
core_inline
vec3<double> round(const vec3<double> &v)
{
    static constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;
    vec3<double> result{};
    simd_store(result, _mm256_round_pd(simd_load(v), rounding));
    return result;
}

template<>
core_inline
vec4<double> round(const vec4<double> &v)
{
    static constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;
    vec4<double> result{};
    simd_store(result, _mm256_round_pd(simd_load(v), rounding));
    return result;
}

template<>
core_inline
mat2<double> round(const mat2<double> &a)
{
    static constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;

    __m128d a0 = simd_load(a, 0);
    __m128d a1 = simd_load(a, 1);

    mat2<double> result{};
    simd_store(result, 0, _mm_round_pd(a0, rounding));
    simd_store(result, 1, _mm_round_pd(a1, rounding));
    return result;
}

template<>
core_inline
mat3<double> round(const mat3<double> &a)
{
    static constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;

    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);

    mat3<double> result{};
    simd_store(result, 0, _mm256_round_pd(a0, rounding));
    simd_store(result, 1, _mm256_round_pd(a1, rounding));
    simd_store(result, 2, _mm256_round_pd(a2, rounding));
    return result;
}

template<>
core_inline
mat4<double> round(const mat4<double> &a)
{
    static constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;

    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);
    __m256d a3 = simd_load(a, 3);

    mat4<double> result{};
    simd_store(result, 0, _mm256_round_pd(a0, rounding));
    simd_store(result, 1, _mm256_round_pd(a1, rounding));
    simd_store(result, 2, _mm256_round_pd(a2, rounding));
    simd_store(result, 3, _mm256_round_pd(a3, rounding));
    return result;
}


/** ---------------------------------------------------------------------------
 * ceil
 * @brief Find the nearest integer that is greater than or equal to the
 * vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/ceil.xhtml
 */
template<>
core_inline
vec2<double> ceil(const vec2<double> &v)
{
    vec2<double> result{};
    simd_store(result, _mm_ceil_pd(simd_load(v)));
    return result;
}

template<>
core_inline
vec3<double> ceil(const vec3<double> &v)
{
    vec3<double> result{};
    simd_store(result, _mm256_ceil_pd(simd_load(v)));
    return result;
}

template<>
core_inline
vec4<double> ceil(const vec4<double> &v)
{
    vec4<double> result{};
    simd_store(result, _mm256_ceil_pd(simd_load(v)));
    return result;
}

template<>
core_inline
mat2<double> ceil(const mat2<double> &a)
{
    __m128d a0 = simd_load(a, 0);
    __m128d a1 = simd_load(a, 1);

    mat2<double> result{};
    simd_store(result, 0, _mm_ceil_pd(a0));
    simd_store(result, 1, _mm_ceil_pd(a1));
    return result;
}

template<>
core_inline
mat3<double> ceil(const mat3<double> &a)
{
    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);

    mat3<double> result{};
    simd_store(result, 0, _mm256_ceil_pd(a0));
    simd_store(result, 1, _mm256_ceil_pd(a1));
    simd_store(result, 2, _mm256_ceil_pd(a2));
    return result;
}

template<>
core_inline
mat4<double> ceil(const mat4<double> &a)
{
    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);
    __m256d a3 = simd_load(a, 3);

    mat4<double> result{};
    simd_store(result, 0, _mm256_ceil_pd(a0));
    simd_store(result, 1, _mm256_ceil_pd(a1));
    simd_store(result, 2, _mm256_ceil_pd(a2));
    simd_store(result, 3, _mm256_ceil_pd(a3));
    return result;
}


/** ---------------------------------------------------------------------------
 * clamp
 * @brief Constrain the vector elements to lie inside the range
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/clamp.xhtml
 *
 * b = max(a, lo)
 * c = min(b, hi) = min(max(a, lo), hi);
 */
template<>
core_inline
vec2<double> clamp(const vec2<double> &v, const double lo, const double hi)
{
    const __m128d a_lo = _mm_set1_pd(lo);
    const __m128d a_hi = _mm_set1_pd(hi);
    __m128d a = simd_load(v);

    vec2<double> result{};
    simd_store(result, _mm_min_pd(_mm_max_pd(a, a_lo), a_hi));
    return result;
}

template<>
core_inline
vec3<double> clamp(const vec3<double> &v, const double lo, const double hi)
{
    const __m256d a_lo = _mm256_set1_pd(lo);
    const __m256d a_hi = _mm256_set1_pd(hi);
    __m256d a = simd_load(v);

    vec3<double> result{};
    simd_store(result, _mm256_min_pd(_mm256_max_pd(a, a_lo), a_hi));
    return result;
}

template<>
core_inline
vec4<double> clamp(const vec4<double> &v, const double lo, const double hi)
{
    const __m256d a_lo = _mm256_set1_pd(lo);
    const __m256d a_hi = _mm256_set1_pd(hi);
    __m256d a = simd_load(v);

    vec4<double> result{};
    simd_store(result, _mm256_min_pd(_mm256_max_pd(a, a_lo), a_hi));
    return result;
}

template<>
core_inline
mat2<double> clamp(const mat2<double> &a, const double lo, const double hi)
{
    const __m128d a_lo = _mm_set1_pd(lo);
    const __m128d a_hi = _mm_set1_pd(hi);

    __m128d a0 = simd_load(a, 0);
    __m128d a1 = simd_load(a, 1);

    mat2<double> result{};
    simd_store(result, 0, _mm_min_pd(_mm_max_pd(a0, a_lo), a_hi));
    simd_store(result, 1, _mm_min_pd(_mm_max_pd(a1, a_lo), a_hi));
    return result;
}

template<>
core_inline
mat3<double> clamp(const mat3<double> &a, const double lo, const double hi)
{
    const __m256d a_lo = _mm256_set1_pd(lo);
    const __m256d a_hi = _mm256_set1_pd(hi);

    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);

    mat3<double> result{};
    simd_store(result, 0, _mm256_min_pd(_mm256_max_pd(a0, a_lo), a_hi));
    simd_store(result, 1, _mm256_min_pd(_mm256_max_pd(a1, a_lo), a_hi));
    simd_store(result, 2, _mm256_min_pd(_mm256_max_pd(a2, a_lo), a_hi));
    return result;
}

template<>
core_inline
mat4<double> clamp(const mat4<double> &a, const double lo, const double hi)
{
    const __m256d a_lo = _mm256_set1_pd(lo);
    const __m256d a_hi = _mm256_set1_pd(hi);

    __m256d a0 = simd_load(a, 0);
    __m256d a1 = simd_load(a, 1);
    __m256d a2 = simd_load(a, 2);
    __m256d a3 = simd_load(a, 3);

    mat4<double> result{};
    simd_store(result, 0, _mm256_min_pd(_mm256_max_pd(a0, a_lo), a_hi));
    simd_store(result, 1, _mm256_min_pd(_mm256_max_pd(a1, a_lo), a_hi));
    simd_store(result, 2, _mm256_min_pd(_mm256_max_pd(a2, a_lo), a_hi));
    simd_store(result, 3, _mm256_min_pd(_mm256_max_pd(a3, a_lo), a_hi));
    return result;
}


/** ---------------------------------------------------------------------------
 * lerp
 * @brief Linearly interpolate between two vectors.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/mix.xhtml
 *
 * lerp = (1.0 - alpha) * lo + alpha * hi
 */
template<>
core_inline
vec2<double> lerp(const vec2<double> lo, const vec2<double> hi, double alpha)
{
    const __m128d alpha_lo = _mm_set1_pd(1.0 - alpha);
    const __m128d alpha_hi = _mm_set1_pd(alpha);

    __m128d res = _mm_add_pd(
        _mm_mul_pd(alpha_lo, simd_load(lo)),
        _mm_mul_pd(alpha_hi, simd_load(hi)));

    vec2<double> result{};
    simd_store(result, res);
    return result;
}

template<>
core_inline
vec3<double> lerp(const vec3<double> lo, const vec3<double> hi, double alpha)
{
    const __m256d alpha_lo = _mm256_set1_pd(1.0 - alpha);
    const __m256d alpha_hi = _mm256_set1_pd(alpha);

    __m256d res = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo, simd_load(lo)),
        _mm256_mul_pd(alpha_hi, simd_load(hi)));

    vec3<double> result{};
    simd_store(result, res);
    return result;
}

template<>
core_inline
vec4<double> lerp(const vec4<double> lo, const vec4<double> hi, double alpha)
{
    const __m256d alpha_lo = _mm256_set1_pd(1.0 - alpha);
    const __m256d alpha_hi = _mm256_set1_pd(alpha);

    __m256d res = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo, simd_load(lo)),
        _mm256_mul_pd(alpha_hi, simd_load(hi)));

    vec4<double> result{};
    simd_store(result, res);
    return result;
}

template<>
core_inline
mat2<double> lerp(const mat2<double> lo, const mat2<double> hi, double alpha)
{
    const __m128d alpha_lo = _mm_set1_pd(1.0 - alpha);
    const __m128d alpha_hi = _mm_set1_pd(alpha);

    __m128d res0 = _mm_add_pd(
        _mm_mul_pd(alpha_lo, simd_load(lo, 0)),
        _mm_mul_pd(alpha_hi, simd_load(hi, 0)));
    __m128d res1 = _mm_add_pd(
        _mm_mul_pd(alpha_lo, simd_load(lo, 1)),
        _mm_mul_pd(alpha_hi, simd_load(hi, 1)));

    mat2<double> result{};
    simd_store(result, 0, res0);
    simd_store(result, 1, res1);
    return result;
}

template<>
core_inline
mat3<double> lerp(const mat3<double> lo, const mat3<double> hi, double alpha)
{
    const __m256d alpha_lo = _mm256_set1_pd(1.0 - alpha);
    const __m256d alpha_hi = _mm256_set1_pd(alpha);

    __m256d res0 = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo, simd_load(lo, 0)),
        _mm256_mul_pd(alpha_hi, simd_load(hi, 0)));
    __m256d res1 = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo, simd_load(lo, 1)),
        _mm256_mul_pd(alpha_hi, simd_load(hi, 1)));
    __m256d res2 = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo, simd_load(lo, 2)),
        _mm256_mul_pd(alpha_hi, simd_load(hi, 2)));

    mat3<double> result{};
    simd_store(result, 0, res0);
    simd_store(result, 1, res1);
    simd_store(result, 2, res2);
    return result;
}

template<>
core_inline
mat4<double> lerp(const mat4<double> lo, const mat4<double> hi, double alpha)
{
    const __m256d alpha_lo = _mm256_set1_pd(1.0 - alpha);
    const __m256d alpha_hi = _mm256_set1_pd(alpha);

    __m256d res0 = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo, simd_load(lo, 0)),
        _mm256_mul_pd(alpha_hi, simd_load(hi, 0)));
    __m256d res1 = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo, simd_load(lo, 1)),
        _mm256_mul_pd(alpha_hi, simd_load(hi, 1)));
    __m256d res2 = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo, simd_load(lo, 2)),
        _mm256_mul_pd(alpha_hi, simd_load(hi, 2)));
    __m256d res3 = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo, simd_load(lo, 3)),
        _mm256_mul_pd(alpha_hi, simd_load(hi, 3)));

    mat4<double> result{};
    simd_store(result, 0, res0);
    simd_store(result, 1, res1);
    simd_store(result, 2, res2);
    simd_store(result, 3, res3);
    return result;
}

} /* math */
} /* atto */

#endif /* ATTO_MATH_GEOMETRY_ARITHMETIC_SIMD_H_ */
