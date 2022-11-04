/*
 * main.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include <vector>
#include <chrono>
#include <numeric>
#include <random>
#include <algorithm>

#include "../base.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * Test function
 */
#define TEST_VEC(T) \
void test_ ## T(void)\
{\
    {\
        std::cout << __PRETTY_FUNCTION__ << ": 2\n";\
        const cl_ ## T ## 2 one  = {1, 1};\
\
        cl_ ## T ## 2 a = {0, 1};\
        cl_ ## T ## 2 b = {1, 0};\
        cl_ ## T ## 2 c{};\
\
        /*\
         * Binary vector operators\
         */\
        c = a + b;\
        for (size_t i = 0; i < 2; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 1,"FAIL");\
        }\
        c = a - a;\
        for (size_t i = 0; i < 2; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 0,"FAIL");\
        }\
        c = a * one;\
        for (size_t i = 0; i < 2; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
        c = a / one;\
        for (size_t i = 0; i < 2; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
\
        /*\
         * Binary scalar operators\
         */\
        c = a + (cl_ ## T) 2;\
        for (size_t i = 0; i < 2; ++i) {\
            core_assert(c.s[i] == a.s[i] + 2,"FAIL");\
        }\
        c = c - (cl_ ## T) 1;\
        for (size_t i = 0; i < 2; ++i) {\
            core_assert(c.s[i] == a.s[i] + 1,"FAIL");\
        }\
        c = a * (cl_ ## T) 2;\
        for (size_t i = 0; i < 2; ++i) {\
            core_assert(c.s[i] == a.s[i] * 2,"FAIL");\
        }\
        c = c / (cl_ ## T) 2;\
        for (size_t i = 0; i < 2; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
\
        /*\
         * Increment operators\
         */\
        c = one;\
        c++;\
        for (size_t i = 0; i < 2; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 2,"FAIL");\
        }\
        c--;\
        for (size_t i = 0; i < 2; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 1,"FAIL");\
        }\
        c = -c;\
        for (size_t i = 0; i < 2; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) -1,"FAIL");\
        }\
    }\
\
    {\
        std::cout << __PRETTY_FUNCTION__ << ": 4\n";\
        const cl_ ## T ## 4 one  = {1, 1, 1, 1};\
\
        cl_ ## T ## 4 a = {0, 1, 2, 3};\
        cl_ ## T ## 4 b = {3, 2, 1, 0};\
        cl_ ## T ## 4 c{};\
\
        /*\
         * Binary vector operators\
         */\
        c = a + b;\
        for (size_t i = 0; i < 4; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 3,"FAIL");\
        }\
        c = a - a;\
        for (size_t i = 0; i < 4; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 0,"FAIL");\
        }\
        c = a * one;\
        for (size_t i = 0; i < 4; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
        c = a / one;\
        for (size_t i = 0; i < 4; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
\
        /*\
         * Binary scalar operators\
         */\
        c = a + (cl_ ## T) 2;\
        for (size_t i = 0; i < 4; ++i) {\
            core_assert(c.s[i] == a.s[i] + 2,"FAIL");\
        }\
        c = c - (cl_ ## T) 1;\
        for (size_t i = 0; i < 4; ++i) {\
            core_assert(c.s[i] == a.s[i] + 1,"FAIL");\
        }\
        c = a * (cl_ ## T) 2;\
        for (size_t i = 0; i < 4; ++i) {\
            core_assert(c.s[i] == a.s[i] * 2,"FAIL");\
        }\
        c = c / (cl_ ## T) 2;\
        for (size_t i = 0; i < 4; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
\
        /*\
         * Increment operators\
         */\
        c = one;\
        c++;\
        for (size_t i = 0; i < 4; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 2,"FAIL");\
        }\
        c--;\
        for (size_t i = 0; i < 4; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 1,"FAIL");\
        }\
        c = -c;\
        for (size_t i = 0; i < 4; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) -1,"FAIL");\
        }\
    }\
\
    {\
        std::cout << __PRETTY_FUNCTION__ << ": 8\n";\
        const cl_ ## T ## 8 one  = {1, 1, 1, 1, 1, 1, 1, 1};\
\
        cl_ ## T ## 8 a = {0, 1, 2, 3, 4, 5, 6, 7};\
        cl_ ## T ## 8 b = {7, 6, 5, 4, 3, 2, 1, 0};\
        cl_ ## T ## 8 c{};\
\
        /*\
         * Binary vector operators\
         */\
        c = a + b;\
        for (size_t i = 0; i < 8; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 7,"FAIL");\
        }\
        c = a - a;\
        for (size_t i = 0; i < 8; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 0,"FAIL");\
        }\
        c = a * one;\
        for (size_t i = 0; i < 8; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
        c = a / one;\
        for (size_t i = 0; i < 8; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
\
        /*\
         * Binary scalar operators\
         */\
        c = a + (cl_ ## T) 2;\
        for (size_t i = 0; i < 8; ++i) {\
            core_assert(c.s[i] == a.s[i] + 2,"FAIL");\
        }\
        c = c - (cl_ ## T) 1;\
        for (size_t i = 0; i < 8; ++i) {\
            core_assert(c.s[i] == a.s[i] + 1,"FAIL");\
        }\
        c = a * (cl_ ## T) 2;\
        for (size_t i = 0; i < 8; ++i) {\
            core_assert(c.s[i] == a.s[i] * 2,"FAIL");\
        }\
        c = c / (cl_ ## T) 2;\
        for (size_t i = 0; i < 8; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
\
        /*\
         * Increment operators\
         */\
        c = one;\
        c++;\
        for (size_t i = 0; i < 8; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 2,"FAIL");\
        }\
        c--;\
        for (size_t i = 0; i < 8; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 1,"FAIL");\
        }\
        c = -c;\
        for (size_t i = 0; i < 8; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) -1,"FAIL");\
        }\
    }\
\
    {\
        std::cout << __PRETTY_FUNCTION__ << ": 16\n";\
        const cl_ ## T ## 16 one  = {1, 1, 1, 1, 1, 1, 1, 1,\
                                     1, 1, 1, 1, 1, 1, 1, 1};\
\
        cl_ ## T ## 16 a = {0, 1,  2,  3,  4,  5,  6,  7,\
                            8, 9, 10, 11, 12, 13, 14, 15};\
        cl_ ## T ## 16 b = {15, 14, 13, 12, 11, 10, 9, 8,\
                             7,  6,  5,  4,  3,  2, 1, 0};\
        cl_ ## T ## 16 c{};\
\
        /*\
         * Binary vector operators\
         */\
        c = a + b;\
        for (size_t i = 0; i < 16; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 15,"FAIL");\
        }\
        c = a - a;\
        for (size_t i = 0; i < 16; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 0,"FAIL");\
        }\
        c = a * one;\
        for (size_t i = 0; i < 16; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
        c = a / one;\
        for (size_t i = 0; i < 16; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
\
        /*\
         * Binary scalar operators\
         */\
        c = a + (cl_ ## T) 2;\
        for (size_t i = 0; i < 16; ++i) {\
            core_assert(c.s[i] == a.s[i] + 2,"FAIL");\
        }\
        c = c - (cl_ ## T) 1;\
        for (size_t i = 0; i < 16; ++i) {\
            core_assert(c.s[i] == a.s[i] + 1,"FAIL");\
        }\
        c = a * (cl_ ## T) 2;\
        for (size_t i = 0; i < 16; ++i) {\
            core_assert(c.s[i] == a.s[i] * 2,"FAIL");\
        }\
        c = c / (cl_ ## T) 2;\
        for (size_t i = 0; i < 16; ++i) {\
            core_assert(c.s[i] == a.s[i],"FAIL");\
        }\
\
        /*\
         * Increment operators\
         */\
        c = one;\
        c++;\
        for (size_t i = 0; i < 16; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 2,"FAIL");\
        }\
        c--;\
        for (size_t i = 0; i < 16; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) 1,"FAIL");\
        }\
        c = -c;\
        for (size_t i = 0; i < 16; ++i) {\
            core_assert(c.s[i] == (cl_ ## T) -1,"FAIL");\
        }\
    }\
}

TEST_VEC(char)
TEST_VEC(uchar)

TEST_VEC(short)
TEST_VEC(ushort)

TEST_VEC(int)
TEST_VEC(uint)

TEST_VEC(long)
TEST_VEC(ulong)

TEST_VEC(float)
TEST_VEC(double)

/** ---------------------------------------------------------------------------
 * main
 */
int main(int argc, char const *argv[])
{
    test_char();
    test_uchar();

    test_short();
    test_ushort();

    test_int();
    test_uint();

    test_long();
    test_ulong();

    test_float();
    test_double();

    exit(EXIT_SUCCESS);
}