/*
 * test-arithmetic2.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_ARITHMETIC2_H_
#define TEST_CORE_GEOMETRY_ARITHMETIC2_H_

#include "../common.hpp"

/**
 * test_arithmetic2<Type>
 * @brief vec2<Type> arithmetic test client.
 */
template<typename Type>
void test_arithmetic2 (void)
{
    using namespace atto;

    { /* test abs */
        math::vec2<Type> a, abs_a;

        a = math::vec2<Type>(-1,0);
        abs_a = math::abs(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(abs_a(i), std::fabs(a(i))));
        }

        a = math::vec2<Type>(-1,1);
        abs_a = abs(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(abs_a(i), std::fabs(a(i))));
        }
    }

    { /* test sign */
        math::vec2<Type> a, sign_a;

        a = math::vec2<Type>(-2,-3);
        sign_a = sign(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(sign_a(i), static_cast<Type>(-1)));
        }

        a = math::vec2<Type>(2,3);
        sign_a = sign(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(sign_a(i), static_cast<Type>(1)));
        }

        a = math::vec2<Type>(0,0);
        sign_a = sign(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(sign_a(i), static_cast<Type>(0)));
        }
    }

    { /* test floor */
        math::vec2<Type> a, floor_a;

        a = math::vec2<Type>(-0.8, -0.4);
        floor_a = floor(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(floor_a(i), std::floor(a(i))));
        }

        a = math::vec2<Type>(0.8, 0.4);
        floor_a = floor(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(floor_a(i), std::floor(a(i))));
        }
    }

    { /* test round */
        math::vec2<Type> a, round_a;

        a = math::vec2<Type>(-1.3, -1.6);
        round_a = round(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(round_a(i), std::round(a(i))));
        }

        a = math::vec2<Type>(1.3, 1.6);
        round_a = round(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(round_a(i), std::round(a(i))));
        }
    }

    { /* test ceil */
        math::vec2<Type> a, ceil_a;

        a = math::vec2<Type>(-1.3, -1.6);
        ceil_a = ceil(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(ceil_a(i), std::ceil(a(i))));
        }

        a = math::vec2<Type>(1.3, 1.6);
        ceil_a = ceil(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(ceil_a(i), std::ceil(a(i))));
        }
    }

    { /* test clamp */
        auto clamp_fun = [&] (
            const Type val,
            const Type lo,
            const Type hi) -> Type {
            return std::min(std::max(val,lo),hi);
        };

        math::vec2<Type> a, clamp_a;
        Type lo = -1.0;
        Type hi =  0.0;
        a = math::vec2<Type>(-1.5, -0.5);
        clamp_a = clamp(a, lo, hi);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(clamp_a(i), clamp_fun(a(i), lo, hi)));
        }

        lo =  0.0;
        hi =  1.0;
        a = math::vec2<Type>(0.5, 1.5);
        clamp_a = clamp(a, lo, hi);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(clamp_a(i), clamp_fun(a(i), lo, hi)));
        }
    }

    { /* test lerp */
        const size_t numsteps = 10000;
        constexpr Type one = static_cast<Type>(1);
        constexpr Type zero = static_cast<Type>(0);
        constexpr Type delta = 1.0 / (Type) numsteps;

        auto lerp_fun = [&] (
            const math::vec2<Type> &lo,
            const math::vec2<Type> &hi,
            const Type alpha) -> math::vec2<Type> {
            return (lo*(one - alpha) + hi*alpha);
        };

        math::vec2<Type> lo(-1.0);
        math::vec2<Type> hi( 0.0);
        Type alpha = 0.0;
        while (alpha < 1.0) {
            math::vec2<Type> a = lerp(lo,  hi, alpha);
            math::vec2<Type> b = lerp_fun(lo,  hi, alpha);
            math::vec2<Type> e = (a - b);
            for (size_t i = 0; i < a.length(); ++i) {
                REQUIRE(math::isequal(std::fabs(e(i)), zero));
            }
            alpha += delta;
        }

        lo = math::vec2<Type>(0.0);
        hi = math::vec2<Type>(1.0);
        alpha = 0.0;
        while (alpha < 1.0) {
            math::vec2<Type> a = lerp(lo,  hi, alpha);
            math::vec2<Type> b = lerp_fun(lo,  hi, alpha);
            math::vec2<Type> e = (a - b);
            for (size_t i = 0; i < a.length(); ++i) {
                REQUIRE(math::isequal(std::fabs(e(i)), zero));
            }
            alpha += delta;
        }
    }
}

#endif /* TEST_CORE_GEOMETRY_ARITHMETIC2_H_ */
