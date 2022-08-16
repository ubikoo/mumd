/*
 * test-geometry.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "Catch2/catch.hpp"

#include "test-geometry2.hpp"
#include "test-geometry3.hpp"
#include "test-geometry4.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * test_geometry
 * @brief Vec geometry test client.
 */
TEST_CASE("Geometry") {
    SECTION("Geometry-vec2") {
        test_geometry2<float>();
        test_geometry2<double>();
    }

    SECTION("Geometry-vec3") {
        test_geometry3<float>();
        test_geometry3<double>();
    }

    SECTION("Geometry-vec4") {
        test_geometry4<float>();
        test_geometry4<double>();
    }
}
