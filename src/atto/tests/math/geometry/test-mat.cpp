/*
 * test-mat.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "Catch2/catch.hpp"

#include "test-mat2.hpp"
#include "test-mat3.hpp"
#include "test-mat4.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * test_mat
 * Mat<Type> test client.
 */
TEST_CASE("Mat<Type>") {
    /**
     * Mat2 test clients.
     */
    SECTION("Mat2") {
        test_mat2<int16_t>();
        test_mat2<int32_t>();
        test_mat2<int64_t>();
        test_mat2<uint16_t>();
        test_mat2<uint32_t>();
        test_mat2<uint64_t>();
        test_mat2<float>();
        test_mat2<double>();
    }

    /**
     * Mat3 test clients.
     */
    SECTION("Mat3") {
        test_mat3<int16_t>();
        test_mat3<int32_t>();
        test_mat3<int64_t>();
        test_mat3<uint16_t>();
        test_mat3<uint32_t>();
        test_mat3<uint64_t>();
        test_mat3<float>();
        test_mat3<double>();
    }

    /**
     * Mat4 test clients.
     */
    SECTION("Mat4") {
        test_mat4<int16_t>();
        test_mat4<int32_t>();
        test_mat4<int64_t>();
        test_mat4<uint16_t>();
        test_mat4<uint32_t>();
        test_mat4<uint64_t>();
        test_mat4<float>();
        test_mat4<double>();
    }
}