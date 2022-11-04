/*
 * test-mat4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_MAT4_H_
#define TEST_CORE_GEOMETRY_MAT4_H_

#include "../common.hpp"

/**
 * test_mat4<Type>
 * @brief mat4<Type> test client.
 */
template<typename Type>
void test_mat4 (void)
{
    using namespace atto;

    {
        /* test default constructor */
        atto::math::mat4<Type> a{};
        atto::math::mat4<Type> b(a);
        atto::math::mat4<Type> c{};
        atto::math::mat4<Type> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(b(i, j) == a(i, j));
                REQUIRE(c(i, j) == a(i, j));
                REQUIRE(d(i, j) == a(i, j));
            }
        }
    }

    {
        /* test element constructor */
        atto::math::mat4<Type> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        atto::math::mat4<Type> b(a);
        atto::math::mat4<Type> c{};
        atto::math::mat4<Type> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(b(i, j) == static_cast<Type>(i*a.dim() + j));
                REQUIRE(c(i, j) == static_cast<Type>(i*a.dim() + j));
                REQUIRE(d(i, j) == static_cast<Type>(i*a.dim() + j));
            }
        }
    }

    {
        /* test data constructor */
        Type data[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        atto::math::mat4<Type> a(&data[0]);
        atto::math::mat4<Type> b(a);
        atto::math::mat4<Type> c{};
        atto::math::mat4<Type> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(b(i, j) == static_cast<Type>(i*a.dim() + j));
                REQUIRE(c(i, j) == static_cast<Type>(i*a.dim() + j));
                REQUIRE(d(i, j) == static_cast<Type>(i*a.dim() + j));
            }
        }
    }

    {
        /* test unary scalar operators */
        Type one = static_cast<Type>(1);
        Type two = static_cast<Type>(2);

        atto::math::mat4<Type> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 2));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 1));
            }
        }
    }

    {
        /* test unary matrix operators */
        atto::math::mat4<Type> one(1);
        atto::math::mat4<Type> two = one + one;                     // +
        atto::math::mat4<Type> three = two + two - one;             // + - /

        atto::math::mat4<Type> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 2));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 1));
            }
        }
    }

    {
        /* test increment operators */
        atto::math::mat4<Type> one(1);
        atto::math::mat4<Type> two(one);
        two++;
        two--;
        ++two;
        ++two;
        atto::math::mat4<Type> three = two--;

        atto::math::mat4<Type> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 2));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 1));
            }
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_mat4<float>
 * @brief mat4<float> test client.
 */
template<>
void test_mat4<float> (void)
{
    using namespace atto;

    {
        /* test default constructor */
        atto::math::mat4<float> a{};
        atto::math::mat4<float> b(a);
        atto::math::mat4<float> c{};
        atto::math::mat4<float> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(math::isequal(b(i, j), a(i, j)));
                REQUIRE(math::isequal(c(i, j), a(i, j)));
                REQUIRE(math::isequal(d(i, j), a(i, j)));
            }
        }
    }

    {
        /* test element constructor */
        atto::math::mat4<float> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        atto::math::mat4<float> b(a);
        atto::math::mat4<float> c{};
        atto::math::mat4<float> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(math::isequal(
                    b(i, j), static_cast<float>(i*a.dim() + j)));
                REQUIRE(math::isequal(
                    c(i, j), static_cast<float>(i*a.dim() + j)));
                REQUIRE(math::isequal(
                    d(i, j), static_cast<float>(i*a.dim() + j)));
            }
        }
    }

    {
        /* test data constructor */
        float data[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        atto::math::mat4<float> a(&data[0]);
        atto::math::mat4<float> b(a);
        atto::math::mat4<float> c{};
        atto::math::mat4<float> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(math::isequal(
                    b(i, j), static_cast<float>(i*a.dim() + j)));
                REQUIRE(math::isequal(
                    c(i, j), static_cast<float>(i*a.dim() + j)));
                REQUIRE(math::isequal(
                    d(i, j), static_cast<float>(i*a.dim() + j)));
            }
        }
    }

    {
        /* test unary scalar operators */
        float one = static_cast<float>(1);
        float two = static_cast<float>(2);

        atto::math::mat4<float> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 1)));
            }
        }
    }

    {
        /* test unary matrix operators */
        atto::math::mat4<float> one(1);
        atto::math::mat4<float> two = one + one;                     // +
        atto::math::mat4<float> three = two + two - one;             // + -

        atto::math::mat4<float> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 1)));
            }
        }
    }

    {
        /* test increment operators */
        atto::math::mat4<float> one(1);
        atto::math::mat4<float> two(one);
        two++;
        two--;
        ++two;
        ++two;
        atto::math::mat4<float> three = two--;

        atto::math::mat4<float> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 1)));
            }
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_mat4<double>
 * @brief mat4<double> test client.
 */
template<>
void test_mat4<double> (void)
{
    using namespace atto;

    {
        /* test default constructor */
        atto::math::mat4<double> a{};
        atto::math::mat4<double> b(a);
        atto::math::mat4<double> c{};
        atto::math::mat4<double> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(math::isequal(b(i, j), a(i, j)));
                REQUIRE(math::isequal(c(i, j), a(i, j)));
                REQUIRE(math::isequal(d(i, j), a(i, j)));
            }
        }
    }

    {
        /* test element constructor */
        atto::math::mat4<double> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        atto::math::mat4<double> b(a);
        atto::math::mat4<double> c{};
        atto::math::mat4<double> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(math::isequal(
                    b(i, j), static_cast<double>(i*a.dim() + j)));
                REQUIRE(math::isequal(
                    c(i, j), static_cast<double>(i*a.dim() + j)));
                REQUIRE(math::isequal(
                    d(i, j), static_cast<double>(i*a.dim() + j)));
            }
        }
    }

    {
        /* test data constructor */
        double data[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        atto::math::mat4<double> a(&data[0]);
        atto::math::mat4<double> b(a);
        atto::math::mat4<double> c{};
        atto::math::mat4<double> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(math::isequal(
                    b(i, j), static_cast<double>(i*a.dim() + j)));
                REQUIRE(math::isequal(
                    c(i, j), static_cast<double>(i*a.dim() + j)));
                REQUIRE(math::isequal(
                    d(i, j), static_cast<double>(i*a.dim() + j)));
            }
        }
    }

    {
        /* test unary scalar operators */
        double one = static_cast<double>(1);
        double two = static_cast<double>(2);

        atto::math::mat4<double> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 1)));
            }
        }
    }

    {
        /* test unary matrix operators */
        atto::math::mat4<double> one(1);
        atto::math::mat4<double> two = one + one;                     // +
        atto::math::mat4<double> three = two + two - one;             // + -

        atto::math::mat4<double> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 1)));
            }
        }
    }

    {
        /* test increment operators */
        atto::math::mat4<double> one(1);
        atto::math::mat4<double> two(one);
        two++;
        two--;
        ++two;
        ++two;
        atto::math::mat4<double> three = two--;

        atto::math::mat4<double> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 1)));
            }
        }
    }
}

#endif /* TEST_CORE_GEOMETRY_VECMAT_ */