/*
 * test-vec4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_VEC4_H_
#define TEST_CORE_GEOMETRY_VEC4_H_

#include "../common.hpp"

/**
 * test_vec4<Type>
 * @brief vec4<Type> test client.
 */
template<typename Type>
void test_vec4 (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::vec4<Type> a{};
        math::vec4<Type> b(a);
        math::vec4<Type> c{};
        math::vec4<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == a(i));
            REQUIRE(c(i) == a(i));
            REQUIRE(d(i) == a(i));
        }
    }

    {
        /* test element constructor */
        math::vec4<Type> a(1,2,3,4);
        math::vec4<Type> b(a);
        math::vec4<Type> c{};
        math::vec4<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == static_cast<Type>(i+1));
            REQUIRE(c(i) == static_cast<Type>(i+1));
            REQUIRE(d(i) == static_cast<Type>(i+1));
        }
    }

    {
        /* test data constructor */
        Type data[4] = {1,2,3,4};
        math::vec4<Type> a(&data[0]);
        math::vec4<Type> b(a);
        math::vec4<Type> c{};
        math::vec4<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == static_cast<Type>(i+1));
            REQUIRE(c(i) == static_cast<Type>(i+1));
            REQUIRE(d(i) == static_cast<Type>(i+1));
        }
    }

    {
        /* test unary scalar operators */
        const Type one = static_cast<Type>(1);
        const Type two = static_cast<Type>(2);

        math::vec4<Type> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }
    }

    {
        /* test unary vector operators */
        math::vec4<Type> one(1);
        math::vec4<Type> two = one + one;                      /* + */
        math::vec4<Type> three = two + two - one;              /* +,- */

        math::vec4<Type> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }
    }

    {
        /* test increment operators */
        math::vec4<Type> one(1);
        math::vec4<Type> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::vec4<Type> three = two--;

        math::vec4<Type> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_vec4<float>
 * @brief vec4<float> test client.
 */
template<>
void test_vec4<float> (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::vec4<float> a{};
        math::vec4<float> b(a);
        math::vec4<float> c{};
        math::vec4<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), a(i)));
            REQUIRE(math::isequal(c(i), a(i)));
            REQUIRE(math::isequal(d(i), a(i)));
        }
    }

    {
        /* test element constructor */
        math::vec4<float> a(1,2,3,4);
        math::vec4<float> b(a);
        math::vec4<float> c{};
        math::vec4<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), static_cast<float>(i+1)));
            REQUIRE(math::isequal(c(i), static_cast<float>(i+1)));
            REQUIRE(math::isequal(d(i), static_cast<float>(i+1)));
        }
    }

    {
        /* test data constructor */
        float data[4] = {1,2,3,4};
        math::vec4<float> a(&data[0]);
        math::vec4<float> b(a);
        math::vec4<float> c;
        math::vec4<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), static_cast<float>(i+1)));
            REQUIRE(math::isequal(c(i), static_cast<float>(i+1)));
            REQUIRE(math::isequal(d(i), static_cast<float>(i+1)));
        }
    }

    {
        /* test unary scalar operators */
        float one = static_cast<float>(1);
        float two = static_cast<float>(2);

        math::vec4<float> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+1)));
        }
    }

    {
        /* test unary vector operators */
        math::vec4<float> one(1);
        math::vec4<float> two = one + one;                      /* + */
        math::vec4<float> three = two + two - one;              /* +,- */

        math::vec4<float> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+1)));
        }
    }

    {
        /* test increment operators */
        math::vec4<float> one(1);
        math::vec4<float> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::vec4<float> three = two--;

        math::vec4<float> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<float>(i+1+1)));
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_vec4<double>
 * @brief vec4<double> test client.
 */
template<>
void test_vec4<double> (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::vec4<double> a{};
        math::vec4<double> b(a);
        math::vec4<double> c{};
        math::vec4<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), a(i)));
            REQUIRE(math::isequal(c(i), a(i)));
            REQUIRE(math::isequal(d(i), a(i)));
        }
    }

    {
        /* test element constructor */
        math::vec4<double> a(1,2,3,4);
        math::vec4<double> b(a);
        math::vec4<double> c{};
        math::vec4<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), static_cast<double>(i+1)));
            REQUIRE(math::isequal(c(i), static_cast<double>(i+1)));
            REQUIRE(math::isequal(d(i), static_cast<double>(i+1)));
        }
    }

    {
        /* test data constructor */
        double data[4] = {1,2,3,4};
        math::vec4<double> a(&data[0]);
        math::vec4<double> b(a);
        math::vec4<double> c;
        math::vec4<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(math::isequal(b(i), static_cast<double>(i+1)));
            REQUIRE(math::isequal(c(i), static_cast<double>(i+1)));
            REQUIRE(math::isequal(d(i), static_cast<double>(i+1)));
        }
    }

    {
        /* test unary scalar operators */
        double one = static_cast<double>(1);
        double two = static_cast<double>(2);

        math::vec4<double> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+1)));
        }
    }

    {
        /* test unary vector operators */
        math::vec4<double> one(1);
        math::vec4<double> two = one + one;                      /* + */
        math::vec4<double> three = two + two - one;              /* +,- */

        math::vec4<double> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+1)));
        }
    }

    {
        /* test increment operators */
        math::vec4<double> one(1);
        math::vec4<double> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::vec4<double> three = two--;

        math::vec4<double> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(math::isequal(a(i), static_cast<double>(i+1+1)));
        }
    }
}

#endif /* TEST_CORE_GEOMETRY_VEC4_H_ */