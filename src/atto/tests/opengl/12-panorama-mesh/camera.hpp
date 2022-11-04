/**
 * camera.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ATTO_OPENGL_CAMERA_H_
#define TEST_ATTO_OPENGL_CAMERA_H_

#include "atto/math/math.hpp"
#include "atto/opengl/opengl.hpp"

/**
 * @brief Minimal camera class providing amodematrix.
 */
struct Camera {
    float m_theta;              /* camera polar angle */
    float m_phi;                /* camera azimuth angle */
    float m_delta_theta;        /* polar angle increment value */
    float m_delta_phi;          /* azimuth angle increment value */

    /* Camera view rotation functions */
    void rotate_left(void);
    void rotate_right(void);
    void rotate_up(void);
    void rotate_down(void);

    /* Camera modelviewprojection matrix */
    atto::math::mat4f lookat(void) const;

    /* Constructor/destructor. */
    Camera();
    ~Camera() = default;
    Camera(const Camera &) = delete;
    Camera &operator=(const Camera &) = delete;
};

#endif /* TEST_ATTO_OPENGL_CAMERA_H_ */
