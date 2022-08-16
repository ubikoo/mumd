/*
 * vec4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_VEC4_H_
#define ATTO_MATH_GEOMETRY_VEC4_H_

namespace atto {
namespace math {

/** ---------------------------------------------------------------------------
 * vec4
 * @brief A templated 4-dimensional vector type.
 */
template<typename Type> struct vec2;
template<typename Type> struct vec3;
template<typename Type> struct vec4;

template<typename Type>
struct vec4 {
    /* Types */
    typedef std::size_t size_type;
    typedef Type        value_type;
    typedef Type&       reference;
    typedef const Type& const_reference;

    /* Member variables/accessors. */
    static const size_type m_length = 4;

    union {
        value_type m_data[m_length] core_aligned(32);
        struct { value_type s0, s1, s2, s3; };
        struct { value_type x, y, z, w; };
    };

    size_type length(void) const { return m_length; }
    size_type size(void) const { return sizeof(m_data); }

    /* Element access. */
    value_type *data(void) { return m_data; }
    const value_type *data(void) const { return m_data; }

    reference operator()(size_type i) { return m_data[i]; }
    const_reference operator()(size_type i) const { return m_data[i]; }

    /* Unary arithmetic vector operators. */
    vec4 &operator+=(const vec4 &other);
    vec4 &operator-=(const vec4 &other);
    vec4 &operator*=(const vec4 &other);
    vec4 &operator/=(const vec4 &other);

    /* Unary plus/negation operators. */
    vec4 operator+(void) const;
    vec4 operator-(void) const;

    /* Unary arithmetic scalar operators. */
    vec4 &operator+=(const value_type scalar);
    vec4 &operator-=(const value_type scalar);
    vec4 &operator*=(const value_type scalar);
    vec4 &operator/=(const value_type scalar);

    /* Increment operators. */
    vec4 &operator++(void);
    vec4 &operator--(void);
    vec4 operator++(int);
    vec4 operator--(int);

    /* Constructor/destructor. */
    vec4() = default;
    explicit vec4(const value_type *data) {
        std::memcpy(m_data, data, sizeof(m_data));
    }
    explicit vec4(const value_type e0) {
        m_data[0] = e0;
        m_data[1] = e0;
        m_data[2] = e0;
        m_data[3] = e0;
    }
    explicit vec4(
        const value_type e0,
        const value_type e1,
        const value_type e2,
        const value_type e3) {
        m_data[0] = e0;
        m_data[1] = e1;
        m_data[2] = e2;
        m_data[3] = e3;
    }
    explicit vec4(
        const vec2<value_type> &v,
        const value_type e2,
        const value_type e3) {
        m_data[0] = v(0);
        m_data[1] = v(1);
        m_data[2] = e2;
        m_data[3] = e3;
    }
    explicit vec4(const vec3<value_type> &v, const value_type e3) {
        m_data[0] = v(0);
        m_data[1] = v(1);
        m_data[2] = v(2);
        m_data[3] = e3;
    }
    ~vec4() = default;
}; /* vec4 */


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
template<typename Type>
core_inline
vec4<Type> &vec4<Type>::operator+=(const vec4<Type> &other)
{
    m_data[0] += other.m_data[0];
    m_data[1] += other.m_data[1];
    m_data[2] += other.m_data[2];
    m_data[3] += other.m_data[3];
    return *this;
}

template<typename Type>
core_inline
vec4<Type> &vec4<Type>::operator-=(const vec4<Type> &other)
{
    m_data[0] -= other.m_data[0];
    m_data[1] -= other.m_data[1];
    m_data[2] -= other.m_data[2];
    m_data[3] -= other.m_data[3];
    return *this;
}

template<typename Type>
core_inline
vec4<Type> &vec4<Type>::operator*=(const vec4<Type> &other)
{
    m_data[0] *= other.m_data[0];
    m_data[1] *= other.m_data[1];
    m_data[2] *= other.m_data[2];
    m_data[3] *= other.m_data[3];
    return *this;
}

template<typename Type>
core_inline
vec4<Type> &vec4<Type>::operator/=(const vec4<Type> &other)
{
    m_data[0] /= other.m_data[0];
    m_data[1] /= other.m_data[1];
    m_data[2] /= other.m_data[2];
    m_data[3] /= other.m_data[3];
    return *this;
}

/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<typename Type>
core_inline
vec4<Type> &vec4<Type>::operator+=(const Type scalar)
{
    m_data[0] += scalar;
    m_data[1] += scalar;
    m_data[2] += scalar;
    m_data[3] += scalar;
    return *this;
}

template<typename Type>
core_inline
vec4<Type> &vec4<Type>::operator-=(const Type scalar)
{
    m_data[0] -= scalar;
    m_data[1] -= scalar;
    m_data[2] -= scalar;
    m_data[3] -= scalar;
    return *this;
}

template<typename Type>
core_inline
vec4<Type> &vec4<Type>::operator*=(const Type scalar)
{
    m_data[0] *= scalar;
    m_data[1] *= scalar;
    m_data[2] *= scalar;
    m_data[3] *= scalar;
    return *this;
}

template<typename Type>
core_inline
vec4<Type> &vec4<Type>::operator/=(const Type scalar)
{
    m_data[0] /= scalar;
    m_data[1] /= scalar;
    m_data[2] /= scalar;
    m_data[3] /= scalar;
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary plus/negation operators.
 */
template<typename Type>
core_inline
vec4<Type> vec4<Type>::operator+(void) const
{
    vec4<Type> result(*this);
    return result;
}

template<typename Type>
core_inline
vec4<Type> vec4<Type>::operator-(void) const
{
    vec4<Type> result(*this);
    result *= (Type) (-1);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Increment operators.
 */
template<typename Type>
core_inline
vec4<Type> &vec4<Type>::operator++(void)
{
    static constexpr Type one = (Type) 1;
    (*this) += one;
    return *this;
}

template<typename Type>
core_inline
vec4<Type> &vec4<Type>::operator--(void)
{
    static constexpr Type one = (Type) 1;
    (*this) -= one;
    return *this;
}


template<typename Type>
core_inline
vec4<Type> vec4<Type>::operator++(int)
{
    vec4<Type> result(*this);
    ++(*this);
    return result;
}

template<typename Type>
core_inline
vec4<Type> vec4<Type>::operator--(int)
{
    vec4<Type> result(*this);
    --(*this);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between two vectors.
 */
template<typename Type>
core_inline
vec4<Type> operator+(vec4<Type> lhs, const vec4<Type> &rhs)
{
    lhs += rhs;
    return lhs;
}

template<typename Type>
core_inline
vec4<Type> operator-(vec4<Type> lhs, const vec4<Type> &rhs)
{
    lhs -= rhs;
    return lhs;
}

template<typename Type>
core_inline
vec4<Type> operator*(vec4<Type> lhs, const vec4<Type> &rhs)
{
    lhs *= rhs;
    return lhs;
}

template<typename Type>
core_inline
vec4<Type> operator/(vec4<Type> lhs, const vec4<Type> &rhs)
{
    lhs /= rhs;
    return lhs;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
template<typename Type>
core_inline
vec4<Type> operator+(vec4<Type> lhs, const Type scalar)
{
    lhs += scalar;
    return lhs;
}

template<typename Type>
core_inline
vec4<Type> operator-(vec4<Type> lhs, const Type scalar)
{
    lhs -= scalar;
    return lhs;
}

template<typename Type>
core_inline
vec4<Type> operator*(vec4<Type> lhs, const Type scalar)
{
    lhs *= scalar;
    return lhs;
}

template<typename Type>
core_inline
vec4<Type> operator/(vec4<Type> lhs, const Type scalar)
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
vec4<Type> operator+(const Type scalar, vec4<Type> rhs)
{
    rhs += scalar;
    return rhs;
}

template<typename Type>
core_inline
vec4<Type> operator-(const Type scalar, vec4<Type> rhs)
{
    rhs -= scalar;
    return rhs;
}

template<typename Type>
core_inline
vec4<Type> operator*(const Type scalar, vec4<Type> rhs)
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
#include "atto/math/geometry/vec4-simd.hpp"
#endif

#endif /* ATTO_MATH_GEOMETRY_VEC4_H_ */