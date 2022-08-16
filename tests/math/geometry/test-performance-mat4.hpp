/*
 * test-performance-mat4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_PERFORMANCE_MAT4_H_
#define TEST_CORE_GEOMETRY_PERFORMANCE_MAT4_H_

#include "../common.hpp"

/**
 * test_performance_mat4
 * @brief mat4d performance test.
 */
core_inline
void test_performance_mat4 (const size_t n_items, const double maxeps)
{
    using namespace atto;

    const size_t dim = 4;
    const math::mat4d zero(0.0);
    const math::mat4d one(1.0);
    const math::mat4d two(2.0);

    /*
     * Create an array of matrixs
     */
    std::vector<math::mat4d> arr_a(n_items, one);
    std::vector<math::mat4d> arr_b(n_items, two);
    std::vector<math::mat4d> arr_c(n_items, zero);

    /*
     * Create an array of random number generators
     */
    std::vector<math::rng::Kiss> engine;
    core_pragma_omp(parallel default(none) shared(engine))
    {
        core_pragma_omp(master)
        {
            engine.resize(omp_get_num_threads());
            for (auto &e : engine) { e.init(); }
        }
    } /* omp parallel */

    /*
     * Test matrix operators
     */
    core_pragma_omp(parallel for \
        default(none) \
        shared(n_items, arr_a, arr_b, arr_c, engine) \
        schedule(static))
    for (size_t ix = 0; ix < n_items; ++ix) {
        size_t tid = omp_get_thread_num();
        math::rng::gauss<double> rand;

        arr_a[ix] = math::mat4d(
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0));
        arr_b[ix] = arr_a[ix] * (-1.0);

        /*
         * Test arithmetic functions
         */
        arr_c[ix] = arr_a[ix] + arr_b[ix];      /* 1 + (-1) = 0 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(math::isequal(arr_c[ix](j, k), 0.0));
            }
        }

        arr_c[ix] = arr_a[ix] - arr_b[ix];      /* 1 - (-1) = 2 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(math::isequal(arr_c[ix](j, k), 2*arr_a[ix](j, k)));
            }
        }

        /*
         * Test arithmetic assignment functions
         */
        (arr_c[ix] = arr_a[ix]) += arr_b[ix];   /* 1 + (-1) = 0 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(math::isequal(arr_c[ix](j, k), 0.0));
            }
        }

        (arr_c[ix] = arr_a[ix]) -= arr_b[ix];   /* 1 - (-1) = 2 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(math::isequal(arr_c[ix](j, k), 2*arr_a[ix](j, k)));
            }
        }

        /*
         * Test algebraic operators
         */
        arr_a[ix] = math::mat4d(
            rand(engine[tid], rand_sdev, rand_avg),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, rand_avg),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, rand_avg),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, rand_avg));

        /* Matrix transpose */
        arr_b[ix] = transpose(arr_a[ix]);       /* b = a^t */
        arr_c[ix] = dot(arr_a[ix], arr_b[ix]);  /* c = a * a^t */
        double det_a = determinant(arr_a[ix]);
        double det_b = determinant(arr_b[ix]);
        double det_c = determinant(arr_c[ix]);
        REQUIRE(math::isequal(det_a, det_b));
        REQUIRE(math::isequal(det_a*det_b, det_c));

        /* Matrix inverse */
        arr_b[ix] = inverse(arr_a[ix]);           /* b = a^(-1) */
        arr_c[ix] = dot(arr_a[ix], arr_b[ix]);  /* c = a * a^(-1) */

        math::mat4d ident = math::mat4d::eye;
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(math::isequal(arr_c[ix](j,k), ident(j,k)));
            }
        }

        /* Matrix solve */
        math::vec4d vec_b(
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0),
            rand(engine[tid], rand_sdev, 0.0));
        math::mat4d inv_a = inverse(arr_a[ix]);
        /*
         * x = a^-1 * arr_b[ix]); err = a * x - b
         */
        math::vec4d vec_x = inverse(arr_a[ix]) * vec_b;
        math::vec4d err = vec_b - (arr_a[ix] * vec_x);
        REQUIRE(math::isequal(norm(err), 0.0));
    }
}

#endif /* TEST_CORE_GEOMETRY_PERFORMANCE_MAT4_H_ */
