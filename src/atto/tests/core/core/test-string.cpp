/*
 * test-string.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/core/core.hpp"
#include "test-string.hpp"
using namespace atto;

/** ---- String utilities -----------------------------------------------------
 */
void test_core_string(void)
{
    {
        const char int64_str[] = "4611686018427387904";
        const char int32_str[] = "1073741824";
        const char int16_str[] = "16384";
        const char int8_str[] = "128";

        std::cout << core::str_cast<long double>(int64_str) << std::endl;
        std::cout << core::str_cast<double>(int64_str) << std::endl;
        std::cout << core::str_cast<float>(int64_str) << std::endl;

        std::cout << core::str_cast<long long>(int64_str) << std::endl;
        std::cout << core::str_cast<long>(int64_str) << std::endl;
        std::cout << core::str_cast<int>(int32_str) << std::endl;
        std::cout << core::str_cast<short>(int16_str) << std::endl;

        std::cout << core::str_cast<unsigned long long>(int64_str) << std::endl;
        std::cout << core::str_cast<unsigned long>(int64_str) << std::endl;
        std::cout << core::str_cast<unsigned int>(int32_str) << std::endl;
        std::cout << core::str_cast<unsigned short>(int16_str) << std::endl;

        std::cout << core::str_cast<int64_t>(int64_str) << std::endl;
        std::cout << core::str_cast<int32_t>(int32_str) << std::endl;
        std::cout << core::str_cast<int16_t>(int16_str) << std::endl;
        std::cout << core::str_cast<int8_t>(int8_str) << std::endl;

        std::cout << core::str_cast<uint64_t>(int64_str) << std::endl;
        std::cout << core::str_cast<uint32_t>(int32_str) << std::endl;
        std::cout << core::str_cast<uint16_t>(int16_str) << std::endl;
        std::cout << core::str_cast<uint8_t>(int8_str) << std::endl;
    }

    {
        /*
         * Test token string 1
         */
        char str[] = "You    should have    received a    copy";
        std::vector<std::string> tokens = core::str_split_tokens(str);
        std::printf("n_tokens %lu\n", tokens.size());
        for (auto &t : tokens) {
            std::string word = t;
            core::str_trim(word);
            std::printf("[%s] -> [%s]\n\n", t.c_str(), word.c_str());
        }
    }

    {
        /*
         * Test token string 2
         */
        char str[] =
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do"
            " eiusmod tempor incididunt ut labore et dolore magna aliqua."
            " Ut enim ad minim veniam, quis nostrud exercitation ullamco"
            " laboris nisi ut aliquip ex ea commodo consequat."
            " Duis aute irure dolor in reprehenderit in voluptate velit esse"
            " cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat"
            " cupidatat non proident, sunt in culpa qui officia deserunt mollit"
            " anim id est laborum.";

        size_t n_tokens;
        char **v_tokens = core::str_split_tokens(str, &n_tokens);
        std::vector<std::string> tokens = core::str_split_tokens(str);
        std::printf("n_tokens %lu %lu\n", n_tokens, tokens.size());
        core_assert(n_tokens == tokens.size(), "FAIL");

        for (size_t i = 0; i < tokens.size(); ++i) {
            std::string word1(tokens[i]);
            std::string word2(v_tokens[i]);
            core::str_trim(word1);
            core::str_trim(word2);
            core_assert(word1.compare(word2) == 0, "FAIL");
            std::printf("[%s] [%s]\n\n", word1.c_str(), word2.c_str());
        }

        for (size_t i = 0; i < n_tokens; ++i) {
            free(v_tokens[i]);
        }
        free(v_tokens);
    }
}
