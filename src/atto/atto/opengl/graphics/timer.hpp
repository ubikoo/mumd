/*
 * timer.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GRAPHICS_TIMER_H_
#define ATTO_OPENGL_GRAPHICS_TIMER_H_

#include "atto/opengl/base.hpp"

namespace atto {
namespace gl {

/** ---------------------------------------------------------------------------
 * @brief Timer is a simple framerate counter class.
 */
struct Timer {
    /* Timer member variables and accessors. */
    size_t m_period;        /* sampling period */
    size_t m_count;         /* framerate counter */
    double m_start;         /* starting time */

    size_t period(void) const { return m_period; }
    size_t count(void) const { return m_count; }
    size_t start(void) const { return m_start; }

    /* Compute the elapsed and return the next frame count. */
    bool next(void) { return (++m_count % m_period == 0); }

    /* Return the elapsed time. */
    double elapsed(void) const { return (glfwGetTime() - m_start); }

    /* Return the framerate count. */
    double framerate(void) const {
        return (static_cast<double>(m_count) / elapsed());
    }

    /* Reset the framerate counter. */
    void reset(void) {
        m_count = 0;
        m_start = glfwGetTime();
    }

    /* Print the framerate count. */
    std::string to_string(void) const {
        const double fps = framerate();
        return core::str_format(
            "%llu frames @ %.2lf ms/frame (%.1lf fps)",
            count(),
            1000.0 / fps,
            fps);
    }

    /* Timer constructor/destructor. */
    Timer() : m_period(60) { reset(); }
    Timer(const size_t period) : m_period(period) { reset(); }
    ~Timer() = default;

    /* Copy constructor/assignment. */
    Timer(const Timer &other) {
        m_period = other.m_period;
        m_count = other.m_count;
        m_start = other.m_start;
    }
    Timer &operator=(const Timer &other) {
        if (this == &other)
            return *this;
        m_period = other.m_period;
        m_count = other.m_count;
        m_start = other.m_start;
        return *this;
    }
}; /* Timer */

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GRAPHICS_TIMER_H_ */
