/*
 * mat4-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_MAT4_SIMD_H_
#define ATTO_MATH_GEOMETRY_MAT4_SIMD_H_

#include "atto/math/geometry/simd.hpp"

namespace atto {
namespace math {

/** ---- simd load/store functions --------------------------------------------
 * simd_load
 * @brief Load 256-bits (4 packed double precision 64-bit)
 * from the specified row in the 4d-matrix (32-byte aligned).
 */
core_inline
__m256d simd_load(const mat4<double> &mat, const size_t row)
{
    return _mm256_load_pd(mat.data() + row*mat.dim());
}

/**
 * simd_store
 * @brief Store 256-bits (4 packed double precision 64-bit)
 * into the specified row in the 4d-matrix (32-byte aligned).
 */
core_inline
void simd_store(mat4<double> &mat, const size_t row, const __m256d a)
{
    _mm256_store_pd(mat.data() + row*mat.dim(), a);
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic matrix operators.
 */
template<>
core_inline
mat4<double> &mat4<double>::operator+=(const mat4<double> &other)
{
    const __m256d a0 = simd_load(*this, 0);
    const __m256d a1 = simd_load(*this, 1);
    const __m256d a2 = simd_load(*this, 2);
    const __m256d a3 = simd_load(*this, 3);

    const __m256d b0 = simd_load(other, 0);
    const __m256d b1 = simd_load(other, 1);
    const __m256d b2 = simd_load(other, 2);
    const __m256d b3 = simd_load(other, 3);

    simd_store(*this, 0, _mm256_add_pd(a0, b0));
    simd_store(*this, 1, _mm256_add_pd(a1, b1));
    simd_store(*this, 2, _mm256_add_pd(a2, b2));
    simd_store(*this, 3, _mm256_add_pd(a3, b3));
    return *this;
}

template<>
core_inline
mat4<double> &mat4<double>::operator-=(const mat4<double> &other)
{
    const __m256d a0 = simd_load(*this, 0);
    const __m256d a1 = simd_load(*this, 1);
    const __m256d a2 = simd_load(*this, 2);
    const __m256d a3 = simd_load(*this, 3);

    const __m256d b0 = simd_load(other, 0);
    const __m256d b1 = simd_load(other, 1);
    const __m256d b2 = simd_load(other, 2);
    const __m256d b3 = simd_load(other, 3);

    simd_store(*this, 0, _mm256_sub_pd(a0, b0));
    simd_store(*this, 1, _mm256_sub_pd(a1, b1));
    simd_store(*this, 2, _mm256_sub_pd(a2, b2));
    simd_store(*this, 3, _mm256_sub_pd(a3, b3));
    return *this;
}

template<>
core_inline
mat4<double> &mat4<double>::operator*=(const mat4<double> &other)
{
    const __m256d a0 = simd_load(*this, 0);
    const __m256d a1 = simd_load(*this, 1);
    const __m256d a2 = simd_load(*this, 2);
    const __m256d a3 = simd_load(*this, 3);

    const __m256d b0 = simd_load(other, 0);
    const __m256d b1 = simd_load(other, 1);
    const __m256d b2 = simd_load(other, 2);
    const __m256d b3 = simd_load(other, 3);

    simd_store(*this, 0, _mm256_mul_pd(a0, b0));
    simd_store(*this, 1, _mm256_mul_pd(a1, b1));
    simd_store(*this, 2, _mm256_mul_pd(a2, b2));
    simd_store(*this, 3, _mm256_mul_pd(a3, b3));
    return *this;
}

template<>
core_inline
mat4<double> &mat4<double>::operator/=(const mat4<double> &other)
{
    const __m256d a0 = simd_load(*this, 0);
    const __m256d a1 = simd_load(*this, 1);
    const __m256d a2 = simd_load(*this, 2);
    const __m256d a3 = simd_load(*this, 3);

    const __m256d b0 = simd_load(other, 0);
    const __m256d b1 = simd_load(other, 1);
    const __m256d b2 = simd_load(other, 2);
    const __m256d b3 = simd_load(other, 3);

    simd_store(*this, 0, _mm256_div_pd(a0, b0));
    simd_store(*this, 1, _mm256_div_pd(a1, b1));
    simd_store(*this, 2, _mm256_div_pd(a2, b2));
    simd_store(*this, 3, _mm256_div_pd(a3, b3));
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<>
core_inline
mat4<double> &mat4<double>::operator+=(const double scalar)
{
    const __m256d a0 = simd_load(*this, 0);
    const __m256d a1 = simd_load(*this, 1);
    const __m256d a2 = simd_load(*this, 2);
    const __m256d a3 = simd_load(*this, 3);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(*this, 0, _mm256_add_pd(a0, b));
    simd_store(*this, 1, _mm256_add_pd(a1, b));
    simd_store(*this, 2, _mm256_add_pd(a2, b));
    simd_store(*this, 3, _mm256_add_pd(a3, b));
    return *this;
}

template<>
core_inline
mat4<double> &mat4<double>::operator-=(const double scalar)
{
    const __m256d a0 = simd_load(*this, 0);
    const __m256d a1 = simd_load(*this, 1);
    const __m256d a2 = simd_load(*this, 2);
    const __m256d a3 = simd_load(*this, 3);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(*this, 0, _mm256_sub_pd(a0, b));
    simd_store(*this, 1, _mm256_sub_pd(a1, b));
    simd_store(*this, 2, _mm256_sub_pd(a2, b));
    simd_store(*this, 3, _mm256_sub_pd(a3, b));
    return *this;
}

template<>
core_inline
mat4<double> &mat4<double>::operator*=(const double scalar)
{
    const __m256d a0 = simd_load(*this, 0);
    const __m256d a1 = simd_load(*this, 1);
    const __m256d a2 = simd_load(*this, 2);
    const __m256d a3 = simd_load(*this, 3);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(*this, 0, _mm256_mul_pd(a0, b));
    simd_store(*this, 1, _mm256_mul_pd(a1, b));
    simd_store(*this, 2, _mm256_mul_pd(a2, b));
    simd_store(*this, 3, _mm256_mul_pd(a3, b));
    return *this;
}

template<>
core_inline
mat4<double> &mat4<double>::operator/=(const double scalar)
{
    const __m256d a0 = simd_load(*this, 0);
    const __m256d a1 = simd_load(*this, 1);
    const __m256d a2 = simd_load(*this, 2);
    const __m256d a3 = simd_load(*this, 3);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(*this, 0, _mm256_div_pd(a0, b));
    simd_store(*this, 1, _mm256_div_pd(a1, b));
    simd_store(*this, 2, _mm256_div_pd(a2, b));
    simd_store(*this, 3, _mm256_div_pd(a3, b));
    return *this;
}

} /* math */
} /* atto */

#endif /* ATTO_MATH_GEOMETRY_MAT4_SIMD_H_ */
