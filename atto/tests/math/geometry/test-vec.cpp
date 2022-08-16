/*
 * test-vec.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "Catch2/catch.hpp"

#include "test-vec2.hpp"
#include "test-vec3.hpp"
#include "test-vec4.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * test_vec
 * Vec<Type> test client.
 */
TEST_CASE("Vec<Type>") {
    /**
     * Vec2 test clients.
     */
    SECTION("Vec2") {
        test_vec2<int16_t>();
        test_vec2<int32_t>();
        test_vec2<int64_t>();
        test_vec2<uint16_t>();
        test_vec2<uint32_t>();
        test_vec2<uint64_t>();
        test_vec2<float>();
        test_vec2<double>();
    }

    /**
     * Vec3 test clients.
     */
    SECTION("Vec3") {
        test_vec3<int16_t>();
        test_vec3<int32_t>();
        test_vec3<int64_t>();
        test_vec3<uint16_t>();
        test_vec3<uint32_t>();
        test_vec3<uint64_t>();
        test_vec3<float>();
        test_vec3<double>();
    }

    /**
     * Vec4 test clients.
     */
    SECTION("Vec4") {
        test_vec4<int16_t>();
        test_vec4<int32_t>();
        test_vec4<int64_t>();
        test_vec4<uint16_t>();
        test_vec4<uint32_t>();
        test_vec4<uint64_t>();
        test_vec4<float>();
        test_vec4<double>();
    }
}