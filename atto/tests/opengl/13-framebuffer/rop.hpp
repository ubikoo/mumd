/*
 * rop.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ATTO_OPENGL_ROP_H_
#define TEST_ATTO_OPENGL_ROP_H_

#include <memory>
#include "atto/math/math.hpp"
#include "atto/opengl/opengl.hpp"

/**
 * Rop
 */
struct Rop : atto::gl::Drawable {
    /* Sphere drawable */
    GLuint m_sphere_program;                         /* shader program object */
    std::unique_ptr<atto::gl::Mesh> m_sphere_mesh;   /* image and texture */
    GLuint m_sphere_texture;                         /* sphere texture */
    atto::math::mat4f m_sphere_mvp;                  /* sphere mvp matrix */

    /* Mixing parameter */
    float m_texmix = 0.0;
    const float &texmix() const { return m_texmix; }
    void texmix(const float val) {
        m_texmix = std::min(std::max(val, 0.0f), 1.0f);
    }

    /* Quad drawable */
    GLuint m_quad_program;                           /* shader program object */
    std::unique_ptr<atto::gl::Mesh> m_quad_mesh;     /* image and texture */
    atto::math::mat4f m_quad_mvp;                    /* quad mvp matrix */

    /* Framebuffer objects */
    GLuint m_fbo;                                    /* framebuffer object */
    GLsizei m_fbo_width;
    GLsizei m_fbo_height;
    GLuint m_fbo_color_texture;
    GLuint m_fbo_depth_texture;

    /* Handle and draw member functions. */
    void handle(const atto::gl::Event &event) override;
    void draw(void *data = nullptr) override;

    /* Constructor/destructor. */
    Rop(const std::string &filename, const size_t n1, const size_t n2);
    ~Rop() = default;
    Rop(const Rop &) = delete;
    Rop &operator=(const Rop &) = delete;
};

#endif /* TEST_ATTO_OPENGL_ROP_H_ */
