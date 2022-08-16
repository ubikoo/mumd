/*
 * test-mat2.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_MAT2_H_
#define TEST_CORE_GEOMETRY_MAT2_H_

#include "../common.hpp"

/**
 * test_mat2<Type>
 * @brief mat2<Type> test client.
 */
template<typename Type>
void test_mat2 (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::mat2<Type> a{};
        math::mat2<Type> b(a);
        math::mat2<Type> c;
        math::mat2<Type> d = c = b;
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
        math::mat2<Type> a(0,1,2,3);
        math::mat2<Type> b(a);
        math::mat2<Type> c;
        math::mat2<Type> d = c = b;
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
        Type data[4] = {0,1,2,3};
        math::mat2<Type> a(&data[0]);
        math::mat2<Type> b(a);
        math::mat2<Type> c;
        math::mat2<Type> d = c = b;
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

        math::mat2<Type> a(0,1,2,3);
        a += two;       /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 2));
            }
        }

        a -= one;       /* a=(1,2,3,4) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 1));
            }
        }
    }

    {
        /* test unary matrix operators */
        math::mat2<Type> one(1);
        math::mat2<Type> two = one + one;                      /* + */

        math::mat2<Type> a(0,1,2,3);
        a += two;       /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 2));
            }
        }

        a -= one;       /* a=(1,2,3,4) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 1));
            }
        }
    }

    {
        /* test increment operators */
        math::mat2<Type> one(1);
        math::mat2<Type> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::mat2<Type> three = two--;

        math::mat2<Type> a(0,1,2,3);
        a += two;       /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 2));
            }
        }

        a -= one;       /* a=(1,2,3,4) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 1));
            }
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_mat2<float>
 * @brief mat2<float> test client.
 */
template<>
void test_mat2<float> (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::mat2<float> a{};
        math::mat2<float> b(a);
        math::mat2<float> c{};
        math::mat2<float> d = c = b;
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
        math::mat2<float> a(0,1,2,3);
        math::mat2<float> b(a);
        math::mat2<float> c{};
        math::mat2<float> d = c = b;
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
        float data[4] = {0,1,2,3};
        math::mat2<float> a(&data[0]);
        math::mat2<float> b(a);
        math::mat2<float> c;
        math::mat2<float> d = c = b;
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

        math::mat2<float> a(0,1,2,3);
        a += two;       /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 1)));
            }
        }
    }

    {
        /* test unary matrix operators */
        math::mat2<float> one(1);
        math::mat2<float> two = one + one;                      /* + */
        math::mat2<float> three = two + two - one;              /* +,- */

        math::mat2<float> a(0,1,2,3);
        a += two;       /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 1)));
            }
        }
    }

    {
        /* test increment operators */
        math::mat2<float> one(1);
        math::mat2<float> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::mat2<float> three = two--;

        math::mat2<float> a(0,1,2,3);
        a += two;       /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 1)));
            }
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_mat2<double>
 * @brief mat2<double> test client.
 */
template<>
void test_mat2<double> (void)
{
    using namespace atto;

    {
        /* test default constructor */
        math::mat2<double> a{};
        math::mat2<double> b(a);
        math::mat2<double> c{};
        math::mat2<double> d = c = b;
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
        math::mat2<double> a(0,1,2,3);
        math::mat2<double> b(a);
        math::mat2<double> c{};
        math::mat2<double> d = c = b;
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
        double data[4] = {0,1,2,3};
        math::mat2<double> a(&data[0]);
        math::mat2<double> b(a);
        math::mat2<double> c{};
        math::mat2<double> d = c = b;
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

        math::mat2<double> a(0,1,2,3);
        a += two;       /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 1)));
            }
        }
    }

    {
        /* test unary matrix operators */
        math::mat2<double> one(1);
        math::mat2<double> two = one + one;                      /* + */
        math::mat2<double> three = two + two - one;              /* +,- */

        math::mat2<double> a(0,1,2,3);
        a += two;       /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 1)));
            }
        }
    }

    {
        /* test increment operators */
        math::mat2<double> one(1);
        math::mat2<double> two(one);
        two++;
        two--;
        ++two;
        ++two;
        math::mat2<double> three = two--;

        math::mat2<double> a(0,1,2,3);
        a += two;       /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 1)));
            }
        }
    }
}

#endif /* TEST_CORE_GEOMETRY_VECMAT_ */
