/*
 * test-ortho.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "Catch2/catch.hpp"

#include "test-ortho.hpp"
using namespace atto;

TEST_CASE("Ortho") {
    const size_t n_iters = 8;
    const size_t n_basis = 8388608;

    for (size_t i = 0; i < n_iters; ++i) {
        auto tic = std::chrono::high_resolution_clock::now();
        test_ortho<float>(n_basis);
        test_ortho<double>(n_basis);
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::cout << msec.count() << " msec\n";
    }
}
