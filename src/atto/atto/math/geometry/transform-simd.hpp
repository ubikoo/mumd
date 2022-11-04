/*
 * transform-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_TRANSFORM_SIMD_H_
#define ATTO_MATH_GEOMETRY_TRANSFORM_SIMD_H_

#include "atto/math/geometry/simd.hpp"

namespace atto {
namespace math {

/** ---------------------------------------------------------------------------
 * rotate
 * @brief Rotate this matrix by theta given an axis of rotation defined by n.
 */
template<>
core_inline
mat4<double> rotate(vec3<double> n, const double theta)
{
    /* Identity matrix */
    const __m256d one0 = _mm256_set_pd(0.0, 0.0, 0.0, 1.0);
    const __m256d one1 = _mm256_set_pd(0.0, 0.0, 1.0, 0.0);
    const __m256d one2 = _mm256_set_pd(0.0, 1.0, 0.0, 0.0);
    const __m256d one3 = _mm256_set_pd(1.0, 0.0, 0.0, 0.0);

    /* Compute trigonometric coefficients */
    __m256d alpha = _mm256_set1_pd(1.0 - std::cos(theta));
    __m256d beta  = _mm256_set1_pd(std::cos(theta));
    __m256d gamma = _mm256_set1_pd(std::sin(theta));

    /* Compute diadic product matrix */
    n = normalize(n);
    __m256d nn  = simd_load(n);
    __m256d nn0 = _mm256_set1_pd(n(0));
    __m256d nn1 = _mm256_set1_pd(n(1));
    __m256d nn2 = _mm256_set1_pd(n(2));
    __m256d nn3 = one3;

    nn0 = _mm256_mul_pd(alpha, _mm256_mul_pd(nn0, nn));
    nn1 = _mm256_mul_pd(alpha, _mm256_mul_pd(nn1, nn));
    nn2 = _mm256_mul_pd(alpha, _mm256_mul_pd(nn2, nn));
    nn3 = _mm256_mul_pd(alpha, nn3);

    /* Compute identity matrix */
    __m256d id0 = _mm256_mul_pd(beta, one0);
    __m256d id1 = _mm256_mul_pd(beta, one1);
    __m256d id2 = _mm256_mul_pd(beta, one2);
    __m256d id3 = _mm256_mul_pd(beta, one3);

    /* Compute cross product matrix */
    __m256d rc0 = _mm256_set_pd(0.0,  n(1), -n(2),  0.0);
    __m256d rc1 = _mm256_set_pd(0.0, -n(0),   0.0,  n(2));
    __m256d rc2 = _mm256_set_pd(0.0,   0.0,  n(0), -n(1));
    __m256d rc3 = _mm256_set_pd(1.0,   0.0,   0.0,  1.0);

    rc0 = _mm256_mul_pd(gamma, rc0);
    rc1 = _mm256_mul_pd(gamma, rc1);
    rc2 = _mm256_mul_pd(gamma, rc2);
    rc3 = _mm256_mul_pd(gamma, rc3);

    /* Compute rotation matrix */
    __m256d rot0 = _mm256_add_pd(nn0, _mm256_add_pd(id0, rc0));
    __m256d rot1 = _mm256_add_pd(nn1, _mm256_add_pd(id1, rc1));
    __m256d rot2 = _mm256_add_pd(nn2, _mm256_add_pd(id2, rc2));
    __m256d rot3 = _mm256_add_pd(nn3, _mm256_add_pd(id3, rc3));

    mat4<double> result{};
    simd_store(result, 0, rot0);
    simd_store(result, 1, rot1);
    simd_store(result, 2, rot2);
    simd_store(result, 3, rot3);
    return result;
}

} /* math */
} /* atto */

#endif /* ATTO_MATH_GEOMETRY_TRANSFORM_SIMD_H_ */
