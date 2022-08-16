/*
 * math.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_MATH_H_
#define ATTO_OPENCL_MATH_H_

#include "atto/opencl/base.hpp"

/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
#define ATTO_CL_VEC2_OPERATOR_COMPOUND_VECTOR(T) \
core_inline \
cl_ ## T ## 2 &operator+=(cl_ ## T ## 2 &lhs, const cl_ ## T ## 2 &rhs) \
{ \
    lhs.s[0] += rhs.s[0]; \
    lhs.s[1] += rhs.s[1]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 &operator-=(cl_ ## T ## 2 &lhs, const cl_ ## T ## 2 &rhs) \
{ \
    lhs.s[0] -= rhs.s[0]; \
    lhs.s[1] -= rhs.s[1]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 &operator*=(cl_ ## T ## 2 &lhs, const cl_ ## T ## 2 &rhs) \
{ \
    lhs.s[0] *= rhs.s[0]; \
    lhs.s[1] *= rhs.s[1]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 &operator/=(cl_ ## T ## 2 &lhs, const cl_ ## T ## 2 &rhs) \
{ \
    lhs.s[0] /= rhs.s[0]; \
    lhs.s[1] /= rhs.s[1]; \
    return lhs; \
}

/**
 * @brief Unary arithmetic scalar operators.
 */
#define ATTO_CL_VEC2_OPERATOR_COMPOUND_SCALAR(T) \
core_inline \
cl_ ## T ## 2 &operator+=(cl_ ## T ## 2 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] += scalar; \
    lhs.s[1] += scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 &operator-=(cl_ ## T ## 2 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] -= scalar; \
    lhs.s[1] -= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 &operator*=(cl_ ## T ## 2 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] *= scalar; \
    lhs.s[1] *= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 &operator/=(cl_ ## T ## 2 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] /= scalar; \
    lhs.s[1] /= scalar; \
    return lhs; \
}

/**
 * @brief Unary plus/negation operators.
 */
#define ATTO_CL_VEC2_OPERATOR_NEGATION(T) \
core_inline \
cl_ ## T ## 2 operator+(const cl_ ## T ## 2 &lhs) \
{ \
    cl_ ## T ## 2 result = lhs; \
    return result; \
} \
\
core_inline \
cl_ ## T ## 2 operator-(const cl_ ## T ## 2 &lhs) \
{ \
    constexpr cl_ ## T minus_one = (cl_ ## T) (-1); \
    cl_ ## T ## 2 result = lhs; \
    result *= minus_one; \
    return result; \
}

/**
 * @brief Increment operators.
 */
#define ATTO_CL_VEC2_OPERATOR_INCREMENT(T) \
core_inline \
cl_ ## T ## 2 &operator++(cl_ ## T ## 2 &lhs) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    lhs += one; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 &operator--(cl_ ## T ## 2 &lhs) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    lhs -= one; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 operator++(cl_ ## T ## 2 &lhs, int) \
{ \
    cl_ ## T ## 2 result = lhs; \
    ++lhs; \
    return result; \
} \
\
core_inline \
cl_ ## T ## 2 operator--(cl_ ## T ## 2 &lhs, int) \
{ \
    cl_ ## T ## 2 result = lhs; \
    --lhs; \
    return result; \
}

/**
 * @brief Binary arithmetic operators between two vectors.
 */
#define ATTO_CL_VEC2_OPERATOR_BINARY_VECTOR(T) \
core_inline \
cl_ ## T ## 2 operator+(cl_ ## T ## 2 lhs, const cl_ ## T ## 2 &rhs) \
{ \
    lhs += rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 operator-(cl_ ## T ## 2 lhs, const cl_ ## T ## 2 &rhs) \
{ \
    lhs -= rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 operator*(cl_ ## T ## 2 lhs, const cl_ ## T ## 2 &rhs) \
{ \
    lhs *= rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 operator/(cl_ ## T ## 2 lhs, const cl_ ## T ## 2 &rhs) \
{ \
    lhs /= rhs; \
    return lhs; \
}

/**
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
#define ATTO_CL_VEC2_OPERATOR_BINARY_SCALAR(T) \
core_inline \
cl_ ## T ## 2 operator+(cl_ ## T ## 2 lhs, const cl_ ## T scalar) \
{ \
    lhs += scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 operator-(cl_ ## T ## 2 lhs, const cl_ ## T scalar) \
{ \
    lhs -= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 operator*(cl_ ## T ## 2 lhs, const cl_ ## T scalar) \
{ \
    lhs *= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 2 operator/(cl_ ## T ## 2 lhs, const cl_ ## T scalar) \
{ \
    lhs /= scalar; \
    return lhs; \
}

ATTO_CL_VEC2_OPERATOR_COMPOUND_VECTOR(char)
ATTO_CL_VEC2_OPERATOR_COMPOUND_SCALAR(char)
ATTO_CL_VEC2_OPERATOR_NEGATION(char)
ATTO_CL_VEC2_OPERATOR_INCREMENT(char)
ATTO_CL_VEC2_OPERATOR_BINARY_VECTOR(char)
ATTO_CL_VEC2_OPERATOR_BINARY_SCALAR(char)

ATTO_CL_VEC2_OPERATOR_COMPOUND_VECTOR(uchar)
ATTO_CL_VEC2_OPERATOR_COMPOUND_SCALAR(uchar)
ATTO_CL_VEC2_OPERATOR_NEGATION(uchar)
ATTO_CL_VEC2_OPERATOR_INCREMENT(uchar)
ATTO_CL_VEC2_OPERATOR_BINARY_VECTOR(uchar)
ATTO_CL_VEC2_OPERATOR_BINARY_SCALAR(uchar)

ATTO_CL_VEC2_OPERATOR_COMPOUND_VECTOR(short)
ATTO_CL_VEC2_OPERATOR_COMPOUND_SCALAR(short)
ATTO_CL_VEC2_OPERATOR_NEGATION(short)
ATTO_CL_VEC2_OPERATOR_INCREMENT(short)
ATTO_CL_VEC2_OPERATOR_BINARY_VECTOR(short)
ATTO_CL_VEC2_OPERATOR_BINARY_SCALAR(short)

ATTO_CL_VEC2_OPERATOR_COMPOUND_VECTOR(ushort)
ATTO_CL_VEC2_OPERATOR_COMPOUND_SCALAR(ushort)
ATTO_CL_VEC2_OPERATOR_NEGATION(ushort)
ATTO_CL_VEC2_OPERATOR_INCREMENT(ushort)
ATTO_CL_VEC2_OPERATOR_BINARY_VECTOR(ushort)
ATTO_CL_VEC2_OPERATOR_BINARY_SCALAR(ushort)

ATTO_CL_VEC2_OPERATOR_COMPOUND_VECTOR(int)
ATTO_CL_VEC2_OPERATOR_COMPOUND_SCALAR(int)
ATTO_CL_VEC2_OPERATOR_NEGATION(int)
ATTO_CL_VEC2_OPERATOR_INCREMENT(int)
ATTO_CL_VEC2_OPERATOR_BINARY_VECTOR(int)
ATTO_CL_VEC2_OPERATOR_BINARY_SCALAR(int)

ATTO_CL_VEC2_OPERATOR_COMPOUND_VECTOR(uint)
ATTO_CL_VEC2_OPERATOR_COMPOUND_SCALAR(uint)
ATTO_CL_VEC2_OPERATOR_NEGATION(uint)
ATTO_CL_VEC2_OPERATOR_INCREMENT(uint)
ATTO_CL_VEC2_OPERATOR_BINARY_VECTOR(uint)
ATTO_CL_VEC2_OPERATOR_BINARY_SCALAR(uint)

ATTO_CL_VEC2_OPERATOR_COMPOUND_VECTOR(long)
ATTO_CL_VEC2_OPERATOR_COMPOUND_SCALAR(long)
ATTO_CL_VEC2_OPERATOR_NEGATION(long)
ATTO_CL_VEC2_OPERATOR_INCREMENT(long)
ATTO_CL_VEC2_OPERATOR_BINARY_VECTOR(long)
ATTO_CL_VEC2_OPERATOR_BINARY_SCALAR(long)

ATTO_CL_VEC2_OPERATOR_COMPOUND_VECTOR(ulong)
ATTO_CL_VEC2_OPERATOR_COMPOUND_SCALAR(ulong)
ATTO_CL_VEC2_OPERATOR_NEGATION(ulong)
ATTO_CL_VEC2_OPERATOR_INCREMENT(ulong)
ATTO_CL_VEC2_OPERATOR_BINARY_VECTOR(ulong)
ATTO_CL_VEC2_OPERATOR_BINARY_SCALAR(ulong)

ATTO_CL_VEC2_OPERATOR_COMPOUND_VECTOR(float)
ATTO_CL_VEC2_OPERATOR_COMPOUND_SCALAR(float)
ATTO_CL_VEC2_OPERATOR_NEGATION(float)
ATTO_CL_VEC2_OPERATOR_INCREMENT(float)
ATTO_CL_VEC2_OPERATOR_BINARY_VECTOR(float)
ATTO_CL_VEC2_OPERATOR_BINARY_SCALAR(float)

ATTO_CL_VEC2_OPERATOR_COMPOUND_VECTOR(double)
ATTO_CL_VEC2_OPERATOR_COMPOUND_SCALAR(double)
ATTO_CL_VEC2_OPERATOR_NEGATION(double)
ATTO_CL_VEC2_OPERATOR_INCREMENT(double)
ATTO_CL_VEC2_OPERATOR_BINARY_VECTOR(double)
ATTO_CL_VEC2_OPERATOR_BINARY_SCALAR(double)


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
#define ATTO_CL_VEC4_OPERATOR_COMPOUND_VECTOR(T) \
core_inline \
cl_ ## T ## 4 &operator+=(cl_ ## T ## 4 &lhs, const cl_ ## T ## 4 &rhs) \
{ \
    lhs.s[0] += rhs.s[0]; \
    lhs.s[1] += rhs.s[1]; \
    lhs.s[2] += rhs.s[2]; \
    lhs.s[3] += rhs.s[3]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 &operator-=(cl_ ## T ## 4 &lhs, const cl_ ## T ## 4 &rhs) \
{ \
    lhs.s[0] -= rhs.s[0]; \
    lhs.s[1] -= rhs.s[1]; \
    lhs.s[2] -= rhs.s[2]; \
    lhs.s[3] -= rhs.s[3]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 &operator*=(cl_ ## T ## 4 &lhs, const cl_ ## T ## 4 &rhs) \
{ \
    lhs.s[0] *= rhs.s[0]; \
    lhs.s[1] *= rhs.s[1]; \
    lhs.s[2] *= rhs.s[2]; \
    lhs.s[3] *= rhs.s[3]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 &operator/=(cl_ ## T ## 4 &lhs, const cl_ ## T ## 4 &rhs) \
{ \
    lhs.s[0] /= rhs.s[0]; \
    lhs.s[1] /= rhs.s[1]; \
    lhs.s[2] /= rhs.s[2]; \
    lhs.s[3] /= rhs.s[3]; \
    return lhs; \
}

/**
 * @brief Unary arithmetic scalar operators.
 */
#define ATTO_CL_VEC4_OPERATOR_COMPOUND_SCALAR(T) \
core_inline \
cl_ ## T ## 4 &operator+=(cl_ ## T ## 4 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] += scalar; \
    lhs.s[1] += scalar; \
    lhs.s[2] += scalar; \
    lhs.s[3] += scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 &operator-=(cl_ ## T ## 4 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] -= scalar; \
    lhs.s[1] -= scalar; \
    lhs.s[2] -= scalar; \
    lhs.s[3] -= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 &operator*=(cl_ ## T ## 4 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] *= scalar; \
    lhs.s[1] *= scalar; \
    lhs.s[2] *= scalar; \
    lhs.s[3] *= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 &operator/=(cl_ ## T ## 4 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] /= scalar; \
    lhs.s[1] /= scalar; \
    lhs.s[2] /= scalar; \
    lhs.s[3] /= scalar; \
    return lhs; \
}

/**
 * @brief Unary plus/negation operators.
 */
#define ATTO_CL_VEC4_OPERATOR_NEGATION(T) \
core_inline \
cl_ ## T ## 4 operator+(const cl_ ## T ## 4 &lhs) \
{ \
    cl_ ## T ## 4 result = lhs; \
    return result; \
} \
\
core_inline \
cl_ ## T ## 4 operator-(const cl_ ## T ## 4 &lhs) \
{ \
    constexpr cl_ ## T minus_one = (cl_ ## T) (-1); \
    cl_ ## T ## 4 result = lhs; \
    result *= minus_one; \
    return result; \
}

/**
 * @brief Increment operators.
 */
#define ATTO_CL_VEC4_OPERATOR_INCREMENT(T) \
core_inline \
cl_ ## T ## 4 &operator++(cl_ ## T ## 4 &lhs) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    lhs += one; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 &operator--(cl_ ## T ## 4 &lhs) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    lhs -= one; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 operator++(cl_ ## T ## 4 &lhs, int) \
{ \
    cl_ ## T ## 4 result = lhs; \
    ++lhs; \
    return result; \
} \
\
core_inline \
cl_ ## T ## 4 operator--(cl_ ## T ## 4 &lhs, int) \
{ \
    cl_ ## T ## 4 result = lhs; \
    --lhs; \
    return result; \
}

/**
 * @brief Binary arithmetic operators between two vectors.
 */
#define ATTO_CL_VEC4_OPERATOR_BINARY_VECTOR(T) \
core_inline \
cl_ ## T ## 4 operator+(cl_ ## T ## 4 lhs, const cl_ ## T ## 4 &rhs) \
{ \
    lhs += rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 operator-(cl_ ## T ## 4 lhs, const cl_ ## T ## 4 &rhs) \
{ \
    lhs -= rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 operator*(cl_ ## T ## 4 lhs, const cl_ ## T ## 4 &rhs) \
{ \
    lhs *= rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 operator/(cl_ ## T ## 4 lhs, const cl_ ## T ## 4 &rhs) \
{ \
    lhs /= rhs; \
    return lhs; \
}

/**
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
#define ATTO_CL_VEC4_OPERATOR_BINARY_SCALAR(T) \
core_inline \
cl_ ## T ## 4 operator+(cl_ ## T ## 4 lhs, const cl_ ## T scalar) \
{ \
    lhs += scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 operator-(cl_ ## T ## 4 lhs, const cl_ ## T scalar) \
{ \
    lhs -= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 operator*(cl_ ## T ## 4 lhs, const cl_ ## T scalar) \
{ \
    lhs *= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 4 operator/(cl_ ## T ## 4 lhs, const cl_ ## T scalar) \
{ \
    lhs /= scalar; \
    return lhs; \
}

ATTO_CL_VEC4_OPERATOR_COMPOUND_VECTOR(char)
ATTO_CL_VEC4_OPERATOR_COMPOUND_SCALAR(char)
ATTO_CL_VEC4_OPERATOR_NEGATION(char)
ATTO_CL_VEC4_OPERATOR_INCREMENT(char)
ATTO_CL_VEC4_OPERATOR_BINARY_VECTOR(char)
ATTO_CL_VEC4_OPERATOR_BINARY_SCALAR(char)

ATTO_CL_VEC4_OPERATOR_COMPOUND_VECTOR(uchar)
ATTO_CL_VEC4_OPERATOR_COMPOUND_SCALAR(uchar)
ATTO_CL_VEC4_OPERATOR_NEGATION(uchar)
ATTO_CL_VEC4_OPERATOR_INCREMENT(uchar)
ATTO_CL_VEC4_OPERATOR_BINARY_VECTOR(uchar)
ATTO_CL_VEC4_OPERATOR_BINARY_SCALAR(uchar)

ATTO_CL_VEC4_OPERATOR_COMPOUND_VECTOR(short)
ATTO_CL_VEC4_OPERATOR_COMPOUND_SCALAR(short)
ATTO_CL_VEC4_OPERATOR_NEGATION(short)
ATTO_CL_VEC4_OPERATOR_INCREMENT(short)
ATTO_CL_VEC4_OPERATOR_BINARY_VECTOR(short)
ATTO_CL_VEC4_OPERATOR_BINARY_SCALAR(short)

ATTO_CL_VEC4_OPERATOR_COMPOUND_VECTOR(ushort)
ATTO_CL_VEC4_OPERATOR_COMPOUND_SCALAR(ushort)
ATTO_CL_VEC4_OPERATOR_NEGATION(ushort)
ATTO_CL_VEC4_OPERATOR_INCREMENT(ushort)
ATTO_CL_VEC4_OPERATOR_BINARY_VECTOR(ushort)
ATTO_CL_VEC4_OPERATOR_BINARY_SCALAR(ushort)

ATTO_CL_VEC4_OPERATOR_COMPOUND_VECTOR(int)
ATTO_CL_VEC4_OPERATOR_COMPOUND_SCALAR(int)
ATTO_CL_VEC4_OPERATOR_NEGATION(int)
ATTO_CL_VEC4_OPERATOR_INCREMENT(int)
ATTO_CL_VEC4_OPERATOR_BINARY_VECTOR(int)
ATTO_CL_VEC4_OPERATOR_BINARY_SCALAR(int)

ATTO_CL_VEC4_OPERATOR_COMPOUND_VECTOR(uint)
ATTO_CL_VEC4_OPERATOR_COMPOUND_SCALAR(uint)
ATTO_CL_VEC4_OPERATOR_NEGATION(uint)
ATTO_CL_VEC4_OPERATOR_INCREMENT(uint)
ATTO_CL_VEC4_OPERATOR_BINARY_VECTOR(uint)
ATTO_CL_VEC4_OPERATOR_BINARY_SCALAR(uint)

ATTO_CL_VEC4_OPERATOR_COMPOUND_VECTOR(long)
ATTO_CL_VEC4_OPERATOR_COMPOUND_SCALAR(long)
ATTO_CL_VEC4_OPERATOR_NEGATION(long)
ATTO_CL_VEC4_OPERATOR_INCREMENT(long)
ATTO_CL_VEC4_OPERATOR_BINARY_VECTOR(long)
ATTO_CL_VEC4_OPERATOR_BINARY_SCALAR(long)

ATTO_CL_VEC4_OPERATOR_COMPOUND_VECTOR(ulong)
ATTO_CL_VEC4_OPERATOR_COMPOUND_SCALAR(ulong)
ATTO_CL_VEC4_OPERATOR_NEGATION(ulong)
ATTO_CL_VEC4_OPERATOR_INCREMENT(ulong)
ATTO_CL_VEC4_OPERATOR_BINARY_VECTOR(ulong)
ATTO_CL_VEC4_OPERATOR_BINARY_SCALAR(ulong)

ATTO_CL_VEC4_OPERATOR_COMPOUND_VECTOR(float)
ATTO_CL_VEC4_OPERATOR_COMPOUND_SCALAR(float)
ATTO_CL_VEC4_OPERATOR_NEGATION(float)
ATTO_CL_VEC4_OPERATOR_INCREMENT(float)
ATTO_CL_VEC4_OPERATOR_BINARY_VECTOR(float)
ATTO_CL_VEC4_OPERATOR_BINARY_SCALAR(float)

ATTO_CL_VEC4_OPERATOR_COMPOUND_VECTOR(double)
ATTO_CL_VEC4_OPERATOR_COMPOUND_SCALAR(double)
ATTO_CL_VEC4_OPERATOR_NEGATION(double)
ATTO_CL_VEC4_OPERATOR_INCREMENT(double)
ATTO_CL_VEC4_OPERATOR_BINARY_VECTOR(double)
ATTO_CL_VEC4_OPERATOR_BINARY_SCALAR(double)


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
#define ATTO_CL_VEC8_OPERATOR_COMPOUND_VECTOR(T) \
core_inline \
cl_ ## T ## 8 &operator+=(cl_ ## T ## 8 &lhs, const cl_ ## T ## 8 &rhs) \
{ \
    lhs.s[0] += rhs.s[0]; \
    lhs.s[1] += rhs.s[1]; \
    lhs.s[2] += rhs.s[2]; \
    lhs.s[3] += rhs.s[3]; \
    lhs.s[4] += rhs.s[4]; \
    lhs.s[5] += rhs.s[5]; \
    lhs.s[6] += rhs.s[6]; \
    lhs.s[7] += rhs.s[7]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 &operator-=(cl_ ## T ## 8 &lhs, const cl_ ## T ## 8 &rhs) \
{ \
    lhs.s[0] -= rhs.s[0]; \
    lhs.s[1] -= rhs.s[1]; \
    lhs.s[2] -= rhs.s[2]; \
    lhs.s[3] -= rhs.s[3]; \
    lhs.s[4] -= rhs.s[4]; \
    lhs.s[5] -= rhs.s[5]; \
    lhs.s[6] -= rhs.s[6]; \
    lhs.s[7] -= rhs.s[7]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 &operator*=(cl_ ## T ## 8 &lhs, const cl_ ## T ## 8 &rhs) \
{ \
    lhs.s[0] *= rhs.s[0]; \
    lhs.s[1] *= rhs.s[1]; \
    lhs.s[2] *= rhs.s[2]; \
    lhs.s[3] *= rhs.s[3]; \
    lhs.s[4] *= rhs.s[4]; \
    lhs.s[5] *= rhs.s[5]; \
    lhs.s[6] *= rhs.s[6]; \
    lhs.s[7] *= rhs.s[7]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 &operator/=(cl_ ## T ## 8 &lhs, const cl_ ## T ## 8 &rhs) \
{ \
    lhs.s[0] /= rhs.s[0]; \
    lhs.s[1] /= rhs.s[1]; \
    lhs.s[2] /= rhs.s[2]; \
    lhs.s[3] /= rhs.s[3]; \
    lhs.s[4] /= rhs.s[4]; \
    lhs.s[5] /= rhs.s[5]; \
    lhs.s[6] /= rhs.s[6]; \
    lhs.s[7] /= rhs.s[7]; \
    return lhs; \
}

/**
 * @brief Unary arithmetic scalar operators.
 */
#define ATTO_CL_VEC8_OPERATOR_COMPOUND_SCALAR(T) \
core_inline \
cl_ ## T ## 8 &operator+=(cl_ ## T ## 8 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] += scalar; \
    lhs.s[1] += scalar; \
    lhs.s[2] += scalar; \
    lhs.s[3] += scalar; \
    lhs.s[4] += scalar; \
    lhs.s[5] += scalar; \
    lhs.s[6] += scalar; \
    lhs.s[7] += scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 &operator-=(cl_ ## T ## 8 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] -= scalar; \
    lhs.s[1] -= scalar; \
    lhs.s[2] -= scalar; \
    lhs.s[3] -= scalar; \
    lhs.s[4] -= scalar; \
    lhs.s[5] -= scalar; \
    lhs.s[6] -= scalar; \
    lhs.s[7] -= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 &operator*=(cl_ ## T ## 8 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] *= scalar; \
    lhs.s[1] *= scalar; \
    lhs.s[2] *= scalar; \
    lhs.s[3] *= scalar; \
    lhs.s[4] *= scalar; \
    lhs.s[5] *= scalar; \
    lhs.s[6] *= scalar; \
    lhs.s[7] *= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 &operator/=(cl_ ## T ## 8 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0] /= scalar; \
    lhs.s[1] /= scalar; \
    lhs.s[2] /= scalar; \
    lhs.s[3] /= scalar; \
    lhs.s[4] /= scalar; \
    lhs.s[5] /= scalar; \
    lhs.s[6] /= scalar; \
    lhs.s[7] /= scalar; \
    return lhs; \
}

/**
 * @brief Unary plus/negation operators.
 */
#define ATTO_CL_VEC8_OPERATOR_NEGATION(T) \
core_inline \
cl_ ## T ## 8 operator+(const cl_ ## T ## 8 &lhs) \
{ \
    cl_ ## T ## 8 result = lhs; \
    return result; \
} \
\
core_inline \
cl_ ## T ## 8 operator-(const cl_ ## T ## 8 &lhs) \
{ \
    constexpr cl_ ## T minus_one = (cl_ ## T) (-1); \
    cl_ ## T ## 8 result = lhs; \
    result *= minus_one; \
    return result; \
}

/**
 * @brief Increment operators.
 */
#define ATTO_CL_VEC8_OPERATOR_INCREMENT(T) \
core_inline \
cl_ ## T ## 8 &operator++(cl_ ## T ## 8 &lhs) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    lhs += one; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 &operator--(cl_ ## T ## 8 &lhs) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    lhs -= one; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 operator++(cl_ ## T ## 8 &lhs, int) \
{ \
    cl_ ## T ## 8 result = lhs; \
    ++lhs; \
    return result; \
} \
\
core_inline \
cl_ ## T ## 8 operator--(cl_ ## T ## 8 &lhs, int) \
{ \
    cl_ ## T ## 8 result = lhs; \
    --lhs; \
    return result; \
}

/**
 * @brief Binary arithmetic operators between two vectors.
 */
#define ATTO_CL_VEC8_OPERATOR_BINARY_VECTOR(T) \
core_inline \
cl_ ## T ## 8 operator+(cl_ ## T ## 8 lhs, const cl_ ## T ## 8 &rhs) \
{ \
    lhs += rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 operator-(cl_ ## T ## 8 lhs, const cl_ ## T ## 8 &rhs) \
{ \
    lhs -= rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 operator*(cl_ ## T ## 8 lhs, const cl_ ## T ## 8 &rhs) \
{ \
    lhs *= rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 operator/(cl_ ## T ## 8 lhs, const cl_ ## T ## 8 &rhs) \
{ \
    lhs /= rhs; \
    return lhs; \
}

/**
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
#define ATTO_CL_VEC8_OPERATOR_BINARY_SCALAR(T) \
core_inline \
cl_ ## T ## 8 operator+(cl_ ## T ## 8 lhs, const cl_ ## T scalar) \
{ \
    lhs += scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 operator-(cl_ ## T ## 8 lhs, const cl_ ## T scalar) \
{ \
    lhs -= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 operator*(cl_ ## T ## 8 lhs, const cl_ ## T scalar) \
{ \
    lhs *= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 8 operator/(cl_ ## T ## 8 lhs, const cl_ ## T scalar) \
{ \
    lhs /= scalar; \
    return lhs; \
}

ATTO_CL_VEC8_OPERATOR_COMPOUND_VECTOR(char)
ATTO_CL_VEC8_OPERATOR_COMPOUND_SCALAR(char)
ATTO_CL_VEC8_OPERATOR_NEGATION(char)
ATTO_CL_VEC8_OPERATOR_INCREMENT(char)
ATTO_CL_VEC8_OPERATOR_BINARY_VECTOR(char)
ATTO_CL_VEC8_OPERATOR_BINARY_SCALAR(char)

ATTO_CL_VEC8_OPERATOR_COMPOUND_VECTOR(uchar)
ATTO_CL_VEC8_OPERATOR_COMPOUND_SCALAR(uchar)
ATTO_CL_VEC8_OPERATOR_NEGATION(uchar)
ATTO_CL_VEC8_OPERATOR_INCREMENT(uchar)
ATTO_CL_VEC8_OPERATOR_BINARY_VECTOR(uchar)
ATTO_CL_VEC8_OPERATOR_BINARY_SCALAR(uchar)

ATTO_CL_VEC8_OPERATOR_COMPOUND_VECTOR(short)
ATTO_CL_VEC8_OPERATOR_COMPOUND_SCALAR(short)
ATTO_CL_VEC8_OPERATOR_NEGATION(short)
ATTO_CL_VEC8_OPERATOR_INCREMENT(short)
ATTO_CL_VEC8_OPERATOR_BINARY_VECTOR(short)
ATTO_CL_VEC8_OPERATOR_BINARY_SCALAR(short)

ATTO_CL_VEC8_OPERATOR_COMPOUND_VECTOR(ushort)
ATTO_CL_VEC8_OPERATOR_COMPOUND_SCALAR(ushort)
ATTO_CL_VEC8_OPERATOR_NEGATION(ushort)
ATTO_CL_VEC8_OPERATOR_INCREMENT(ushort)
ATTO_CL_VEC8_OPERATOR_BINARY_VECTOR(ushort)
ATTO_CL_VEC8_OPERATOR_BINARY_SCALAR(ushort)

ATTO_CL_VEC8_OPERATOR_COMPOUND_VECTOR(int)
ATTO_CL_VEC8_OPERATOR_COMPOUND_SCALAR(int)
ATTO_CL_VEC8_OPERATOR_NEGATION(int)
ATTO_CL_VEC8_OPERATOR_INCREMENT(int)
ATTO_CL_VEC8_OPERATOR_BINARY_VECTOR(int)
ATTO_CL_VEC8_OPERATOR_BINARY_SCALAR(int)

ATTO_CL_VEC8_OPERATOR_COMPOUND_VECTOR(uint)
ATTO_CL_VEC8_OPERATOR_COMPOUND_SCALAR(uint)
ATTO_CL_VEC8_OPERATOR_NEGATION(uint)
ATTO_CL_VEC8_OPERATOR_INCREMENT(uint)
ATTO_CL_VEC8_OPERATOR_BINARY_VECTOR(uint)
ATTO_CL_VEC8_OPERATOR_BINARY_SCALAR(uint)

ATTO_CL_VEC8_OPERATOR_COMPOUND_VECTOR(long)
ATTO_CL_VEC8_OPERATOR_COMPOUND_SCALAR(long)
ATTO_CL_VEC8_OPERATOR_NEGATION(long)
ATTO_CL_VEC8_OPERATOR_INCREMENT(long)
ATTO_CL_VEC8_OPERATOR_BINARY_VECTOR(long)
ATTO_CL_VEC8_OPERATOR_BINARY_SCALAR(long)

ATTO_CL_VEC8_OPERATOR_COMPOUND_VECTOR(ulong)
ATTO_CL_VEC8_OPERATOR_COMPOUND_SCALAR(ulong)
ATTO_CL_VEC8_OPERATOR_NEGATION(ulong)
ATTO_CL_VEC8_OPERATOR_INCREMENT(ulong)
ATTO_CL_VEC8_OPERATOR_BINARY_VECTOR(ulong)
ATTO_CL_VEC8_OPERATOR_BINARY_SCALAR(ulong)

ATTO_CL_VEC8_OPERATOR_COMPOUND_VECTOR(float)
ATTO_CL_VEC8_OPERATOR_COMPOUND_SCALAR(float)
ATTO_CL_VEC8_OPERATOR_NEGATION(float)
ATTO_CL_VEC8_OPERATOR_INCREMENT(float)
ATTO_CL_VEC8_OPERATOR_BINARY_VECTOR(float)
ATTO_CL_VEC8_OPERATOR_BINARY_SCALAR(float)

ATTO_CL_VEC8_OPERATOR_COMPOUND_VECTOR(double)
ATTO_CL_VEC8_OPERATOR_COMPOUND_SCALAR(double)
ATTO_CL_VEC8_OPERATOR_NEGATION(double)
ATTO_CL_VEC8_OPERATOR_INCREMENT(double)
ATTO_CL_VEC8_OPERATOR_BINARY_VECTOR(double)
ATTO_CL_VEC8_OPERATOR_BINARY_SCALAR(double)


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
#define ATTO_CL_VEC16_OPERATOR_COMPOUND_VECTOR(T) \
core_inline \
cl_ ## T ## 16 &operator+=(cl_ ## T ## 16 &lhs, const cl_ ## T ## 16 &rhs) \
{ \
    lhs.s[0]  += rhs.s[0]; \
    lhs.s[1]  += rhs.s[1]; \
    lhs.s[2]  += rhs.s[2]; \
    lhs.s[3]  += rhs.s[3]; \
    lhs.s[4]  += rhs.s[4]; \
    lhs.s[5]  += rhs.s[5]; \
    lhs.s[6]  += rhs.s[6]; \
    lhs.s[7]  += rhs.s[7]; \
    lhs.s[8]  += rhs.s[8]; \
    lhs.s[9]  += rhs.s[9]; \
    lhs.s[10] += rhs.s[10]; \
    lhs.s[11] += rhs.s[11]; \
    lhs.s[12] += rhs.s[12]; \
    lhs.s[13] += rhs.s[13]; \
    lhs.s[14] += rhs.s[14]; \
    lhs.s[15] += rhs.s[15]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 &operator-=(cl_ ## T ## 16 &lhs, const cl_ ## T ## 16 &rhs) \
{ \
    lhs.s[0]  -= rhs.s[0]; \
    lhs.s[1]  -= rhs.s[1]; \
    lhs.s[2]  -= rhs.s[2]; \
    lhs.s[3]  -= rhs.s[3]; \
    lhs.s[4]  -= rhs.s[4]; \
    lhs.s[5]  -= rhs.s[5]; \
    lhs.s[6]  -= rhs.s[6]; \
    lhs.s[7]  -= rhs.s[7]; \
    lhs.s[8]  -= rhs.s[8]; \
    lhs.s[9]  -= rhs.s[9]; \
    lhs.s[10] -= rhs.s[10]; \
    lhs.s[11] -= rhs.s[11]; \
    lhs.s[12] -= rhs.s[12]; \
    lhs.s[13] -= rhs.s[13]; \
    lhs.s[14] -= rhs.s[14]; \
    lhs.s[15] -= rhs.s[15]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 &operator*=(cl_ ## T ## 16 &lhs, const cl_ ## T ## 16 &rhs) \
{ \
    lhs.s[0]  *= rhs.s[0]; \
    lhs.s[1]  *= rhs.s[1]; \
    lhs.s[2]  *= rhs.s[2]; \
    lhs.s[3]  *= rhs.s[3]; \
    lhs.s[4]  *= rhs.s[4]; \
    lhs.s[5]  *= rhs.s[5]; \
    lhs.s[6]  *= rhs.s[6]; \
    lhs.s[7]  *= rhs.s[7]; \
    lhs.s[8]  *= rhs.s[8]; \
    lhs.s[9]  *= rhs.s[9]; \
    lhs.s[10] *= rhs.s[10]; \
    lhs.s[11] *= rhs.s[11]; \
    lhs.s[12] *= rhs.s[12]; \
    lhs.s[13] *= rhs.s[13]; \
    lhs.s[14] *= rhs.s[14]; \
    lhs.s[15] *= rhs.s[15]; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 &operator/=(cl_ ## T ## 16 &lhs, const cl_ ## T ## 16 &rhs) \
{ \
    lhs.s[0]  /= rhs.s[0]; \
    lhs.s[1]  /= rhs.s[1]; \
    lhs.s[2]  /= rhs.s[2]; \
    lhs.s[3]  /= rhs.s[3]; \
    lhs.s[4]  /= rhs.s[4]; \
    lhs.s[5]  /= rhs.s[5]; \
    lhs.s[6]  /= rhs.s[6]; \
    lhs.s[7]  /= rhs.s[7]; \
    lhs.s[8]  /= rhs.s[8]; \
    lhs.s[9]  /= rhs.s[9]; \
    lhs.s[10] /= rhs.s[10]; \
    lhs.s[11] /= rhs.s[11]; \
    lhs.s[12] /= rhs.s[12]; \
    lhs.s[13] /= rhs.s[13]; \
    lhs.s[14] /= rhs.s[14]; \
    lhs.s[15] /= rhs.s[15]; \
    return lhs; \
}

/**
 * @brief Unary arithmetic scalar operators.
 */
#define ATTO_CL_VEC16_OPERATOR_COMPOUND_SCALAR(T) \
core_inline \
cl_ ## T ## 16 &operator+=(cl_ ## T ## 16 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0]  += scalar; \
    lhs.s[1]  += scalar; \
    lhs.s[2]  += scalar; \
    lhs.s[3]  += scalar; \
    lhs.s[4]  += scalar; \
    lhs.s[5]  += scalar; \
    lhs.s[6]  += scalar; \
    lhs.s[7]  += scalar; \
    lhs.s[8]  += scalar; \
    lhs.s[9]  += scalar; \
    lhs.s[10] += scalar; \
    lhs.s[11] += scalar; \
    lhs.s[12] += scalar; \
    lhs.s[13] += scalar; \
    lhs.s[14] += scalar; \
    lhs.s[15] += scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 &operator-=(cl_ ## T ## 16 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0]  -= scalar; \
    lhs.s[1]  -= scalar; \
    lhs.s[2]  -= scalar; \
    lhs.s[3]  -= scalar; \
    lhs.s[4]  -= scalar; \
    lhs.s[5]  -= scalar; \
    lhs.s[6]  -= scalar; \
    lhs.s[7]  -= scalar; \
    lhs.s[8]  -= scalar; \
    lhs.s[9]  -= scalar; \
    lhs.s[10] -= scalar; \
    lhs.s[11] -= scalar; \
    lhs.s[12] -= scalar; \
    lhs.s[13] -= scalar; \
    lhs.s[14] -= scalar; \
    lhs.s[15] -= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 &operator*=(cl_ ## T ## 16 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0]  *= scalar; \
    lhs.s[1]  *= scalar; \
    lhs.s[2]  *= scalar; \
    lhs.s[3]  *= scalar; \
    lhs.s[4]  *= scalar; \
    lhs.s[5]  *= scalar; \
    lhs.s[6]  *= scalar; \
    lhs.s[7]  *= scalar; \
    lhs.s[8]  *= scalar; \
    lhs.s[9]  *= scalar; \
    lhs.s[10] *= scalar; \
    lhs.s[11] *= scalar; \
    lhs.s[12] *= scalar; \
    lhs.s[13] *= scalar; \
    lhs.s[14] *= scalar; \
    lhs.s[15] *= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 &operator/=(cl_ ## T ## 16 &lhs, const cl_ ## T scalar) \
{ \
    lhs.s[0]  /= scalar; \
    lhs.s[1]  /= scalar; \
    lhs.s[2]  /= scalar; \
    lhs.s[3]  /= scalar; \
    lhs.s[4]  /= scalar; \
    lhs.s[5]  /= scalar; \
    lhs.s[6]  /= scalar; \
    lhs.s[7]  /= scalar; \
    lhs.s[8]  /= scalar; \
    lhs.s[9]  /= scalar; \
    lhs.s[10] /= scalar; \
    lhs.s[11] /= scalar; \
    lhs.s[12] /= scalar; \
    lhs.s[13] /= scalar; \
    lhs.s[14] /= scalar; \
    lhs.s[15] /= scalar; \
    return lhs; \
}

/**
 * @brief Unary plus/negation operators.
 */
#define ATTO_CL_VEC16_OPERATOR_NEGATION(T) \
core_inline \
cl_ ## T ## 16 operator+(const cl_ ## T ## 16 &lhs) \
{ \
    cl_ ## T ## 16 result = lhs; \
    return result; \
} \
\
core_inline \
cl_ ## T ## 16 operator-(const cl_ ## T ## 16 &lhs) \
{ \
    constexpr cl_ ## T minus_one = (cl_ ## T) (-1); \
    cl_ ## T ## 16 result = lhs; \
    result *= minus_one; \
    return result; \
}

/**
 * @brief Increment operators.
 */
#define ATTO_CL_VEC16_OPERATOR_INCREMENT(T) \
core_inline \
cl_ ## T ## 16 &operator++(cl_ ## T ## 16 &lhs) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    lhs += one; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 &operator--(cl_ ## T ## 16 &lhs) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    lhs -= one; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 operator++(cl_ ## T ## 16 &lhs, int) \
{ \
    cl_ ## T ## 16 result = lhs; \
    ++lhs; \
    return result; \
} \
\
core_inline \
cl_ ## T ## 16 operator--(cl_ ## T ## 16 &lhs, int) \
{ \
    cl_ ## T ## 16 result = lhs; \
    --lhs; \
    return result; \
}

/**
 * @brief Binary arithmetic operators between two vectors.
 */
#define ATTO_CL_VEC16_OPERATOR_BINARY_VECTOR(T) \
core_inline \
cl_ ## T ## 16 operator+(cl_ ## T ## 16 lhs, const cl_ ## T ## 16 &rhs) \
{ \
    lhs += rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 operator-(cl_ ## T ## 16 lhs, const cl_ ## T ## 16 &rhs) \
{ \
    lhs -= rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 operator*(cl_ ## T ## 16 lhs, const cl_ ## T ## 16 &rhs) \
{ \
    lhs *= rhs; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 operator/(cl_ ## T ## 16 lhs, const cl_ ## T ## 16 &rhs) \
{ \
    lhs /= rhs; \
    return lhs; \
}

/**
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
#define ATTO_CL_VEC16_OPERATOR_BINARY_SCALAR(T) \
core_inline \
cl_ ## T ## 16 operator+(cl_ ## T ## 16 lhs, const cl_ ## T scalar) \
{ \
    lhs += scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 operator-(cl_ ## T ## 16 lhs, const cl_ ## T scalar) \
{ \
    lhs -= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 operator*(cl_ ## T ## 16 lhs, const cl_ ## T scalar) \
{ \
    lhs *= scalar; \
    return lhs; \
} \
\
core_inline \
cl_ ## T ## 16 operator/(cl_ ## T ## 16 lhs, const cl_ ## T scalar) \
{ \
    lhs /= scalar; \
    return lhs; \
}

ATTO_CL_VEC16_OPERATOR_COMPOUND_VECTOR(char)
ATTO_CL_VEC16_OPERATOR_COMPOUND_SCALAR(char)
ATTO_CL_VEC16_OPERATOR_NEGATION(char)
ATTO_CL_VEC16_OPERATOR_INCREMENT(char)
ATTO_CL_VEC16_OPERATOR_BINARY_VECTOR(char)
ATTO_CL_VEC16_OPERATOR_BINARY_SCALAR(char)

ATTO_CL_VEC16_OPERATOR_COMPOUND_VECTOR(uchar)
ATTO_CL_VEC16_OPERATOR_COMPOUND_SCALAR(uchar)
ATTO_CL_VEC16_OPERATOR_NEGATION(uchar)
ATTO_CL_VEC16_OPERATOR_INCREMENT(uchar)
ATTO_CL_VEC16_OPERATOR_BINARY_VECTOR(uchar)
ATTO_CL_VEC16_OPERATOR_BINARY_SCALAR(uchar)

ATTO_CL_VEC16_OPERATOR_COMPOUND_VECTOR(short)
ATTO_CL_VEC16_OPERATOR_COMPOUND_SCALAR(short)
ATTO_CL_VEC16_OPERATOR_NEGATION(short)
ATTO_CL_VEC16_OPERATOR_INCREMENT(short)
ATTO_CL_VEC16_OPERATOR_BINARY_VECTOR(short)
ATTO_CL_VEC16_OPERATOR_BINARY_SCALAR(short)

ATTO_CL_VEC16_OPERATOR_COMPOUND_VECTOR(ushort)
ATTO_CL_VEC16_OPERATOR_COMPOUND_SCALAR(ushort)
ATTO_CL_VEC16_OPERATOR_NEGATION(ushort)
ATTO_CL_VEC16_OPERATOR_INCREMENT(ushort)
ATTO_CL_VEC16_OPERATOR_BINARY_VECTOR(ushort)
ATTO_CL_VEC16_OPERATOR_BINARY_SCALAR(ushort)

ATTO_CL_VEC16_OPERATOR_COMPOUND_VECTOR(int)
ATTO_CL_VEC16_OPERATOR_COMPOUND_SCALAR(int)
ATTO_CL_VEC16_OPERATOR_NEGATION(int)
ATTO_CL_VEC16_OPERATOR_INCREMENT(int)
ATTO_CL_VEC16_OPERATOR_BINARY_VECTOR(int)
ATTO_CL_VEC16_OPERATOR_BINARY_SCALAR(int)

ATTO_CL_VEC16_OPERATOR_COMPOUND_VECTOR(uint)
ATTO_CL_VEC16_OPERATOR_COMPOUND_SCALAR(uint)
ATTO_CL_VEC16_OPERATOR_NEGATION(uint)
ATTO_CL_VEC16_OPERATOR_INCREMENT(uint)
ATTO_CL_VEC16_OPERATOR_BINARY_VECTOR(uint)
ATTO_CL_VEC16_OPERATOR_BINARY_SCALAR(uint)

ATTO_CL_VEC16_OPERATOR_COMPOUND_VECTOR(long)
ATTO_CL_VEC16_OPERATOR_COMPOUND_SCALAR(long)
ATTO_CL_VEC16_OPERATOR_NEGATION(long)
ATTO_CL_VEC16_OPERATOR_INCREMENT(long)
ATTO_CL_VEC16_OPERATOR_BINARY_VECTOR(long)
ATTO_CL_VEC16_OPERATOR_BINARY_SCALAR(long)

ATTO_CL_VEC16_OPERATOR_COMPOUND_VECTOR(ulong)
ATTO_CL_VEC16_OPERATOR_COMPOUND_SCALAR(ulong)
ATTO_CL_VEC16_OPERATOR_NEGATION(ulong)
ATTO_CL_VEC16_OPERATOR_INCREMENT(ulong)
ATTO_CL_VEC16_OPERATOR_BINARY_VECTOR(ulong)
ATTO_CL_VEC16_OPERATOR_BINARY_SCALAR(ulong)

ATTO_CL_VEC16_OPERATOR_COMPOUND_VECTOR(float)
ATTO_CL_VEC16_OPERATOR_COMPOUND_SCALAR(float)
ATTO_CL_VEC16_OPERATOR_NEGATION(float)
ATTO_CL_VEC16_OPERATOR_INCREMENT(float)
ATTO_CL_VEC16_OPERATOR_BINARY_VECTOR(float)
ATTO_CL_VEC16_OPERATOR_BINARY_SCALAR(float)

ATTO_CL_VEC16_OPERATOR_COMPOUND_VECTOR(double)
ATTO_CL_VEC16_OPERATOR_COMPOUND_SCALAR(double)
ATTO_CL_VEC16_OPERATOR_NEGATION(double)
ATTO_CL_VEC16_OPERATOR_INCREMENT(double)
ATTO_CL_VEC16_OPERATOR_BINARY_VECTOR(double)
ATTO_CL_VEC16_OPERATOR_BINARY_SCALAR(double)


namespace atto {
namespace cl {

/** ---------------------------------------------------------------------------
 * @brief OpenCL cl_vec2 arithmetic functions.
 */
#define ATTO_CL_VEC2_ARITHMETIC(T) \
core_inline \
cl_ ## T ## 2 abs(const cl_ ## T ## 2 &v) \
{ \
    return cl_ ## T ## 2{ \
        std::abs(v.s[0]), \
        std::abs(v.s[1])}; \
} \
 \
core_inline \
cl_ ## T ## 2 sign(const cl_ ## T ## 2 &v) \
{ \
    constexpr cl_ ## T zero = (cl_ ## T) 0; \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    return cl_ ## T ## 2{ \
        (v.s[0] < zero) ? -one : (v.s[0] > zero) ? one : zero, \
        (v.s[1] < zero) ? -one : (v.s[1] > zero) ? one : zero}; \
} \
 \
core_inline \
cl_ ## T ## 2 floor(const cl_ ## T ## 2 &v) \
{ \
    return cl_ ## T ## 2{ \
        std::floor(v.s[0]), \
        std::floor(v.s[1])}; \
} \
 \
core_inline \
cl_ ## T ## 2 round(const cl_ ## T ## 2 &v) \
{ \
    return cl_ ## T ## 2{ \
        std::round(v.s[0]), \
        std::round(v.s[1])}; \
} \
 \
core_inline \
cl_ ## T ## 2 ceil(const cl_ ## T ## 2 &v) \
{ \
    return cl_ ## T ## 2{ \
        std::ceil(v.s[0]), \
        std::ceil(v.s[1])}; \
} \
 \
core_inline \
cl_ ## T ## 2 clamp(const cl_ ## T ## 2 &v, const cl_ ## T lo, const cl_ ## T hi) \
{ \
    return cl_ ## T ## 2{ \
        std::min(std::max(v.s[0], lo), hi), \
        std::min(std::max(v.s[1], lo), hi)}; \
} \
 \
core_inline \
cl_ ## T ## 2 lerp(const cl_ ## T ## 2 &lo, const cl_ ## T ## 2 &hi, const cl_ ## T alpha) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    return cl_ ## T ## 2{ \
        (one - alpha) * lo.s[0] + alpha * hi.s[0], \
        (one - alpha) * lo.s[1] + alpha * hi.s[1]}; \
}

ATTO_CL_VEC2_ARITHMETIC(float)
ATTO_CL_VEC2_ARITHMETIC(double)

/** ---------------------------------------------------------------------------
 * @brief OpenCL cl_vec2 algebra functions.
 */
#define ATTO_CL_VEC2_ALGEBRA(T) \
core_inline \
cl_ ## T dot(const cl_ ## T ## 2 &a, const cl_ ## T ## 2 &b) \
{ \
    return (a.s[0] * b.s[0] + \
            a.s[1] * b.s[1]); \
} \
 \
core_inline \
cl_ ## T norm(const cl_ ## T ## 2 &a) \
{ \
    return std::sqrt(dot(a,a)); \
} \
 \
core_inline \
cl_ ## T ## 2 normalize(const cl_ ## T ## 2 &a) \
{ \
    return (a / norm(a)); \
} \
 \
core_inline \
cl_ ## T distance(const cl_ ## T ## 2 &a, const cl_ ## T ## 2 &b) \
{ \
    return norm(a-b); \
} \

ATTO_CL_VEC2_ALGEBRA(float)
ATTO_CL_VEC2_ALGEBRA(double)

/** ---------------------------------------------------------------------------
 * @brief OpenCL cl_vec4 arithmetic functions.
 */
#define ATTO_CL_VEC4_ARITHMETIC(T) \
core_inline \
cl_ ## T ## 4 abs(const cl_ ## T ## 4 &v) \
{ \
    return cl_ ## T ## 4{ \
        std::abs(v.s[0]), \
        std::abs(v.s[1]), \
        std::abs(v.s[2]), \
        std::abs(v.s[3])}; \
} \
 \
core_inline \
cl_ ## T ## 4 sign(const cl_ ## T ## 4 &v) \
{ \
    constexpr cl_ ## T zero = (cl_ ## T) 0; \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    return cl_ ## T ## 4{ \
        (v.s[0] < zero) ? -one : (v.s[0] > zero) ? one : zero, \
        (v.s[1] < zero) ? -one : (v.s[1] > zero) ? one : zero, \
        (v.s[2] < zero) ? -one : (v.s[2] > zero) ? one : zero, \
        (v.s[3] < zero) ? -one : (v.s[3] > zero) ? one : zero}; \
} \
 \
core_inline \
cl_ ## T ## 4 floor(const cl_ ## T ## 4 &v) \
{ \
    return cl_ ## T ## 4{ \
        std::floor(v.s[0]), \
        std::floor(v.s[1]), \
        std::floor(v.s[2]), \
        std::floor(v.s[3])}; \
} \
 \
core_inline \
cl_ ## T ## 4 round(const cl_ ## T ## 4 &v) \
{ \
    return cl_ ## T ## 4{ \
        std::round(v.s[0]), \
        std::round(v.s[1]), \
        std::round(v.s[2]), \
        std::round(v.s[3])}; \
} \
 \
core_inline \
cl_ ## T ## 4 ceil(const cl_ ## T ## 4 &v) \
{ \
    return cl_ ## T ## 4{ \
        std::ceil(v.s[0]), \
        std::ceil(v.s[1]), \
        std::ceil(v.s[2]), \
        std::ceil(v.s[3])}; \
} \
 \
core_inline \
cl_ ## T ## 4 clamp(const cl_ ## T ## 4 &v, const cl_ ## T lo, const cl_ ## T hi) \
{ \
    return cl_ ## T ## 4{ \
        std::min(std::max(v.s[0], lo), hi), \
        std::min(std::max(v.s[1], lo), hi), \
        std::min(std::max(v.s[2], lo), hi), \
        std::min(std::max(v.s[3], lo), hi)}; \
} \
 \
core_inline \
cl_ ## T ## 4 lerp(const cl_ ## T ## 4 &lo, const cl_ ## T ## 4 &hi, const cl_ ## T alpha) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    return cl_ ## T ## 4{ \
        (one - alpha) * lo.s[0] + alpha * hi.s[0], \
        (one - alpha) * lo.s[1] + alpha * hi.s[1], \
        (one - alpha) * lo.s[2] + alpha * hi.s[2], \
        (one - alpha) * lo.s[3] + alpha * hi.s[3]}; \
}

ATTO_CL_VEC4_ARITHMETIC(float)
ATTO_CL_VEC4_ARITHMETIC(double)

/** ---------------------------------------------------------------------------
 * @brief OpenCL cl_vec4 algebra functions.
 */
#define ATTO_CL_VEC4_ALGEBRA(T) \
core_inline \
cl_ ## T dot(const cl_ ## T ## 4 &a, const cl_ ## T ## 4 &b) \
{ \
    return (a.s[0] * b.s[0] + \
            a.s[1] * b.s[1] + \
            a.s[2] * b.s[2] + \
            a.s[3] * b.s[3]); \
} \
 \
core_inline \
cl_ ## T norm(const cl_ ## T ## 4 &a) \
{ \
    return std::sqrt(dot(a,a)); \
} \
 \
core_inline \
cl_ ## T ## 4 normalize(const cl_ ## T ## 4 &a) \
{ \
    return (a / norm(a)); \
} \
 \
core_inline \
cl_ ## T distance(const cl_ ## T ## 4 &a, const cl_ ## T ## 4 &b) \
{ \
    return norm(a-b); \
} \

ATTO_CL_VEC4_ALGEBRA(float)
ATTO_CL_VEC4_ALGEBRA(double)

/** ---------------------------------------------------------------------------
 * @brief OpenCL cl_vec8 arithmetic functions.
 */
#define ATTO_CL_VEC8_ARITHMETIC(T) \
core_inline \
cl_ ## T ## 8 abs(const cl_ ## T ## 8 &v) \
{ \
    return cl_ ## T ## 8{ \
        std::abs(v.s[0]), \
        std::abs(v.s[1]), \
        std::abs(v.s[2]), \
        std::abs(v.s[3]), \
        std::abs(v.s[4]), \
        std::abs(v.s[5]), \
        std::abs(v.s[6]), \
        std::abs(v.s[7])}; \
} \
 \
core_inline \
cl_ ## T ## 8 sign(const cl_ ## T ## 8 &v) \
{ \
    constexpr cl_ ## T zero = (cl_ ## T) 0; \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    return cl_ ## T ## 8{ \
        (v.s[0] < zero) ? -one : (v.s[0] > zero) ? one : zero, \
        (v.s[1] < zero) ? -one : (v.s[1] > zero) ? one : zero, \
        (v.s[2] < zero) ? -one : (v.s[2] > zero) ? one : zero, \
        (v.s[3] < zero) ? -one : (v.s[3] > zero) ? one : zero, \
        (v.s[4] < zero) ? -one : (v.s[4] > zero) ? one : zero, \
        (v.s[5] < zero) ? -one : (v.s[5] > zero) ? one : zero, \
        (v.s[6] < zero) ? -one : (v.s[6] > zero) ? one : zero, \
        (v.s[7] < zero) ? -one : (v.s[7] > zero) ? one : zero}; \
} \
 \
core_inline \
cl_ ## T ## 8 floor(const cl_ ## T ## 8 &v) \
{ \
    return cl_ ## T ## 8{ \
        std::floor(v.s[0]), \
        std::floor(v.s[1]), \
        std::floor(v.s[2]), \
        std::floor(v.s[3]), \
        std::floor(v.s[4]), \
        std::floor(v.s[5]), \
        std::floor(v.s[6]), \
        std::floor(v.s[7])}; \
} \
 \
core_inline \
cl_ ## T ## 8 round(const cl_ ## T ## 8 &v) \
{ \
    return cl_ ## T ## 8{ \
        std::round(v.s[0]), \
        std::round(v.s[1]), \
        std::round(v.s[2]), \
        std::round(v.s[3]), \
        std::round(v.s[4]), \
        std::round(v.s[5]), \
        std::round(v.s[6]), \
        std::round(v.s[7])}; \
} \
 \
core_inline \
cl_ ## T ## 8 ceil(const cl_ ## T ## 8 &v) \
{ \
    return cl_ ## T ## 8{ \
        std::ceil(v.s[0]), \
        std::ceil(v.s[1]), \
        std::ceil(v.s[2]), \
        std::ceil(v.s[3]), \
        std::ceil(v.s[4]), \
        std::ceil(v.s[5]), \
        std::ceil(v.s[6]), \
        std::ceil(v.s[7])}; \
} \
 \
core_inline \
cl_ ## T ## 8 clamp(const cl_ ## T ## 8 &v, const cl_ ## T lo, const cl_ ## T hi) \
{ \
    return cl_ ## T ## 8{ \
        std::min(std::max(v.s[0], lo), hi), \
        std::min(std::max(v.s[1], lo), hi), \
        std::min(std::max(v.s[2], lo), hi), \
        std::min(std::max(v.s[3], lo), hi), \
        std::min(std::max(v.s[4], lo), hi), \
        std::min(std::max(v.s[5], lo), hi), \
        std::min(std::max(v.s[6], lo), hi), \
        std::min(std::max(v.s[7], lo), hi)}; \
} \
 \
core_inline \
cl_ ## T ## 8 lerp(const cl_ ## T ## 8 &lo, const cl_ ## T ## 8 &hi, const cl_ ## T alpha) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    return cl_ ## T ## 8{ \
        (one - alpha) * lo.s[0] + alpha * hi.s[0], \
        (one - alpha) * lo.s[1] + alpha * hi.s[1], \
        (one - alpha) * lo.s[2] + alpha * hi.s[2], \
        (one - alpha) * lo.s[3] + alpha * hi.s[3], \
        (one - alpha) * lo.s[4] + alpha * hi.s[4], \
        (one - alpha) * lo.s[5] + alpha * hi.s[5], \
        (one - alpha) * lo.s[6] + alpha * hi.s[6], \
        (one - alpha) * lo.s[7] + alpha * hi.s[7]}; \
}

ATTO_CL_VEC8_ARITHMETIC(float)
ATTO_CL_VEC8_ARITHMETIC(double)

/** ---------------------------------------------------------------------------
 * @brief OpenCL cl_vec8 algebra functions.
 */
#define ATTO_CL_VEC8_ALGEBRA(T) \
core_inline \
cl_ ## T dot(const cl_ ## T ## 8 &a, const cl_ ## T ## 8 &b) \
{ \
    return (a.s[0] * b.s[0] + \
            a.s[1] * b.s[1] + \
            a.s[2] * b.s[2] + \
            a.s[3] * b.s[3] + \
            a.s[4] * b.s[4] + \
            a.s[5] * b.s[5] + \
            a.s[6] * b.s[6] + \
            a.s[7] * b.s[7]); \
} \
 \
core_inline \
cl_ ## T norm(const cl_ ## T ## 8 &a) \
{ \
    return std::sqrt(dot(a,a)); \
} \
 \
core_inline \
cl_ ## T ## 8 normalize(const cl_ ## T ## 8 &a) \
{ \
    return (a / norm(a)); \
} \
 \
core_inline \
cl_ ## T distance(const cl_ ## T ## 8 &a, const cl_ ## T ## 8 &b) \
{ \
    return norm(a-b); \
} \

ATTO_CL_VEC8_ALGEBRA(float)
ATTO_CL_VEC8_ALGEBRA(double)

/** ---------------------------------------------------------------------------
 * @brief OpenCL cl_vec16 arithmetic functions.
 */
#define ATTO_CL_VEC16_ARITHMETIC(T) \
core_inline \
cl_ ## T ## 16 abs(const cl_ ## T ## 16 &v) \
{ \
    return cl_ ## T ## 16{ \
        std::abs(v.s[0]), \
        std::abs(v.s[1]), \
        std::abs(v.s[2]), \
        std::abs(v.s[3]), \
        std::abs(v.s[4]), \
        std::abs(v.s[5]), \
        std::abs(v.s[6]), \
        std::abs(v.s[7]), \
        std::abs(v.s[8]), \
        std::abs(v.s[9]), \
        std::abs(v.s[10]), \
        std::abs(v.s[11]), \
        std::abs(v.s[12]), \
        std::abs(v.s[13]), \
        std::abs(v.s[14]), \
        std::abs(v.s[15])}; \
} \
 \
core_inline \
cl_ ## T ## 16 sign(const cl_ ## T ## 16 &v) \
{ \
    constexpr cl_ ## T zero = (cl_ ## T) 0; \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    return cl_ ## T ## 16{ \
        (v.s[0] < zero) ? -one : (v.s[0] > zero) ? one : zero, \
        (v.s[1] < zero) ? -one : (v.s[1] > zero) ? one : zero, \
        (v.s[2] < zero) ? -one : (v.s[2] > zero) ? one : zero, \
        (v.s[3] < zero) ? -one : (v.s[3] > zero) ? one : zero, \
        (v.s[4] < zero) ? -one : (v.s[4] > zero) ? one : zero, \
        (v.s[5] < zero) ? -one : (v.s[5] > zero) ? one : zero, \
        (v.s[6] < zero) ? -one : (v.s[6] > zero) ? one : zero, \
        (v.s[7] < zero) ? -one : (v.s[7] > zero) ? one : zero, \
        (v.s[8] < zero) ? -one : (v.s[8] > zero) ? one : zero, \
        (v.s[9] < zero) ? -one : (v.s[9] > zero) ? one : zero, \
        (v.s[10] < zero) ? -one : (v.s[10] > zero) ? one : zero, \
        (v.s[11] < zero) ? -one : (v.s[11] > zero) ? one : zero, \
        (v.s[12] < zero) ? -one : (v.s[12] > zero) ? one : zero, \
        (v.s[13] < zero) ? -one : (v.s[13] > zero) ? one : zero, \
        (v.s[14] < zero) ? -one : (v.s[14] > zero) ? one : zero, \
        (v.s[15] < zero) ? -one : (v.s[15] > zero) ? one : zero}; \
} \
 \
core_inline \
cl_ ## T ## 16 floor(const cl_ ## T ## 16 &v) \
{ \
    return cl_ ## T ## 16{ \
        std::floor(v.s[0]), \
        std::floor(v.s[1]), \
        std::floor(v.s[2]), \
        std::floor(v.s[3]), \
        std::floor(v.s[4]), \
        std::floor(v.s[5]), \
        std::floor(v.s[6]), \
        std::floor(v.s[7]), \
        std::floor(v.s[8]), \
        std::floor(v.s[9]), \
        std::floor(v.s[10]), \
        std::floor(v.s[11]), \
        std::floor(v.s[12]), \
        std::floor(v.s[13]), \
        std::floor(v.s[14]), \
        std::floor(v.s[15])}; \
} \
 \
core_inline \
cl_ ## T ## 16 round(const cl_ ## T ## 16 &v) \
{ \
    return cl_ ## T ## 16{ \
        std::round(v.s[0]), \
        std::round(v.s[1]), \
        std::round(v.s[2]), \
        std::round(v.s[3]), \
        std::round(v.s[4]), \
        std::round(v.s[5]), \
        std::round(v.s[6]), \
        std::round(v.s[7]), \
        std::round(v.s[8]), \
        std::round(v.s[9]), \
        std::round(v.s[10]), \
        std::round(v.s[11]), \
        std::round(v.s[12]), \
        std::round(v.s[13]), \
        std::round(v.s[14]), \
        std::round(v.s[15])}; \
} \
 \
core_inline \
cl_ ## T ## 16 ceil(const cl_ ## T ## 16 &v) \
{ \
    return cl_ ## T ## 16{ \
        std::ceil(v.s[0]), \
        std::ceil(v.s[1]), \
        std::ceil(v.s[2]), \
        std::ceil(v.s[3]), \
        std::ceil(v.s[4]), \
        std::ceil(v.s[5]), \
        std::ceil(v.s[6]), \
        std::ceil(v.s[7]), \
        std::ceil(v.s[8]), \
        std::ceil(v.s[9]), \
        std::ceil(v.s[10]), \
        std::ceil(v.s[11]), \
        std::ceil(v.s[12]), \
        std::ceil(v.s[13]), \
        std::ceil(v.s[14]), \
        std::ceil(v.s[15])}; \
} \
 \
core_inline \
cl_ ## T ## 16 clamp(const cl_ ## T ## 16 &v, const cl_ ## T lo, const cl_ ## T hi) \
{ \
    return cl_ ## T ## 16{ \
        std::min(std::max(v.s[0], lo), hi), \
        std::min(std::max(v.s[1], lo), hi), \
        std::min(std::max(v.s[2], lo), hi), \
        std::min(std::max(v.s[3], lo), hi), \
        std::min(std::max(v.s[4], lo), hi), \
        std::min(std::max(v.s[5], lo), hi), \
        std::min(std::max(v.s[6], lo), hi), \
        std::min(std::max(v.s[7], lo), hi), \
        std::min(std::max(v.s[8], lo), hi), \
        std::min(std::max(v.s[9], lo), hi), \
        std::min(std::max(v.s[10], lo), hi), \
        std::min(std::max(v.s[11], lo), hi), \
        std::min(std::max(v.s[12], lo), hi), \
        std::min(std::max(v.s[13], lo), hi), \
        std::min(std::max(v.s[14], lo), hi), \
        std::min(std::max(v.s[15], lo), hi)}; \
} \
 \
core_inline \
cl_ ## T ## 16 lerp(const cl_ ## T ## 16 &lo, const cl_ ## T ## 16 &hi, const cl_ ## T alpha) \
{ \
    constexpr cl_ ## T one = (cl_ ## T) 1; \
    return cl_ ## T ## 16{ \
        (one - alpha) * lo.s[0] + alpha * hi.s[0], \
        (one - alpha) * lo.s[1] + alpha * hi.s[1], \
        (one - alpha) * lo.s[2] + alpha * hi.s[2], \
        (one - alpha) * lo.s[3] + alpha * hi.s[3], \
        (one - alpha) * lo.s[4] + alpha * hi.s[4], \
        (one - alpha) * lo.s[5] + alpha * hi.s[5], \
        (one - alpha) * lo.s[6] + alpha * hi.s[6], \
        (one - alpha) * lo.s[7] + alpha * hi.s[7], \
        (one - alpha) * lo.s[8] + alpha * hi.s[8], \
        (one - alpha) * lo.s[9] + alpha * hi.s[9], \
        (one - alpha) * lo.s[10] + alpha * hi.s[10], \
        (one - alpha) * lo.s[11] + alpha * hi.s[11], \
        (one - alpha) * lo.s[12] + alpha * hi.s[12], \
        (one - alpha) * lo.s[13] + alpha * hi.s[13], \
        (one - alpha) * lo.s[14] + alpha * hi.s[14], \
        (one - alpha) * lo.s[15] + alpha * hi.s[15]}; \
}

ATTO_CL_VEC16_ARITHMETIC(float)
ATTO_CL_VEC16_ARITHMETIC(double)

/** ---------------------------------------------------------------------------
 * @brief OpenCL cl_vec16 algebra functions.
 */
#define ATTO_CL_VEC16_ALGEBRA(T) \
core_inline \
cl_ ## T dot(const cl_ ## T ## 16 &a, const cl_ ## T ## 16 &b) \
{ \
    return (a.s[0] * b.s[0] + \
            a.s[1] * b.s[1] + \
            a.s[2] * b.s[2] + \
            a.s[3] * b.s[3] + \
            a.s[4] * b.s[4] + \
            a.s[5] * b.s[5] + \
            a.s[6] * b.s[6] + \
            a.s[7] * b.s[7] + \
            a.s[8] * b.s[8] + \
            a.s[9] * b.s[9] + \
            a.s[10] * b.s[10] + \
            a.s[11] * b.s[11] + \
            a.s[12] * b.s[12] + \
            a.s[13] * b.s[13] + \
            a.s[14] * b.s[14] + \
            a.s[15] * b.s[15]); \
} \
 \
core_inline \
cl_ ## T norm(const cl_ ## T ## 16 &a) \
{ \
    return std::sqrt(dot(a,a)); \
} \
 \
core_inline \
cl_ ## T ## 16 normalize(const cl_ ## T ## 16 &a) \
{ \
    return (a / norm(a)); \
} \
 \
core_inline \
cl_ ## T distance(const cl_ ## T ## 16 &a, const cl_ ## T ## 16 &b) \
{ \
    return norm(a-b); \
} \

ATTO_CL_VEC16_ALGEBRA(float)
ATTO_CL_VEC16_ALGEBRA(double)

} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_MATH_H_ */
