/*
 * vec3.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_VEC3_H_
#define ATTO_MATH_GEOMETRY_VEC3_H_

namespace atto {
namespace math {

/** ---------------------------------------------------------------------------
 * vec3
 * @brief A templated 3-dimensional vector type.
 */
template<typename Type> struct vec2;
template<typename Type> struct vec3;
template<typename Type> struct vec4;

template<typename Type>
struct vec3 {
    /* Types */
    typedef std::size_t size_type;
    typedef Type        value_type;
    typedef Type&       reference;
    typedef const Type& const_reference;

    /* Member variables/accessors. */
    static const size_type m_length = 3;

    union {
        value_type m_data[m_length] core_aligned(32);
        struct { value_type s0, s1, s2; };
        struct { value_type x, y, z; };
    };

    size_type length(void) const { return m_length; }
    size_type size(void) const { return sizeof(m_data); }

    /* Element access. */
    value_type *data(void) { return &m_data[0]; }
    const value_type *data(void) const { return &m_data[0]; }

    reference operator()(size_type i) { return m_data[i]; }
    const_reference operator()(size_type i) const { return m_data[i]; }

    /* Unary arithmetic vector operators. */
    vec3 &operator+=(const vec3 &other);
    vec3 &operator-=(const vec3 &other);
    vec3 &operator*=(const vec3 &other);
    vec3 &operator/=(const vec3 &other);

    /* Unary arithmetic scalar operators. */
    vec3 &operator+=(const value_type scalar);
    vec3 &operator-=(const value_type scalar);
    vec3 &operator*=(const value_type scalar);
    vec3 &operator/=(const value_type scalar);

    /* Unary plus/negation operators. */
    vec3 operator+(void) const;
    vec3 operator-(void) const;

    /* Increment operators. */
    vec3 &operator++(void);
    vec3 &operator--(void);
    vec3 operator++(int);
    vec3 operator--(int);

    /* Constructor/destructor. */
    vec3() = default;
    explicit vec3(const value_type *data) {
        std::memcpy(m_data, data, sizeof(m_data));
    }
    explicit vec3(const value_type e0) {
        m_data[0] = e0;
        m_data[1] = e0;
        m_data[2] = e0;
    }
    explicit vec3(
        const value_type e0,
        const value_type e1,
        const value_type e2) {
        m_data[0] = e0;
        m_data[1] = e1;
        m_data[2] = e2;
    }
    explicit vec3(const vec2<value_type> &v, const value_type e2) {
        m_data[0] = v(0);
        m_data[1] = v(1);
        m_data[2] = e2;
    }
    explicit vec3(const vec4<value_type> &v) {
        m_data[0] = v(0);
        m_data[1] = v(1);
        m_data[2] = v(2);
    }
    ~vec3() = default;
}; /* vec3 */


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
template<typename Type>
core_inline
vec3<Type> &vec3<Type>::operator+=(const vec3<Type> &other)
{
    m_data[0] += other.m_data[0];
    m_data[1] += other.m_data[1];
    m_data[2] += other.m_data[2];
    return *this;
}

template<typename Type>
core_inline
vec3<Type> &vec3<Type>::operator-=(const vec3<Type> &other)
{
    m_data[0] -= other.m_data[0];
    m_data[1] -= other.m_data[1];
    m_data[2] -= other.m_data[2];
    return *this;
}

template<typename Type>
core_inline
vec3<Type> &vec3<Type>::operator*=(const vec3<Type> &other)
{
    m_data[0] *= other.m_data[0];
    m_data[1] *= other.m_data[1];
    m_data[2] *= other.m_data[2];
    return *this;
}

template<typename Type>
core_inline
vec3<Type> &vec3<Type>::operator/=(const vec3<Type> &other)
{
    m_data[0] /= other.m_data[0];
    m_data[1] /= other.m_data[1];
    m_data[2] /= other.m_data[2];
    return *this;
}

/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<typename Type>
core_inline
vec3<Type> &vec3<Type>::operator+=(const Type scalar)
{
    m_data[0] += scalar;
    m_data[1] += scalar;
    m_data[2] += scalar;
    return *this;
}

template<typename Type>
core_inline
vec3<Type> &vec3<Type>::operator-=(const Type scalar)
{
    m_data[0] -= scalar;
    m_data[1] -= scalar;
    m_data[2] -= scalar;
    return *this;
}

template<typename Type>
core_inline
vec3<Type> &vec3<Type>::operator*=(const Type scalar)
{
    m_data[0] *= scalar;
    m_data[1] *= scalar;
    m_data[2] *= scalar;
    return *this;
}

template<typename Type>
core_inline
vec3<Type> &vec3<Type>::operator/=(const Type scalar)
{
    m_data[0] /= scalar;
    m_data[1] /= scalar;
    m_data[2] /= scalar;
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary plus/negation operators.
 */
template<typename Type>
core_inline
vec3<Type> vec3<Type>::operator+(void) const
{
    vec3<Type> result(*this);
    return result;
}

template<typename Type>
core_inline
vec3<Type> vec3<Type>::operator-(void) const
{
    vec3<Type> result(*this);
    result *= (Type) (-1);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Increment operators.
 */
template<typename Type>
core_inline
vec3<Type> &vec3<Type>::operator++(void)
{
    static constexpr Type one = (Type) 1;
    (*this) += one;
    return *this;
}

template<typename Type>
core_inline
vec3<Type> &vec3<Type>::operator--(void)
{
    static constexpr Type one = (Type) 1;
    (*this) -= one;
    return *this;
}


template<typename Type>
core_inline
vec3<Type> vec3<Type>::operator++(int)
{
    vec3<Type> result(*this);
    ++(*this);
    return result;
}

template<typename Type>
core_inline
vec3<Type> vec3<Type>::operator--(int)
{
    vec3<Type> result(*this);
    --(*this);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between two vectors.
 */
template<typename Type>
core_inline
vec3<Type> operator+(vec3<Type> lhs, const vec3<Type> &rhs)
{
    lhs += rhs;
    return lhs;
}

template<typename Type>
core_inline
vec3<Type> operator-(vec3<Type> lhs, const vec3<Type> &rhs)
{
    lhs -= rhs;
    return lhs;
}

template<typename Type>
core_inline
vec3<Type> operator*(vec3<Type> lhs, const vec3<Type> &rhs)
{
    lhs *= rhs;
    return lhs;
}

template<typename Type>
core_inline
vec3<Type> operator/(vec3<Type> lhs, const vec3<Type> &rhs)
{
    lhs /= rhs;
    return lhs;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
template<typename Type>
core_inline
vec3<Type> operator+(vec3<Type> lhs, const Type scalar)
{
    lhs += scalar;
    return lhs;
}

template<typename Type>
core_inline
vec3<Type> operator-(vec3<Type> lhs, const Type scalar)
{
    lhs -= scalar;
    return lhs;
}

template<typename Type>
core_inline
vec3<Type> operator*(vec3<Type> lhs, const Type scalar)
{
    lhs *= scalar;
    return lhs;
}

template<typename Type>
core_inline
vec3<Type> operator/(vec3<Type> lhs, const Type scalar)
{
    lhs /= scalar;
    return lhs;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between a scalar and a vector.
 * Division is not commutative, so its not implemented.
 */
template<typename Type>
core_inline
vec3<Type> operator+(const Type scalar, vec3<Type> rhs)
{
    rhs += scalar;
    return rhs;
}

template<typename Type>
core_inline
vec3<Type> operator-(const Type scalar, vec3<Type> rhs)
{
    rhs -= scalar;
    return rhs;
}

template<typename Type>
core_inline
vec3<Type> operator*(const Type scalar, vec3<Type> rhs)
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
#include "atto/math/geometry/vec3-simd.hpp"
#endif

#endif /* ATTO_MATH_GEOMETRY_VEC3_H_ */