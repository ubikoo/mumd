/*
 * test-memory.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/core/core.hpp"
#include "test-memory.hpp"
using namespace atto;

/** ---- Memory test structure ------------------------------------------------
 */
struct X {
    /* Member variables and access functions. */
    size_t m_num = 0;
    size_t m_id = 0;
    core::align_unique_array<size_t> m_arr;

    int get() const { return m_num; }
    void set(int num) { m_num = num; }

    /* Constructor and destructor. */
    X(size_t num, size_t id)
        : m_num(num)
        , m_id(id) {
        m_arr = std::move(core::make_align_unique_array<size_t>(m_num, m_id));
    }
    ~X() = default;

    /* Disable copy constructor/assignment operators */
    X(const X &other) = delete;
    X &operator=(const X &other) = delete;

    /* Methods */
    bool check(void) const {
        bool ok = true;
        for (size_t i = 0; i < m_num; ++i) {
            ok &= (*m_arr)[i] == m_id;
        }
        return ok;
    }
};

/* ---- Memory ----------------------------------------------------------------
 */
void test_core_memory(size_t length)
{
    const size_t array_size = 1024;

    /*
     * Test align_unique_ptr
     */
    {
        std::vector<core::align_unique_ptr<X>> vector_data;
        for (size_t i = 0; i < length; ++i) {
            core::align_unique_ptr<X> px1 =
                core::make_align_unique_ptr<X>(array_size, 1);
            vector_data.push_back(std::move(px1));
            core_assert(vector_data.back()->check(), "FAIL");
            //vector_data.pop_back();
        }

        std::printf("vector_data size before clear: %lu\n", vector_data.size());
        core_assert(vector_data.size() == length, "FAIL");
        vector_data.clear();
        std::printf("vector_data size after clear: %lu\n", vector_data.size());
        core_assert(vector_data.size() == 0, "FAIL");
    }

    /*
     * Test align_unique_array
     */
    {
        std::vector<core::align_unique_array<X>> array_data;
        for (size_t i = 0; i < length; ++i) {
            core::align_unique_array<X> px2 =
                core::make_align_unique_array<X>(array_size, array_size, i);
            array_data.push_back(std::move(px2));
            for (size_t j = 0; j < array_size; ++j) {
                core_assert((*array_data.back() + j)->check(), "FAIL");
                core_assert((*array_data.back())[j].check(), "FAIL");
            }
            //array_data.pop_back();
        }

        std::printf(
            "array_data size before clear: %lu\n", array_data.size());
        core_assert(array_data.size() == length, "FAIL");
        array_data.clear();
        std::printf(
            "array_data size after clear: %lu\n", array_data.size());
        core_assert(array_data.size() == 0, "FAIL");
    }

    /* Return OK */
    std::printf("OK\n");
}
