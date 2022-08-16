/*
 * mat4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_MAT4_H_
#define ATTO_MATH_GEOMETRY_MAT4_H_

namespace atto {
namespace math {

/** ---------------------------------------------------------------------------
 * mat4
 * @brief A templated 4x4-dimensional matrix type.
 */
template<typename Type>
struct mat4 {
    /* Types */
    typedef std::size_t     size_type;
    typedef Type            value_type;
    typedef Type&           reference;
    typedef const Type&     const_reference;

    /* Special matrices */
    static const mat4 eye;
    static const mat4 ones;
    static const mat4 zeros;

    /* Member variables/accessors. */
    static const size_type m_dim = 4;
    static const size_type m_length = m_dim * m_dim;

    union {
        value_type m_data[m_dim][m_dim] core_aligned(32);
        struct { value_type s0, s1, s2, s3,
                            s4, s5, s6, s7,
                            s8, s9, s10, s11,
                            s12, s13, s14, s15; };
        struct { value_type xx, xy, xz, xw,
                            yx, yy, yz, yw,
                            zx, zy, zz, zw,
                            wx, wy, wz, ww; };
    };

    size_type dim(void) const { return m_dim; }
    size_type length(void) const { return m_length; }
    size_type size(void) const { return sizeof(m_data); }

    /* Element access. */
    value_type *data(void) { return &m_data[0][0]; }
    const value_type *data(void) const { return &m_data[0][0]; }

    value_type &operator()(size_t i, size_t j) { return m_data[i][j]; }
    const value_type &operator()(size_t i, size_t j) const {
        return m_data[i][j];
    }

    /* Unary arithmetic matrix operators. */
    mat4 &operator+=(const mat4 &other);
    mat4 &operator-=(const mat4 &other);
    mat4 &operator*=(const mat4 &other);
    mat4 &operator/=(const mat4 &other);

    /* Unary arithmetic scalar operators. */
    mat4 &operator+=(const Type scalar);
    mat4 &operator-=(const Type scalar);
    mat4 &operator*=(const Type scalar);
    mat4 &operator/=(const Type scalar);

    /* Unary plus/negation operators. */
    mat4 operator+(void) const;
    mat4 operator-(void) const;

    /* Increment operators. */
    mat4 &operator++(void);
    mat4 &operator--(void);
    mat4 operator++(int);
    mat4 operator--(int);

    /* Constructor/destructor. */
    mat4() = default;
    explicit mat4(const value_type *data) {
        std::memcpy(m_data, data, sizeof(m_data));
    }
    explicit mat4(const Type e0) {
        m_data[0][0] = e0;
        m_data[0][1] = e0;
        m_data[0][2] = e0;
        m_data[0][3] = e0;

        m_data[1][0] = e0;
        m_data[1][1] = e0;
        m_data[1][2] = e0;
        m_data[1][3] = e0;

        m_data[2][0] = e0;
        m_data[2][1] = e0;
        m_data[2][2] = e0;
        m_data[2][3] = e0;

        m_data[3][0] = e0;
        m_data[3][1] = e0;
        m_data[3][2] = e0;
        m_data[3][3] = e0;
    }
    explicit mat4(
        const Type e0,
        const Type e1,
        const Type e2,
        const Type e3,
        const Type e4,
        const Type e5,
        const Type e6,
        const Type e7,
        const Type e8,
        const Type e9,
        const Type e10,
        const Type e11,
        const Type e12,
        const Type e13,
        const Type e14,
        const Type e15) {
        m_data[0][0] = e0;
        m_data[0][1] = e1;
        m_data[0][2] = e2;
        m_data[0][3] = e3;

        m_data[1][0] = e4;
        m_data[1][1] = e5;
        m_data[1][2] = e6;
        m_data[1][3] = e7;

        m_data[2][0] = e8;
        m_data[2][1] = e9;
        m_data[2][2] = e10;
        m_data[2][3] = e11;

        m_data[3][0] = e12;
        m_data[3][1] = e13;
        m_data[3][2] = e14;
        m_data[3][3] = e15;
    }
    ~mat4() = default;
}; /* mat4 */


/** ---------------------------------------------------------------------------
 * @brief Special matrices.
 */
template<typename Type>
const mat4<Type> mat4<Type>::eye(
    (Type) 1, (Type) 0, (Type) 0, (Type) 0,
    (Type) 0, (Type) 1, (Type) 0, (Type) 0,
    (Type) 0, (Type) 0, (Type) 1, (Type) 0,
    (Type) 0, (Type) 0, (Type) 0, (Type) 1);

template<typename Type>
const mat4<Type> mat4<Type>::ones(
    (Type) 1, (Type) 1, (Type) 1, (Type) 1,
    (Type) 1, (Type) 1, (Type) 1, (Type) 1,
    (Type) 1, (Type) 1, (Type) 1, (Type) 1,
    (Type) 1, (Type) 1, (Type) 1, (Type) 1);

template<typename Type>
const mat4<Type> mat4<Type>::zeros(
    (Type) 0, (Type) 0, (Type) 0, (Type) 0,
    (Type) 0, (Type) 0, (Type) 0, (Type) 0,
    (Type) 0, (Type) 0, (Type) 0, (Type) 0,
    (Type) 0, (Type) 0, (Type) 0, (Type) 0);


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic matrix operators.
 */
template<typename Type>
core_inline
mat4<Type> &mat4<Type>::operator+=(const mat4<Type> &other)
{
    m_data[0][0] += other.m_data[0][0];
    m_data[0][1] += other.m_data[0][1];
    m_data[0][2] += other.m_data[0][2];
    m_data[0][3] += other.m_data[0][3];

    m_data[1][0] += other.m_data[1][0];
    m_data[1][1] += other.m_data[1][1];
    m_data[1][2] += other.m_data[1][2];
    m_data[1][3] += other.m_data[1][3];

    m_data[2][0] += other.m_data[2][0];
    m_data[2][1] += other.m_data[2][1];
    m_data[2][2] += other.m_data[2][2];
    m_data[2][3] += other.m_data[2][3];

    m_data[3][0] += other.m_data[3][0];
    m_data[3][1] += other.m_data[3][1];
    m_data[3][2] += other.m_data[3][2];
    m_data[3][3] += other.m_data[3][3];
    return *this;
}

template<typename Type>
core_inline
mat4<Type> &mat4<Type>::operator-=(const mat4<Type> &other)
{
    m_data[0][0] -= other.m_data[0][0];
    m_data[0][1] -= other.m_data[0][1];
    m_data[0][2] -= other.m_data[0][2];
    m_data[0][3] -= other.m_data[0][3];

    m_data[1][0] -= other.m_data[1][0];
    m_data[1][1] -= other.m_data[1][1];
    m_data[1][2] -= other.m_data[1][2];
    m_data[1][3] -= other.m_data[1][3];

    m_data[2][0] -= other.m_data[2][0];
    m_data[2][1] -= other.m_data[2][1];
    m_data[2][2] -= other.m_data[2][2];
    m_data[2][3] -= other.m_data[2][3];

    m_data[3][0] -= other.m_data[3][0];
    m_data[3][1] -= other.m_data[3][1];
    m_data[3][2] -= other.m_data[3][2];
    m_data[3][3] -= other.m_data[3][3];
    return *this;
}

template<typename Type>
core_inline
mat4<Type> &mat4<Type>::operator*=(const mat4<Type> &other)
{
    *this = dot(*this, other);
    return *this;
}

template<typename Type>
core_inline
mat4<Type> &mat4<Type>::operator/=(const mat4<Type> &other)
{
    *this = dot(*this, inverse(other));
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<typename Type>
core_inline
mat4<Type> &mat4<Type>::operator+=(const Type scalar)
{
    m_data[0][0] += scalar;
    m_data[0][1] += scalar;
    m_data[0][2] += scalar;
    m_data[0][3] += scalar;

    m_data[1][0] += scalar;
    m_data[1][1] += scalar;
    m_data[1][2] += scalar;
    m_data[1][3] += scalar;

    m_data[2][0] += scalar;
    m_data[2][1] += scalar;
    m_data[2][2] += scalar;
    m_data[2][3] += scalar;

    m_data[3][0] += scalar;
    m_data[3][1] += scalar;
    m_data[3][2] += scalar;
    m_data[3][3] += scalar;
    return *this;
}

template<typename Type>
core_inline
mat4<Type> &mat4<Type>::operator-=(const Type scalar)
{
    m_data[0][0] -= scalar;
    m_data[0][1] -= scalar;
    m_data[0][2] -= scalar;
    m_data[0][3] -= scalar;

    m_data[1][0] -= scalar;
    m_data[1][1] -= scalar;
    m_data[1][2] -= scalar;
    m_data[1][3] -= scalar;

    m_data[2][0] -= scalar;
    m_data[2][1] -= scalar;
    m_data[2][2] -= scalar;
    m_data[2][3] -= scalar;

    m_data[3][0] -= scalar;
    m_data[3][1] -= scalar;
    m_data[3][2] -= scalar;
    m_data[3][3] -= scalar;
    return *this;
}

template<typename Type>
core_inline
mat4<Type> &mat4<Type>::operator*=(const Type scalar)
{
    m_data[0][0] *= scalar;
    m_data[0][1] *= scalar;
    m_data[0][2] *= scalar;
    m_data[0][3] *= scalar;

    m_data[1][0] *= scalar;
    m_data[1][1] *= scalar;
    m_data[1][2] *= scalar;
    m_data[1][3] *= scalar;

    m_data[2][0] *= scalar;
    m_data[2][1] *= scalar;
    m_data[2][2] *= scalar;
    m_data[2][3] *= scalar;

    m_data[3][0] *= scalar;
    m_data[3][1] *= scalar;
    m_data[3][2] *= scalar;
    m_data[3][3] *= scalar;
    return *this;
}

template<typename Type>
core_inline
mat4<Type> &mat4<Type>::operator/=(const Type scalar)
{
    m_data[0][0] /= scalar;
    m_data[0][1] /= scalar;
    m_data[0][2] /= scalar;
    m_data[0][3] /= scalar;

    m_data[1][0] /= scalar;
    m_data[1][1] /= scalar;
    m_data[1][2] /= scalar;
    m_data[1][3] /= scalar;

    m_data[2][0] /= scalar;
    m_data[2][1] /= scalar;
    m_data[2][2] /= scalar;
    m_data[2][3] /= scalar;

    m_data[3][0] /= scalar;
    m_data[3][1] /= scalar;
    m_data[3][2] /= scalar;
    m_data[3][3] /= scalar;
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary plus/negation operators.
 */
template<typename Type>
core_inline
mat4<Type> mat4<Type>::operator+(void) const
{
    mat4<Type> result(*this);
    return result;
}

template<typename Type>
core_inline
mat4<Type> mat4<Type>::operator-(void) const
{
    mat4<Type> result(*this);
    result *= (Type) (-1);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Increment operators.
 */
template<typename Type>
core_inline
mat4<Type> &mat4<Type>::operator++(void)
{
    static constexpr Type one = (Type) 1;
    (*this) += one;
    return *this;
}

template<typename Type>
core_inline
mat4<Type> &mat4<Type>::operator--(void)
{
    static constexpr Type one = (Type) 1;
    (*this) -= one;
    return *this;
}

template<typename Type>
core_inline
mat4<Type> mat4<Type>::operator++(int)
{
    mat4<Type> result(*this);
    ++(*this);
    return result;
}

template<typename Type>
core_inline
mat4<Type> mat4<Type>::operator--(int)
{
    mat4<Type> result(*this);
    --(*this);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between two vectors.
 */
template<typename Type>
core_inline
mat4<Type> operator+(mat4<Type> lhs, const mat4<Type> &rhs)
{
    lhs += rhs;
    return lhs;
}

template<typename Type>
core_inline
mat4<Type> operator-(mat4<Type> lhs, const mat4<Type> &rhs)
{
    lhs -= rhs;
    return lhs;
}

template<typename Type>
core_inline
mat4<Type> operator*(mat4<Type> lhs, const mat4<Type> &rhs)
{
    lhs *= rhs;
    return lhs;
}

template<typename Type>
core_inline
mat4<Type> operator/(mat4<Type> lhs, const mat4<Type> &rhs)
{
    lhs /= rhs;
    return lhs;
}

template<typename Type>
core_inline
vec4<Type> operator*(mat4<Type> lhs, const vec4<Type> &rhs)
{
    return dot(lhs, rhs);
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
template<typename Type>
core_inline
mat4<Type> operator+(mat4<Type> lhs, const Type scalar)
{
    lhs += scalar;
    return lhs;
}

template<typename Type>
core_inline
mat4<Type> operator-(mat4<Type> lhs, const Type scalar)
{
    lhs -= scalar;
    return lhs;
}

template<typename Type>
core_inline
mat4<Type> operator*(mat4<Type> lhs, const Type scalar)
{
    lhs *= scalar;
    return lhs;
}

template<typename Type>
core_inline
mat4<Type> operator/(mat4<Type> lhs, const Type scalar)
{
    lhs /= scalar;
    return lhs;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between a vector and a scalar.
 * Division is not commutative, so its not implemented.
 */
template<typename Type>
core_inline
mat4<Type> operator+(const Type scalar, mat4<Type> rhs)
{
    rhs += scalar;
    return rhs;
}

template<typename Type>
core_inline
mat4<Type> operator-(const Type scalar, mat4<Type> rhs)
{
    rhs -= scalar;
    return rhs;
}

template<typename Type>
core_inline
mat4<Type> operator*(const Type scalar, mat4<Type> rhs)
{
    rhs *= scalar;
    return rhs;
}

} /* math */
} /* atto */


/** ---------------------------------------------------------------------------
 * @brief Enable simd vectorized instructions.
 */
#ifdef ATTO_MATH_SIMD
#include "atto/math/geometry/mat4-simd.hpp"
#endif

#endif /* ATTO_MATH_GEOMETRY_MAT4_H_ */
