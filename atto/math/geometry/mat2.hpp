/*
 * mat2.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_MAT2_H_
#define ATTO_MATH_GEOMETRY_MAT2_H_

namespace atto {
namespace math {

/** ---------------------------------------------------------------------------
 * mat2
 * @brief A templated 2x2-dimensional matrix type.
 */
template<typename Type>
struct mat2 {
    /* Types */
    typedef std::size_t     size_type;
    typedef Type            value_type;
    typedef Type&           reference;
    typedef const Type&     const_reference;

    /* Special matrices */
    static const mat2 eye;
    static const mat2 ones;
    static const mat2 zeros;

    /* Member variables/accessors. */
    static const size_type m_dim = 2;
    static const size_type m_length = m_dim * m_dim;

    union {
        value_type m_data[m_dim][m_dim] core_aligned(32);
        struct { value_type s0, s1,
                            s2, s3; };
        struct { value_type xx, xy,
                            yx, yy; };
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
    mat2 &operator+=(const mat2 &other);
    mat2 &operator-=(const mat2 &other);
    mat2 &operator*=(const mat2 &other);
    mat2 &operator/=(const mat2 &other);

    /* Unary arithmetic scalar operators. */
    mat2 &operator+=(const Type scalar);
    mat2 &operator-=(const Type scalar);
    mat2 &operator*=(const Type scalar);
    mat2 &operator/=(const Type scalar);

    /* Unary plus/negation operators. */
    mat2 operator+(void) const;
    mat2 operator-(void) const;

    /* Increment operators. */
    mat2 &operator++(void);
    mat2 &operator--(void);
    mat2 operator++(int);
    mat2 operator--(int);

    /* Constructor/destructor. */
    mat2() = default;
    explicit mat2(const value_type *data) {
        std::memcpy(m_data, data, sizeof(m_data));
    }
    explicit mat2(const Type e0) {
        m_data[0][0] = e0;
        m_data[0][1] = e0;

        m_data[1][0] = e0;
        m_data[1][1] = e0;
    }
    explicit mat2(
        const Type e0,
        const Type e1,
        const Type e2,
        const Type e3) {
        m_data[0][0] = e0;
        m_data[0][1] = e1;

        m_data[1][0] = e2;
        m_data[1][1] = e3;
    }
    ~mat2() = default;
}; /* mat2 */


/** ---------------------------------------------------------------------------
 * @brief Special matrices.
 */
template<typename Type>
const mat2<Type> mat2<Type>::eye(
    (Type) 1, (Type) 0,
    (Type) 0, (Type) 1);

template<typename Type>
const mat2<Type> mat2<Type>::ones(
    (Type) 1, (Type) 1,
    (Type) 1, (Type) 1);

template<typename Type>
const mat2<Type> mat2<Type>::zeros(
    (Type) 0, (Type) 0,
    (Type) 0, (Type) 0);


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic matrix operators.
 */
template<typename Type>
core_inline
mat2<Type> &mat2<Type>::operator+=(const mat2<Type> &other)
{
    m_data[0][0] += other.m_data[0][0];
    m_data[0][1] += other.m_data[0][1];

    m_data[1][0] += other.m_data[1][0];
    m_data[1][1] += other.m_data[1][1];
    return *this;
}

template<typename Type>
core_inline
mat2<Type> &mat2<Type>::operator-=(const mat2<Type> &other)
{
    m_data[0][0] -= other.m_data[0][0];
    m_data[0][1] -= other.m_data[0][1];

    m_data[1][0] -= other.m_data[1][0];
    m_data[1][1] -= other.m_data[1][1];
    return *this;
}

template<typename Type>
core_inline
mat2<Type> &mat2<Type>::operator*=(const mat2<Type> &other)
{
    *this = dot(*this, other);
    return *this;
}

template<typename Type>
core_inline
mat2<Type> &mat2<Type>::operator/=(const mat2<Type> &other)
{
    *this = dot(*this, inverse(other));
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<typename Type>
core_inline
mat2<Type> &mat2<Type>::operator+=(const Type scalar)
{
    m_data[0][0] += scalar;
    m_data[0][1] += scalar;

    m_data[1][0] += scalar;
    m_data[1][1] += scalar;
    return *this;
}

template<typename Type>
core_inline
mat2<Type> &mat2<Type>::operator-=(const Type scalar)
{
    m_data[0][0] -= scalar;
    m_data[0][1] -= scalar;

    m_data[1][0] -= scalar;
    m_data[1][1] -= scalar;
    return *this;
}

template<typename Type>
core_inline
mat2<Type> &mat2<Type>::operator*=(const Type scalar)
{
    m_data[0][0] *= scalar;
    m_data[0][1] *= scalar;

    m_data[1][0] *= scalar;
    m_data[1][1] *= scalar;
    return *this;
}

template<typename Type>
core_inline
mat2<Type> &mat2<Type>::operator/=(const Type scalar)
{
    m_data[0][0] /= scalar;
    m_data[0][1] /= scalar;

    m_data[1][0] /= scalar;
    m_data[1][1] /= scalar;
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary plus/negation operators.
 */
template<typename Type>
core_inline
mat2<Type> mat2<Type>::operator+(void) const
{
    mat2<Type> result(*this);
    return result;
}

template<typename Type>
core_inline
mat2<Type> mat2<Type>::operator-(void) const
{
    mat2<Type> result(*this);
    result *= (Type) (-1);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Increment operators.
 */
template<typename Type>
core_inline
mat2<Type> &mat2<Type>::operator++(void)
{
    static constexpr Type one = (Type) 1;
    (*this) += one;
    return *this;
}

template<typename Type>
core_inline
mat2<Type> &mat2<Type>::operator--(void)
{
    static constexpr Type one = (Type) 1;
    (*this) -= one;
    return *this;
}

template<typename Type>
core_inline
mat2<Type> mat2<Type>::operator++(int)
{
    mat2<Type> result(*this);
    ++(*this);
    return result;
}

template<typename Type>
core_inline
mat2<Type> mat2<Type>::operator--(int)
{
    mat2<Type> result(*this);
    --(*this);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between two vectors.
 */
template<typename Type>
core_inline
mat2<Type> operator+(mat2<Type> lhs, const mat2<Type> &rhs)
{
    lhs += rhs;
    return lhs;
}

template<typename Type>
core_inline
mat2<Type> operator-(mat2<Type> lhs, const mat2<Type> &rhs)
{
    lhs -= rhs;
    return lhs;
}

template<typename Type>
core_inline
mat2<Type> operator*(mat2<Type> lhs, const mat2<Type> &rhs)
{
    lhs *= rhs;
    return lhs;
}

template<typename Type>
core_inline
mat2<Type> operator/(mat2<Type> lhs, const mat2<Type> &rhs)
{
    lhs /= rhs;
    return lhs;
}

template<typename Type>
core_inline
vec2<Type> operator*(mat2<Type> lhs, const vec2<Type> &rhs)
{
    return dot(lhs, rhs);
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
template<typename Type>
core_inline
mat2<Type> operator+(mat2<Type> lhs, const Type scalar)
{
    lhs += scalar;
    return lhs;
}

template<typename Type>
core_inline
mat2<Type> operator-(mat2<Type> lhs, const Type scalar)
{
    lhs -= scalar;
    return lhs;
}

template<typename Type>
core_inline
mat2<Type> operator*(mat2<Type> lhs, const Type scalar)
{
    lhs *= scalar;
    return lhs;
}

template<typename Type>
core_inline
mat2<Type> operator/(mat2<Type> lhs, const Type scalar)
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
mat2<Type> operator+(const Type scalar, mat2<Type> rhs)
{
    rhs += scalar;
    return rhs;
}

template<typename Type>
core_inline
mat2<Type> operator-(const Type scalar, mat2<Type> rhs)
{
    rhs -= scalar;
    return rhs;
}

template<typename Type>
core_inline
mat2<Type> operator*(const Type scalar, mat2<Type> rhs)
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
#include "atto/math/geometry/mat2-simd.hpp"
#endif

#endif /* ATTO_MATH_GEOMETRY_MAT2_H_ */
