/*
 * camera.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/opengl.hpp"
#include "camera.hpp"
using namespace atto;

/** ---- Camera constructor ---------------------------------------------------
 * Camera::Camera
 */
Camera::Camera()
    : m_theta(0.0)
    , m_phi(0.0)
    , m_delta_theta(0.01*M_PI)
    , m_delta_phi(0.02*M_PI)
{}

/** ---- Camera view rotation functions ---------------------------------------
 * Camera::rotate_left
 * @brief Left-rotate the camera
 */
void Camera::rotate_left(void)
{
    m_phi += m_delta_phi;
}

/**
 * Camera::rotate_right
 * @brief Right-rotate the camera
 */
void Camera::rotate_right(void)
{
    m_phi -= m_delta_phi;
}

/**
 * Camera::rotate_up
 * @brief Up-rotate the camera
 */
void Camera::rotate_up(void)
{
    m_theta += m_delta_theta;
}

/**
 * Camera::rotate_down
 * @brief Down-rotate the camera.
 */
void Camera::rotate_down(void)
{
    m_theta -= m_delta_theta;
}

/** ---------------------------------------------------------------------------
 * Camera::mvp
 * @brief Return camera modelviewprojection matrix
 */
/*  */
math::mat4f Camera::lookat(void) const
{
    /* The camera is located at the origin by default. */
    math::vec3f eye(0.0);

    /* And is looking in the positive x-direction. */
    math::vec3f ctr(1.0, 0.0, 0.0);

    /* Rotate the view direction in the azimuth direction */
    ctr = math::rotate(ctr, math::vec3f(0.0, 0.0, 1.0), m_phi);

    /*
     * Compute the local basis set for the centre of reference.
     * The Gram-Schmidt up vector is in the z-direction unless it is parallel
     * to the w-direction, in which case, it will be in the y-direction.
     */
    math::vec3f w = math::normalize(eye - ctr);
    math::vec3f up(0.0, 0.0, 1.0);
    math::vec3f u = math::normalize(math::cross(up, w));
    /* math::vec3f v = math::normalize(math::cross(w, u)); */

    /* Rotate the view centre of reference in the polar direction */
    ctr = math::rotate(ctr, u, m_theta);

    /* Compute the modelviewprojection matrix */
    return math::lookat(eye, ctr, up);
}
