/*
 * geometry.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_GEOMETRY_H_
#define ATTO_MATH_GEOMETRY_H_

/** ---- Geometry objects -----------------------------------------------------
 * @brief Vector Data representation:
 * - The vector components have a minimum memory aligment specified by
 * the macro core_aligned and are interpreted as follows:
 *
 * A 1x2 vector is interpreted as one 128-bit (2x64) memory references.
 * A 1x3 vector is interpreted as one 256-bit (4x64) memory references
 * with the last 64-bit double padded to zero.
 * A 1x4 vector is interpreted as one 256-bit (4x64) memory references.
 *
 * - SIMD instructions that use 128-bit memory references require the
 * address to be 16-byte aligned, and SIMD instructions that use 256-bit
 * memory references require the address to be 32-byte aligned.
 *
 * - A n-byte aligned memory address (a) means the address of (a) is a
 * multiple of (n) bytes. A multiple of 32 is also a multiple of 16,
 * therefore 2x2 matrices are set to be 32-byte aligned to allow
 * consistency between SSE and AVX instructions.
 *
 * @brief Matrix Data representation:
 * - The matrix components have a minimum memory aligment specified by
 * the macro core_aligned and are interpreted as follows:

 * A 2x2 matrix is interpreted as two 128-bit (2x64) memory references.
 * A 3x3 matrix is interpreted as three 256-bit (4x64) memory references
 * with the last 64-bit double padded to zero.
 * A 4x4 matrix is interpreted as four 256-bit (4x64) memory references.
 *
 * - SIMD instructions that use 128-bit memory references require the
 * address to be 16-byte aligned, and SIMD instructions that use 256-bit
 * memory references require the address to be 32-byte aligned.
 *
 * - A n-byte aligned memory address (a) means the address of (a) is a
 * multiple of (n) bytes. A multiple of 32 is also a multiple of 16,
 * therefore 2x2 matrices are set to be 32-byte aligned to allow
 * consistency between SSE and AVX instructions.
 *
 * @brief Arithmetic operators:
 * - All arithmetic operations between vectors or matrices are elementwise.
 * Binary arithmetic operators are defined as non-member functions with
 * const references to the corresponding operands.
 *
 * @brief Algebraic operators:
 * - All algebraic operators are non member functions operating on const
 * references to the operands.
 * - Not all geometric operations exist for every vector dimension, e.g.,
 * vector cross product is only meaningful for 3d-vectors.
 * - Not all geometric operations exist for every matrix dimension, e.g.,
 * orthographic and perspective projection operations are only meaningful
 * for 4d-matrices.
 *
 * @brief Matrix multiplication:
 * - Matrix multiplication is a type of operation where this matrix acts
 * as a linear projection function object on its operand (vector or matrix).
 * In both cases (vector or matrix) the modified object is the operand
 * and not the calling matrix object which acts as function operator.
 *
 * @see
 * https://stackoverflow.com/questions/4421706
 * https://stackoverflow.com/questions/36955576
 * https://gamedev.stackexchange.com/questions/33142
 */
#include <cmath>
#include <limits>
#include <type_traits>

#include "atto/math/geometry/vec2.hpp"
#include "atto/math/geometry/vec3.hpp"
#include "atto/math/geometry/vec4.hpp"
#include "atto/math/geometry/mat2.hpp"
#include "atto/math/geometry/mat3.hpp"
#include "atto/math/geometry/mat4.hpp"
#include "atto/math/geometry/arithmetic.hpp"
#include "atto/math/geometry/algebra.hpp"
#include "atto/math/geometry/transform.hpp"
#include "atto/math/geometry/io.hpp"
#include "atto/math/geometry/ortho.hpp"

namespace atto {
namespace math {

/**
 * @brief Aligned data types.
 * Define aligned data types to get the attribute to apply to the pointed-to
 * type, rather than the pointer itself.
 * With GNU C __attribute__ syntax, it may be necessary use a typedef to get
 * the attribute to apply to the pointed-to type, rather than to the pointer
 * itself.
 * Set a 32-byte alignment as required by AVX instruction set.
 * @see https://stackoverflow.com/questions/36211864
 *      https://gcc.gnu.org/onlinedocs/gcc-6.5.0/gcc/Common-Type-Attributes.html
 */
typedef std::int8_t     aligned_int8_t     core_aligned(32);
typedef std::uint8_t    aligned_uint8_t    core_aligned(32);
typedef std::int16_t    aligned_int16_t    core_aligned(32);
typedef std::uint16_t   aligned_uint16_t   core_aligned(32);
typedef std::int32_t    aligned_int32_t    core_aligned(32);
typedef std::uint32_t   aligned_uint32_t   core_aligned(32);
typedef std::int64_t    aligned_int64_t    core_aligned(32);
typedef std::uint64_t   aligned_uint64_t   core_aligned(32);
typedef std::float_t    aligned_float_t    core_aligned(32);
typedef std::double_t   aligned_double_t   core_aligned(32);

/**
 * @brief Vector type definitions.
 */
typedef vec2<int16_t>   vec2i16;
typedef vec2<int32_t>   vec2i32;
typedef vec2<int64_t>   vec2i64;
typedef vec2<int32_t>   vec2i;
typedef vec2<uint16_t>  vec2u16;
typedef vec2<uint32_t>  vec2u32;
typedef vec2<uint64_t>  vec2u64;
typedef vec2<uint32_t>  vec2u;
typedef vec2<float>     vec2f;
typedef vec2<double>    vec2d;

typedef vec3<int16_t>   vec3i16;
typedef vec3<int32_t>   vec3i32;
typedef vec3<int64_t>   vec3i64;
typedef vec3<int32_t>   vec3i;
typedef vec3<uint16_t>  vec3u16;
typedef vec3<uint32_t>  vec3u32;
typedef vec3<uint64_t>  vec3u64;
typedef vec3<uint32_t>  vec3u;
typedef vec3<float>     vec3f;
typedef vec3<double>    vec3d;

typedef vec4<int16_t>   vec4i16;
typedef vec4<int32_t>   vec4i32;
typedef vec4<int64_t>   vec4i64;
typedef vec4<int32_t>   vec4i;
typedef vec4<uint16_t>  vec4u16;
typedef vec4<uint32_t>  vec4u32;
typedef vec4<uint64_t>  vec4u64;
typedef vec4<uint32_t>  vec4u;
typedef vec4<float>     vec4f;
typedef vec4<double>    vec4d;

/**
 * @brief Matrix type definitions.
 */
typedef mat2<int16_t>   mat2i16;
typedef mat2<int32_t>   mat2i32;
typedef mat2<int64_t>   mat2i64;
typedef mat2<int32_t>   mat2i;
typedef mat2<uint16_t>  mat2u16;
typedef mat2<uint32_t>  mat2u32;
typedef mat2<uint64_t>  mat2u64;
typedef mat2<uint32_t>  mat2u;
typedef mat2<float>     mat2f;
typedef mat2<double>    mat2d;

typedef mat3<int16_t>   mat3i16;
typedef mat3<int32_t>   mat3i32;
typedef mat3<int64_t>   mat3i64;
typedef mat3<int32_t>   mat3i;
typedef mat3<uint16_t>  mat3u16;
typedef mat3<uint32_t>  mat3u32;
typedef mat3<uint64_t>  mat3u64;
typedef mat3<uint32_t>  mat3u;
typedef mat3<float>     mat3f;
typedef mat3<double>    mat3d;

typedef mat4<int16_t>   mat4i16;
typedef mat4<int32_t>   mat4i32;
typedef mat4<int64_t>   mat4i64;
typedef mat4<int32_t>   mat4i;
typedef mat4<uint16_t>  mat4u16;
typedef mat4<uint32_t>  mat4u32;
typedef mat4<uint64_t>  mat4u64;
typedef mat4<uint32_t>  mat4u;
typedef mat4<float>     mat4f;
typedef mat4<double>    mat4d;

/**
 * @brief Ortho type definitions.
 */
typedef Ortho<float>    Orthof;
typedef Ortho<double>   Orthod;

} /* math */
} /* atto */

#endif /* ATTO_MATH_GEOMETRY_H_ */