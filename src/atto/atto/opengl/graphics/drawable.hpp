/*
 * drawable.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GRAPHICS_DRAWABLE_H_
#define ATTO_OPENGL_GRAPHICS_DRAWABLE_H_

#include "atto/opengl/base.hpp"
#include "atto/opengl/graphics/event.hpp"
#include "atto/opengl/graphics/renderer.hpp"

namespace atto {
namespace gl {

/**
 * Drawable
 * @brief Drawable is an abstract class representing a drawable object.
 * Its interface is comprised of two pure virtual functions:
 *  handle(Event)   Handle the event in the drawable object.
 *  draw(data)      Draw the contents of the drawable object.
 */
struct Drawable {
    /* Drawable interface. */
    virtual void handle(const atto::gl::Event &event) = 0;
    virtual void draw(void *data = nullptr) = 0;

    /* Constructor/destructor. */
    Drawable() = default;
    virtual ~Drawable() = default;
    Drawable(const Drawable &other) = delete;
    Drawable &operator=(const Drawable &other) = delete;
};

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GRAPHICS_DRAWABLE_H_ */
