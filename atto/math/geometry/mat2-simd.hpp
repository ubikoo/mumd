/*
 * mat2-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_MAT2_SIMD_H_
#define ATTO_MATH_GEOMETRY_MAT2_SIMD_H_

#include "atto/math/geometry/simd.hpp"

namespace atto {
namespace math {

/** ---- simd load/store functions --------------------------------------------
 * simd_load
 * @brief Load 128-bits (2 packed double-precision 64-bit)
 * from the specified row in the 2d-matrix (16-byte aligned).
 *
 * @fn _mm256_extractf128_pd(__m256d a, const int mask)
 * CASE mask[7:0] of
 *  0: dst[127:0] := a[127:0]
 *  1: dst[127:0] := a[255:128]
 * ESAC
 */
core_inline
__m128d simd_load(const mat2<double> &mat, const size_t row)
{
    return _mm_load_pd(mat.data() + row*mat.dim());
}

/**
 * simd_store
 * @brief Store 128-bits (2 packed double-precision 64-bit)
 * into the specified row in the 2d-matrix (16-byte aligned).
 *
 * @fn _mm256_insertf128_pd(__m256d a, __m128d b, int mask)
 * dst[255:0] := a[255:0]
 * CASE mask[7:0] of
 *  0: dst[127:0]   := b[127:0]
 *  1: dst[255:128] := b[127:0]
 * ESAC
 */
core_inline
void simd_store(mat2<double> &mat, const size_t row, const __m128d a)
{
    _mm_store_pd(mat.data() + row*mat.dim(), a);
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic matrix operators.
 */
template<>
core_inline
mat2<double> &mat2<double>::operator+=(const mat2<double> &other)
{
    const __m128d a0 = simd_load(*this, 0);
    const __m128d a1 = simd_load(*this, 1);

    const __m128d b0 = simd_load(other, 0);
    const __m128d b1 = simd_load(other, 1);

    simd_store(*this, 0, _mm_add_pd(a0, b0));
    simd_store(*this, 1, _mm_add_pd(a1, b1));
    return *this;
}

template<>
core_inline
mat2<double> &mat2<double>::operator-=(const mat2<double> &other)
{
    const __m128d a0 = simd_load(*this, 0);
    const __m128d a1 = simd_load(*this, 1);

    const __m128d b0 = simd_load(other, 0);
    const __m128d b1 = simd_load(other, 1);

    simd_store(*this, 0, _mm_sub_pd(a0, b0));
    simd_store(*this, 1, _mm_sub_pd(a1, b1));
    return *this;
}

template<>
core_inline
mat2<double> &mat2<double>::operator*=(const mat2<double> &other)
{
    const __m128d a0 = simd_load(*this, 0);
    const __m128d a1 = simd_load(*this, 1);

    const __m128d b0 = simd_load(other, 0);
    const __m128d b1 = simd_load(other, 1);

    simd_store(*this, 0, _mm_mul_pd(a0, b0));
    simd_store(*this, 1, _mm_mul_pd(a1, b1));
    return *this;
}

template<>
core_inline
mat2<double> &mat2<double>::operator/=(const mat2<double> &other)
{
    const __m128d a0 = simd_load(*this, 0);
    const __m128d a1 = simd_load(*this, 1);

    const __m128d b0 = simd_load(other, 0);
    const __m128d b1 = simd_load(other, 1);

    simd_store(*this, 0, _mm_div_pd(a0, b0));
    simd_store(*this, 1, _mm_div_pd(a1, b1));
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<>
core_inline
mat2<double> &mat2<double>::operator+=(const double scalar)
{
    const __m128d a0 = simd_load(*this, 0);
    const __m128d a1 = simd_load(*this, 1);
    const __m128d b = _mm_set1_pd(scalar);

    simd_store(*this, 0, _mm_add_pd(a0, b));
    simd_store(*this, 1, _mm_add_pd(a1, b));
    return *this;
}

template<>
core_inline
mat2<double> &mat2<double>::operator-=(const double scalar)
{
    const __m128d a0 = simd_load(*this, 0);
    const __m128d a1 = simd_load(*this, 1);
    const __m128d b = _mm_set1_pd(scalar);

    simd_store(*this, 0, _mm_sub_pd(a0, b));
    simd_store(*this, 1, _mm_sub_pd(a1, b));
    return *this;
}

template<>
core_inline
mat2<double> &mat2<double>::operator*=(const double scalar)
{
    const __m128d a0 = simd_load(*this, 0);
    const __m128d a1 = simd_load(*this, 1);
    const __m128d b = _mm_set1_pd(scalar);

    simd_store(*this, 0, _mm_mul_pd(a0, b));
    simd_store(*this, 1, _mm_mul_pd(a1, b));
    return *this;
}

template<>
core_inline
mat2<double> &mat2<double>::operator/=(const double scalar)
{
    const __m128d a0 = simd_load(*this, 0);
    const __m128d a1 = simd_load(*this, 1);
    const __m128d b = _mm_set1_pd(scalar);

    simd_store(*this, 0, _mm_div_pd(a0, b));
    simd_store(*this, 1, _mm_div_pd(a1, b));
    return *this;
}

} /* math */
} /* atto */

#endif /* ATTO_MATH_GEOMETRY_MAT2_SIMD_H_ */
