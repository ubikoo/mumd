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
 * @brief Image viewer drawable object.
 */
struct Rop : atto::gl::Drawable {
    /* Images drawable */
    struct {
        GLuint program;                         /* shader program object */
        std::unique_ptr<atto::gl::Mesh> quad;   /* quad mesh */
        std::unique_ptr<atto::gl::Image> image1;/* first compositing image */
        std::unique_ptr<atto::gl::Image> image2;/* second compositing image */
        GLuint texture1;
        GLuint texture2;
        atto::math::mat4f mvp;                  /* mvp matrix */
        float alpha;                            /* blending alpha */
    } m_drawimg;

    /* Framebuffer drawable */
    struct {
        GLuint program;                         /* shader program object */
        GLsizei width;
        GLsizei height;
        GLuint framebuffer;                     /* framebuffer */
        GLuint color_texture;
        GLuint depth_texture;
        std::unique_ptr<atto::gl::Mesh> quad;   /* quad mesh */
        atto::math::mat4f mvp;                  /* mvp matrix */
    } m_drawfbo;

    /* Handle and draw member functions. */
    void handle(const atto::gl::Event &event) override;
    void draw(void *data = nullptr) override;

    /* Constructor/destructor. */
    Rop(
        const std::string &filename1,
        const std::string &filename2,
        const size_t n1,
        const size_t n2,
        const int width,
        const int height);
    ~Rop() = default;
    Rop(const Rop &) = delete;
    Rop &operator=(const Rop &) = delete;
};

#endif /* TEST_ATTO_OPENGL_ROP_H_ */
