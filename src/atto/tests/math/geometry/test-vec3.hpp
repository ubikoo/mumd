/*
 * test-vec3.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_VEC3_H_
#define TEST_CORE_GEOMETRY_VEC3_H_

#include "../common.hpp"

/**
 * test_vec3<Type>
 * @brief vec3<Type> test client.
 */
template<typename Type>
void test_vec3 (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::vec3<Type> a{};
        math::vec3<Type> b(a);
        math::vec3<Type> c{};
        math::vec3<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == a(i));
            REQUIRE(c(i) == a(i));
            REQUIRE(d(i) == a(i));
        }
    }

    {
        /* test element constructor */
        math::vec3<Type> a(1,2,3);
        math::vec3<Type> b(a);
        math::vec3<Type> c{};
        math::vec3<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == static_cast<Type>(i+1));
            REQUIRE(c(i) == static_cast<Type>(i+1));
            REQUIRE(d(i) == static_cast<Type>(i+1));
        }
    }

    {
        /* test data constructor */
        Type data[3] = {1,2,3};
        math::vec3<Type> a(&data[0]);
        math::vec3<Type> b(a);
        math::vec3<Type> c{};
        math::vec3<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == static_cast<Type>(i+1));
            REQUIRE(c(i) == static_cast<Type>(i+1));
            REQUIRE(d(i) == static_cast<Type>(i+1));
        }
    }

    {
        /* test unary scalar operators */
        math::vec3<Type> a(1,2,3);
        a += 2;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= 1;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }

        a *= 6;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*6));
        }

        a /= 3;     /* a=(4,6,8) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*2));
        }
    }

    {
        /* test unary vector operators */
        math::vec3<Type> one(1);
        math::vec3<Type> two = one + one;                      /* + */
        math::vec3<Type> three = two + two - one;              /* +,- */

        math::vec3<Type> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }
    }

    {
        /* test increment operators */
        math::vec3<Type> one(1);
        math::vec3<Type> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::vec3<Type> three = two--;

        math::vec3<Type> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_vec3<float>
 * @brief vec3<float> test client.
 */
template<>
void test_vec3<float> (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::vec3<float> a{};
        math::vec3<float> b(a);
        math::vec3<float> c{};
        math::vec3<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), a(i)));
            REQUIRE(math::isequal(c(i), a(i)));
            REQUIRE(math::isequal(d(i), a(i)));
        }
    }

    {
        /* test element constructor */
        math::vec3<float> a(1,2,3);
        math::vec3<float> b(a);
        math::vec3<float> c{};
        math::vec3<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), static_cast<float>(i+1)));
            REQUIRE(math::isequal(c(i), static_cast<float>(i+1)));
            REQUIRE(math::isequal(d(i), static_cast<float>(i+1)));
        }
    }

    {
        /* test data constructor */
        float data[3] = {1,2,3};
        math::vec3<float> a(&data[0]);
        math::vec3<float> b(a);
        math::vec3<float> c{};
        math::vec3<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), static_cast<float>(i+1)));
            REQUIRE(math::isequal(c(i), static_cast<float>(i+1)));
            REQUIRE(math::isequal(d(i), static_cast<float>(i+1)));
        }
    }

    {
        /* test unary scalar operators */
        math::vec3<float> a(1,2,3);
        a += 2;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= 1;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+1)));
        }

        a *= 6;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>((i+1+1)*6)));
        }

        a /= 3;     /* a=(4,6,8) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>((i+1+1)*2)));
        }
    }

    {
        /* test unary vector operators */
        math::vec3<float> one(1);
        math::vec3<float> two = one + one;                      /* + */
        math::vec3<float> three = two + two - one;              /* +,- */

        math::vec3<float> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+1)));
        }
    }

    {
        /* test increment operators */
        math::vec3<float> one(1);
        math::vec3<float> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::vec3<float> three = two--;

        math::vec3<float> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+1)));
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_vec3<double>
 * @brief vec3<double> test client.
 */
template<>
void test_vec3<double> (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::vec3<double> a{};
        math::vec3<double> b(a);
        math::vec3<double> c{};
        math::vec3<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), a(i)));
            REQUIRE(math::isequal(c(i), a(i)));
            REQUIRE(math::isequal(d(i), a(i)));
        }
    }

    {
        /* test element constructor */
        math::vec3<double> a(1,2,3);
        math::vec3<double> b(a);
        math::vec3<double> c{};
        math::vec3<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), static_cast<double>(i+1)));
            REQUIRE(math::isequal(c(i), static_cast<double>(i+1)));
            REQUIRE(math::isequal(d(i), static_cast<double>(i+1)));
        }
    }

    {
        /* test data constructor */
        double data[3] = {1,2,3};
        math::vec3<double> a(&data[0]);
        math::vec3<double> b(a);
        math::vec3<double> c{};
        math::vec3<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), static_cast<double>(i+1)));
            REQUIRE(math::isequal(c(i), static_cast<double>(i+1)));
            REQUIRE(math::isequal(d(i), static_cast<double>(i+1)));
        }
    }

    {
        /* test unary scalar operators */
        math::vec3<double> a(1,2,3);
        a += 2;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= 1;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+1)));
        }

        a *= 6;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>((i+1+1)*6)));
        }

        a /= 3;     /* a=(4,6,8) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>((i+1+1)*2)));
        }
    }

    {
        /* test unary vector operators */
        math::vec3<double> one(1);
        math::vec3<double> two = one + one;                      /* + */
        math::vec3<double> three = two + two - one;              /* +,- */

        math::vec3<double> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+1)));
        }
    }

    {
        /* test increment operators */
        math::vec3<double> one(1);
        math::vec3<double> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::vec3<double> three = two--;

        math::vec3<double> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+1)));
        }
    }
}

#endif /* TEST_CORE_GEOMETRY_VEC3_H_ */