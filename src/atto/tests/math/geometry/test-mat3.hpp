/*
 * test-mat3.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_MAT3_H_
#define TEST_CORE_GEOMETRY_MAT3_H_

#include "../common.hpp"

/**
 * test_mat3<Type>
 * @brief mat3<Type> test client.
 */
template<typename Type>
void test_mat3 (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::mat3<Type> a{};
        math::mat3<Type> b(a);
        math::mat3<Type> c{};
        math::mat3<Type> d = c = b;
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
        math::mat3<Type> a(0,1,2,3,4,5,6,7,8);
        math::mat3<Type> b(a);
        math::mat3<Type> c{};
        math::mat3<Type> d = c = b;
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
        Type data[9] = {0,1,2,3,4,5,6,7,8};
        math::mat3<Type> a(&data[0]);
        math::mat3<Type> b(a);
        math::mat3<Type> c;
        math::mat3<Type> d = c = b;
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

        math::mat3<Type> a(0,1,2,3,4,5,6,7,8);
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
        math::mat3<Type> one(1);
        math::mat3<Type> two = one + one;                      /* + */
        math::mat3<Type> three = two + two - one;              /* +,- */

        math::mat3<Type> a(0,1,2,3,4,5,6,7,8);
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
        math::mat3<Type> one(1);
        math::mat3<Type> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::mat3<Type> three = two--;

        math::mat3<Type> a(0,1,2,3,4,5,6,7,8);
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
 * test_mat3<float>
 * @brief mat3<float> test client.
 */
template<>
void test_mat3<float> (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::mat3<float> a{};
        math::mat3<float> b(a);
        math::mat3<float> c{};
        math::mat3<float> d = c = b;
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
        math::mat3<float> a(0,1,2,3,4,5,6,7,8);
        math::mat3<float> b(a);
        math::mat3<float> c{};
        math::mat3<float> d = c = b;
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
        float data[9] = {0,1,2,3,4,5,6,7,8};
        math::mat3<float> a(&data[0]);
        math::mat3<float> b(a);
        math::mat3<float> c{};
        math::mat3<float> d = c = b;
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

        math::mat3<float> a(0,1,2,3,4,5,6,7,8);
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
        math::mat3<float> one(1);
        math::mat3<float> two = one + one;                      /* + */
        math::mat3<float> three = two + two - one;              /* +,- */

        math::mat3<float> a(0,1,2,3,4,5,6,7,8);
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
        math::mat3<float> one(1);
        math::mat3<float> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::mat3<float> three = two--;

        math::mat3<float> a(0,1,2,3,4,5,6,7,8);
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
 * test_mat3<double>
 * @brief mat3<double> test client.
 */
template<>
void test_mat3<double> (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::mat3<double> a{};
        math::mat3<double> b(a);
        math::mat3<double> c{};
        math::mat3<double> d = c = b;
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
        math::mat3<double> a(0,1,2,3,4,5,6,7,8);
        math::mat3<double> b(a);
        math::mat3<double> c{};
        math::mat3<double> d = c = b;
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
        double data[9] = {0,1,2,3,4,5,6,7,8};
        math::mat3<double> a(&data[0]);
        math::mat3<double> b(a);
        math::mat3<double> c{};
        math::mat3<double> d = c = b;
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

        math::mat3<double> a(0,1,2,3,4,5,6,7,8);
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
        math::mat3<double> one(1);
        math::mat3<double> two = one + one;                      /* + */
        math::mat3<double> three = two + two - one;              /* +,- */

        math::mat3<double> a(0,1,2,3,4,5,6,7,8);
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
        math::mat3<double> one(1);
        math::mat3<double> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::mat3<double> three = two--;

        math::mat3<double> a(0,1,2,3,4,5,6,7,8);
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