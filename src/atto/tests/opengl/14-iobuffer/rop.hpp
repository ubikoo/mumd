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
#include "atto/opengl/opengl.hpp"

/**
 * Rop
 */
struct Rop : atto::gl::Drawable {
    /* Map begin shader. */
    struct MapBegin {
        GLuint program;
        std::unique_ptr<atto::gl::Mesh> quad;
        GLuint texture;
    };
    MapBegin map_begin;

    /* Map run shader. */
    struct MapRun {
        GLuint program;
        std::unique_ptr<atto::gl::Mesh> quad;
        std::array<std::unique_ptr<IOBuffer>,2> buffer;
        GLint width;
        GLint height;
        size_t read_ix = 0;
        size_t write_ix = 1;
        size_t iterations = 0;
    };
    MapRun map_run;

    /* Map end shader. */
    struct MapEnd {
        GLuint program;
        std::unique_ptr<atto::gl::Mesh> quad;
    };
    MapEnd map_end;

    /* Handle and draw member functions. */
    void handle(const atto::gl::Event &event) override;
    void draw(void *data = nullptr) override;

    /* Constructor/destructor. */
    Rop(const std::string &filename, const size_t width, const size_t height);
    ~Rop() = default;
    Rop(const Rop &) = delete;
    Rop &operator=(const Rop &) = delete;
};

#endif /* TEST_ATTO_OPENGL_ROP_H_ */
