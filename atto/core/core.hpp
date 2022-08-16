/*
 * core.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_CORE_H
#define ATTO_CORE_H

/**
 * @brief Define ATTO_MATH_SIMD to enable SIMD based math.
 */
#define ATTO_MATH_SIMD

/**
 * @brief C headers
 *    #include <cstdlib>
 *    #include <cstddef>
 *    #include <cstdio>
 *    #include <cstring>
 *    #include <cstdint>
 *    #include <cstdbool>
 *    #include <cmath>
 *    #include <cfloat>
 *    #include <climits>
 *    #include <cctype>
 *    #include <ctime>
 *    #include <cerrno>
 *    #include <cassert>
 * @brief C++ headers
 *    #include <iostream>
 *    #include <fstream>
 *    #include <sstream>
 *    #include <iomanip>
 *    #include <string>
 *    #include <memory>
 *    #include <typeinfo>
 *    #include <typeindex>
 *    #include <type_traits>
 *    #include <exception>
 *    #include <chrono>
 * @brief C++ containers
 *    #include <array>
 *    #include <vector>
 *    #include <list>
 *    #include <deque>
 *    #include <stack>
 *    #include <queue>
 *    #include <set>
 *    #include <map>
 *    #include <unordered_map>
 *    #include <unordered_set>
 * @brief C++ numerics
 *    #include <functional>
 *    #include <algorithm>
 *    #include <numeric>
 *    #include <random>
 *    #include <bitset>
 *    #include <limits>
 *
 * @brief OpenGL/GLFW headers using GLEW loader.
 * @note Define GLFW_INCLUDE_NONE before the GLFW header to explicitly
 * disable inclusion of the OpenGL environment header and use the header
 * provided by glad.
 * @see https://www.glfw.org/docs/3.3
 *    #define GLFW_INCLUDE_NONE
 *    #include <glad/glad.h>
 *    #include <GLFW/glfw3.h>
 *
 * @brief OpenGL/GLFW headers using GLAD loader.
 * @note Make sure to include the GLEW header before the GLFW one.
 * The GLEW header defines macros that disable any OpenGL header
 * that the GLFW header includes and GLEW will work as expected.
 * @see https://www.glfw.org/docs/3.3
 *    #define GLEW_STATIC
 *    #include <GL/glew.h>
 *    #include <GLFW/glfw3.h>
 *    #define GLFW_INCLUDE_NONE
 *    #include <glad/glad.h>
 *    #include <GLFW/glfw3.h>
 *
 * @brief GLM linear algebra.
 *    #define GLM_FORCE_CXX11
 *    #define GLM_FORCE_AVX
 *    #define GLM_FORCE_INLINE
 *    #include <glm/glm.hpp>
 *    #include <glm/gtc/type_ptr.hpp>
 *    #include <glm/gtc/matrix_transform.hpp>
 *    #include <glm/gtx/transform.hpp>
 *    #include <glm/gtx/rotate_vector.hpp>
 */

/**
 * Stringify macros
 */
#define core_strify(a)          #a
#define core_strify_x(a)        core_strify(a)

#define core_concat2(a,b)       a ## b
#define core_concat2_x(a,b)     core_concat2(a,b)

#define core_concat3(a,b,c)     a ## b ## c
#define core_concat3_x(a,b,c)   core_concat3(a,b,c)

#define core_concat4(a,b,c,d)   a ## b ## c ## d
#define core_concat4_x(a,b,c,d) core_concat4(a,b,c,d)

/**
 * @brief Define the SIMD AVX instruction set according to the __AVX__ macro,
 * set by mavx compiler option.
 * SSE/AVX instructions operate on packed integers/floats/doubles.
 * SSE vector registers are 128 bit wide (16 bytes), and able to pack 4 integers
 * (4 byte integer) or 2 doubles (8 byte double).
 * AVX vector registers are 256 bit wide (32 bytes), and able to pack 8 integers
 * (4 byte integer) or 4 doubles (8 byte double).
 *
 * Data is transferred in chuncks from/to vector registers. Data transfer is
 * more efficient if the memory addresses are a multiple of 16 bytes (SSE) or
 * 32 bytes(AVX). A boundary multiple of 16/32 bytes is 16/32 byte aligned.
 *
 * Alternatively, fail with the error macro:
 *  #error AVX extended instruction set required.
 */
#ifdef ATTO_MATH_SIMD
#ifdef __AVX__
#include <x86intrin.h>
#else
#error AVX extended instruction set required.
#endif  /* __AVX__ */
#endif  /* ATTO_MATH_SIMD */

/**
 * POSIX standard symbolic constants, types and threads
 */
// #include <unistd.h>
// #include <pthread.h>

/**
 * OpenMP headers and macros
 * These functions are declared in the omp.h header file and have well defined
 * meanings in a non parallel environment. If _OPENMP is not defined, fallback
 * and elide the macro.
 */
#ifdef _OPENMP
#include <omp.h>
#define core_pragma_omp(a)      _Pragma(core_strify(omp a))
#else   /* _OPENMP */
#define core_pragma_omp(a)
inline int omp_get_level(void) { return 0; }
inline int omp_get_thread_num(void) { return 0; }
inline int omp_get_num_threads(void) { return 1; }
inline int omp_get_max_threads(void) { return 1; }
#endif  /* _OPENMP */

/**
 * GNU C extensions
 * @brief Convenience GNU-C extension macros. Define the following extensions
 * if we're using GNUC. Otherwise, fallback and elide the corresponding macro.
 * Alternatively, fail with:
 *  #error GNU C extensions required.
 */
#ifdef __GNUC__
#define core_inline             inline __attribute__((always_inline))
#define core_aligned(key)       __attribute__((aligned(key)))
#define core_unused(decl)       __attribute__((unused)) decl
#define core_extension(decl)    __extension__ decl
#define core_restrict(decl)     __restrict__ decl
#else
#define core_inline             inline
#define core_aligned(key)
#define core_unused(decl)       decl
#define core_extension(decl)    decl
#define core_restrict(decl)     decl
#endif /* __GNUC__ */

/**
 * Core interface.
 */
#include "atto/core/error.hpp"
#include "atto/core/memory.hpp"
#include "atto/core/string.hpp"
#include "atto/core/file.hpp"

#endif /* ATTO_CORE_H */
