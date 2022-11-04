/*
 * test-performance-vec4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_PERFORMANCE_VEC4_H_
#define TEST_CORE_GEOMETRY_PERFORMANCE_VEC4_H_

#include "../common.hpp"

/**
 * test_performance_vec4
 * @brief vec4d performance test.
 */
core_inline
void test_performance_vec4 (const size_t n_items, const double maxeps)
{
    using namespace atto;

    const size_t dim = 4;
    const math::vec4d zero(0.0);
    const math::vec4d one(1.0);
    const math::vec4d two(2.0);

    /*
     * Create an array of vectors
     */
    std::vector<math::vec4d> arr_a(n_items, one);
    std::vector<math::vec4d> arr_b(n_items, two);
    std::vector<math::vec4d> arr_c(n_items, zero);

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
     * Test vector operators
     */
    core_pragma_omp(parallel for \
        default(none) \
        shared(n_items, arr_a, arr_b, arr_c, engine) \
        schedule(static))
    for (size_t ix = 0; ix < n_items; ++ix) {
        size_t tid = omp_get_thread_num();
        math::rng::gauss<double> rand;

        arr_a[ix] = math::vec4d(
            rand(engine[tid], rand_sdev, rand_avg),
            rand(engine[tid], rand_sdev, rand_avg),
            rand(engine[tid], rand_sdev, rand_avg),
            rand(engine[tid], rand_sdev, rand_avg));
        arr_b[ix] = arr_a[ix] * (-1.0);

        /*
         * Test arithmetic functions
         */
        arr_c[ix] = arr_a[ix] + arr_b[ix];      /* 1 + (-1) = 0 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(math::isequal(arr_c[ix](j), 0.0));
        }

        arr_c[ix] = arr_a[ix] - arr_b[ix];      /* 1 - (-1) = 2 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(math::isequal(arr_c[ix](j), 2*arr_a[ix](j)));
        }

        /*
         * Test arithmetic assignment functions
         */
        (arr_c[ix] = arr_a[ix]) += arr_b[ix];   /* 1 + (-1) = 0 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(math::isequal(arr_c[ix](j), 0.0));
        }

        (arr_c[ix] = arr_a[ix]) -= arr_b[ix];   /* 1 - (-1) = 2 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(math::isequal(arr_c[ix](j), 2*arr_a[ix](j)));
        }

        /* Test dot, norm and normalize */
        arr_c[ix] = arr_a[ix] - arr_b[ix];

        REQUIRE(math::isequal(
            dot(arr_c[ix], arr_c[ix]), 4.0*dot(arr_a[ix], arr_a[ix])));
        REQUIRE(math::isequal(
            norm(arr_c[ix]), 2.0*std::sqrt(dot(arr_a[ix], arr_a[ix]))));
        REQUIRE(math::isequal(norm(normalize(arr_c[ix])), 1.0));
    }
}

#endif /* TEST_CORE_GEOMETRY_PERFORMANCE_VEC4_H_ */
