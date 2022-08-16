/*
 * test-performance.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "Catch2/catch.hpp"

#include "test-performance-vec2.hpp"
#include "test-performance-vec3.hpp"
#include "test-performance-vec4.hpp"
#include "test-performance-mat2.hpp"
#include "test-performance-mat3.hpp"
#include "test-performance-mat4.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * test_performance.
 * @brief Vec performance test client.
 */
TEST_CASE("Performance") {
    /*
     * Use a maxeps given by the sqrt of machine epsilon.
     */
    const double maxeps = std::sqrt(std::numeric_limits<float>::epsilon());
    const size_t n_iterations = 8;
    const size_t n_items = 131072;

    SECTION("Performance-vec2") {
        auto tic = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < n_iterations; ++i) {
            test_performance_vec2(n_items, maxeps);
        }
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::cout << msec.count() << " msec\n";
    }

    SECTION("Performance-vec3") {
        auto tic = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < n_iterations; ++i) {
            test_performance_vec3(n_items, maxeps);
        }
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::cout << msec.count() << " msec\n";
    }

    SECTION("Performance-vec4") {
        auto tic = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < n_iterations; ++i) {
            test_performance_vec4(n_items, maxeps);
        }
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::cout << msec.count() << " msec\n";
    }

    SECTION("Performance mat2") {
        auto tic = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < n_iterations; ++i) {
            test_performance_mat2(n_items, maxeps);
        }
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::cout << msec.count() << " msec\n";
    }

    SECTION("Performance mat3") {
        auto tic = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < n_iterations; ++i) {
            test_performance_mat3(n_items, maxeps);
        }
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::cout << msec.count() << " msec\n";
    }

    SECTION("Performance mat4") {
        auto tic = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < n_iterations; ++i) {
            test_performance_mat4(n_items, maxeps);
        }
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::cout << msec.count() << " msec\n";
    }
}
