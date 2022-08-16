/*
 * ndrange.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_NDRANGE_H_
#define ATTO_OPENCL_NDRANGE_H_

#include "atto/opencl/base.hpp"

namespace atto {
namespace cl {

/**
 * NDRange
 * @brief Class interface for specifying kernel NDRange values.
 */
struct NDRange {
    /** NDRange dimensions and sizes */
    cl_uint m_dim;
    std::array<size_t,3> m_sizes;

    cl_uint dim(void) const { return m_dim; }
    const size_t *data(void) const { return m_sizes.data(); }
    const size_t operator()(const size_t i) const { return m_sizes[i]; }

    /** A zero-dimensional range. */
    static const NDRange Null;

    /** Roundup global work size to the next multiple of local work size. */
    static size_t Roundup(size_t global_work_size, size_t local_work_size);

    /**
     * Make a new NDRange with global work size rounded up to the next
     * multiple of local work size.
     */
    static NDRange Make(size_t global_work_size, size_t local_work_size);

    /* Constructor/destructor. */
    NDRange()
        : m_dim(0) { m_sizes = {0, 0, 0}; }
    NDRange(size_t s0)
        : m_dim(1) { m_sizes = {s0, 0, 0}; }
    NDRange(size_t s0, size_t s1)
        : m_dim(2) { m_sizes = {s0, s1, 0}; }
    NDRange(size_t s0, size_t s1, size_t s2)
        : m_dim(3) { m_sizes = {s0, s1, s2}; }
    ~NDRange() = default;

    /* Copy constructor/assignment */
    NDRange(const NDRange &other) {
        m_dim = other.m_dim;
        m_sizes = other.m_sizes;
    }
    NDRange &operator=(const NDRange &other) {
        if (this == &other) {
            return *this;
        }
        m_dim = other.m_dim;
        m_sizes = other.m_sizes;
        return *this;
    }
}; /* NDRange */

} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_NDRANGE_H_ */
