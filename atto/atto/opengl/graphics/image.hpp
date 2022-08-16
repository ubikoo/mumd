/*
 * image.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GRAPHICS_IMAGE_H_
#define ATTO_OPENGL_GRAPHICS_IMAGE_H_

#include "atto/opengl/base.hpp"
#include "atto/opengl/graphics/image-stb.hpp"

namespace atto {
namespace gl {

/** ---- Image ----------------------------------------------------------------
 * Image maintains an image bitmap as a pixel buffer characterized by:
 *
 *  1. Pixel storage layout
 *      width:  bitmap width in pixels.
 *      height: bitmap height in pixels.
 *      bpp:    bitmap pixel depth in bits per pixel.
 *      stride: bitmap pixel stride in bytes per pixel,
 *               4=32/8(RGBA), 3=24/8(RGB), 2=16/8(RG) and 1=8/8(R).
 *
 *  2. Bitmap size and data
 *      pitch:  bitmap row stride in bytes, calculated from its width
 *              and bpp rounded to the next 4-byte (32-bit) boundary.
 *      size:   size of the bitmap pixel buffer in bytes.
 *      bitmap: bitmap pixel buffer data array.
 */
struct Image {
    /**
     * @brief Image member variables.
     */
    uint32_t m_width;                   /* bitmap width in pixels */
    uint32_t m_height;                  /* bitmap height in pixels */
    uint32_t m_bpp;                     /* pixel bit depth */
    uint32_t m_pitch;                   /* scanline width in bytes */
    uint32_t m_size;                    /* bitmap size in bytes, height*pitch */
    core::align_unique_array<uint8_t> m_bitmap; /* bitmap pixel buffer array */

    /**
     * @brief Image query functions.
     */
    uint32_t width(void) const { return m_width; }
    uint32_t height(void) const { return m_height; }
    uint32_t bpp(void) const { return m_bpp; }
    uint32_t pitch(void) const { return m_pitch; }
    uint32_t size(void) const { return m_size; }
    GLenum pixelformat(void) const;

    bool is_empty(void) const { return (size() == 0); }
    uint8_t *bitmap(void) { return (is_empty() ? nullptr : *m_bitmap); }
    const uint8_t *bitmap(void) const {
        return (is_empty() ? nullptr : *m_bitmap);
    }

    /**
     * @brief Image resize and information log.
     */
    void resize(
        const uint32_t width,
        const uint32_t height,
        const uint32_t bpp);
    std::string infolog(const char *comment = nullptr) const;

    /**
     * @brief Copy image bitmap data to 2d-texture object.
     */
    void copy(const GLuint &texture) const;

    /**
     * @brief Image file input/output.
     */
    void load(
        const std::string &filename,
        const bool flip_vertically = false,
        const int32_t n_channels = 0);
    void write_png(
        const std::string &filename,
        const bool flip_vertically = false);
    void write_ppma(
        const std::string &filename,
        const bool flip_vertically = false);
    void write_ppmb(
        const std::string &filename,
        const bool flip_vertically = false);

    /**
     * @brief Image pixel functions.
     */
    uint8_t *pixel(const uint32_t x, const uint32_t y);
    const uint8_t *pixel(const uint32_t, const uint32_t y) const;

    uint8_t *red(const uint32_t x, const uint32_t y);
    const uint8_t *red(const uint32_t x, const uint32_t y) const;

    uint8_t *green(const uint32_t x, const uint32_t y);
    const uint8_t *green(const uint32_t x, const uint32_t y) const;

    uint8_t *blue(const uint32_t x, const uint32_t y);
    const uint8_t *blue(const uint32_t x, const uint32_t y) const;

    uint8_t *alpha(const uint32_t x, const uint32_t y);
    const uint8_t *alpha(const uint32_t x, const uint32_t y) const;

    void swap(const uint32_t ic, const uint32_t jc, bool native = true);
    void swap_pix32(const uint32_t ic, const uint32_t jc, uint32_t *pixel);
    void swap_pix8(const uint32_t ic, const uint32_t jc, uint8_t *pixel);

    /** ---- Special member functions -----------------------------------------
     * Create an image with a bitmap pixel buffer with (width * height) pixels,
     * and a pixel bit depth in bits per pixel.
     */
    explicit Image(
        const uint32_t width,
        const uint32_t height,
        const uint32_t bpp) {
        try {
            resize(width, height, bpp);
        } catch (std::exception &e) {
            core_throw(e.what());
        }
    }

    /** Create an image bitmap from a file and flip vertically. */
    explicit Image(
        const std::string &filename,
        const bool flip_vertically = false,
        const int32_t n_channels = 0) {
        try {
            load(filename, flip_vertically, n_channels);
        } catch (std::exception &e) {
            core_throw(e.what());
        }
    }
    ~Image() = default;

    /* Copy constructor/assignment. */
    Image(const Image &other) {
        try {
            resize(other.m_width, other.m_height, other.m_bpp);
            std::memcpy(*m_bitmap, *(other.m_bitmap), m_size);
        } catch (std::exception &e) {
            core_throw(e.what());
        }
    }
    Image &operator=(const Image &other) {
        if (this == &other) {
            return *this;
        }
        try {
            resize(other.m_width, other.m_height, other.m_bpp);
            std::memcpy(*m_bitmap, *(other.m_bitmap), m_size);
        } catch (std::exception &e) {
            core_throw(e.what());
        }
        return *this;
    }

    /* Move constructor/assignment. */
    Image(Image &&other)
        : m_width(other.m_width)
        , m_height(other.m_height)
        , m_bpp(other.m_bpp)
        , m_pitch(other.m_pitch)
        , m_size(other.m_size)
        , m_bitmap(other.m_bitmap.release()) {}
    Image &operator=(Image &&other) {
        if (this == &other) {
            return *this;
        }
        m_width = other.m_width;
        m_height = other.m_height;
        m_bpp = other.m_bpp;
        m_pitch = other.m_pitch;
        m_size = other.m_size;
        m_bitmap.reset(other.m_bitmap.release());
        return *this;
    }
};

/** ---- Image pixel functions ------------------------------------------------
 * Image::pixel
 * @brief Access the pixel at row x and column y.
 * If x and y are inside the bitmap range, return a pointer to the pixel
 * address specified by the pitch size and number of colour components.
 * Otherwise, return null.
 */
core_inline
uint8_t *Image::pixel(const uint32_t x, const uint32_t y)
{
    const uint32_t pixel_bytes = m_bpp >> 3;
    if (!is_empty() && x < m_width && y < m_height) {
        return (*m_bitmap + y*m_pitch + x*pixel_bytes);
    }
    return nullptr;
}

core_inline
const uint8_t *Image::pixel(const uint32_t x, const uint32_t y) const
{
    const uint32_t pixel_bytes = m_bpp >> 3;
    if (!is_empty() && x < m_width && y < m_height) {
        return (*m_bitmap + y*m_pitch + x*pixel_bytes);
    }
    return nullptr;
}

/** ---- Image red component --------------------------------------------------
 * Image::red
 * @brief Access to the red component of pixel at row x and column y.
 */
core_inline
uint8_t *Image::red(const uint32_t x, const uint32_t y)
{
    uint8_t *px = pixel(x,y);
    if (px) {
        return (px + 0);  /* red is first byte in a pixel with rgb layout */
    }
    return nullptr;
}

core_inline
const uint8_t *Image::red(const uint32_t x, const uint32_t y) const
{
    const uint8_t *px = pixel(x,y);
    if (px) {
        return (px + 0);  /* red is first byte in a pixel with rgb layout */
    }
    return nullptr;
}

/** ---- Image green component ------------------------------------------------
 * Image::green
 * @brief Access to the green component of pixel at row x and column y.
 */
core_inline
uint8_t *Image::green(const uint32_t x, const uint32_t y)
{
    uint8_t *px = pixel(x,y);
    if (px && m_bpp > 8) {
        return (px + 1);  /* green is second byte in a pixel with rgb layout */
    }
    return nullptr;
}

core_inline
const uint8_t *Image::green(const uint32_t x, const uint32_t y) const
{
    const uint8_t *px = pixel(x,y);
    if (px && m_bpp > 8) {
        return (px + 1);  /* green is second byte in a pixel with rgb layout */
    }
    return nullptr;
}

/** ---- Image blue component -------------------------------------------------
 * Image::blue
 * @brief Access to the blue component of pixel at row x and column y.
 */
core_inline
uint8_t *Image::blue(const uint32_t x, const uint32_t y)
{
    uint8_t *px = pixel(x,y);
    if (px && m_bpp > 16) {
        return (px + 2);  /* blue is third byte in a pixel with rgb layout */
    }
    return nullptr;
}

core_inline
const uint8_t *Image::blue(const uint32_t x, const uint32_t y) const
{
    const uint8_t *px = pixel(x,y);
    if (px && m_bpp > 16) {
        return (px + 2);  /* blue is third byte in a pixel with rgb layout */
    }
    return nullptr;
}

/** ---- Image alpha component ------------------------------------------------
 * Image::alpha
 * @brief Access to the alpha component of pixel at row x and column y.
 */
core_inline
uint8_t *Image::alpha(const uint32_t x, const uint32_t y)
{
    uint8_t *px = pixel(x,y);
    if (px && m_bpp > 24) {
        return (px + 3);  /* alpha is fourth byte in a pixel with rgb layout */
    }
    return nullptr;
}

core_inline
const uint8_t *Image::alpha(const uint32_t x, const uint32_t y) const
{
    const uint8_t *px = pixel(x,y);
    if (px && m_bpp > 24) {
        return (px + 3);  /* alpha is fourth byte in a pixel with rgb layout */
    }
    return nullptr;
}

/** ---- Image component swap functions ---------------------------------------
 * Image::swap
 * @brief Swap the image colour components ic and jc. Valid component indices
 * are smaller than the number of components per pixel(bpp/8 = bpp >> 3):
 *
 *  (0,1,2,3) for bpp = 32
 *  (0,1,2)   for bpp = 24
 *  (0,1)     for bpp = 16
 *
 * If ic = jc, no color is swapped.
 *
 * Set native to true to use the host machine native byte order endianess.
 * Otherwise, set native to false to assume little endian byte order and
 * swap pixel components using bitwise operations.
 */
core_inline
void Image::swap(const uint32_t ic, const uint32_t jc, bool native)
{
    /* Assert we have a valid bitmap. */
    core_assert(!is_empty(), "empty image bitmap");

    /*
     * Don't do anything if pixel component indices
     * are equal or outside the pixel stride range
     */
    const uint32_t pixel_bytes = m_bpp >> 3;
    if (ic == jc || ic > pixel_bytes || jc > pixel_bytes) {
        return;
    }

    /*
     * Swap the bitmap colour components.
     *
     * We can cast the bitmap as an array of 32-bit words because
     * the bitmap row stride is aligned to the a 32-bit boundary.
     *
     * If the pixel bitdepth is 16-bpp or 24-bpp, the last padding
     * 2 and 4 bytes respectively are ignored.
     */
    uint8_t *px = *m_bitmap;
    for (uint32_t y = 0; y < m_height; ++y) {
        for (uint32_t x = 0; x < m_width; ++x) {
            if (native) {
                swap_pix8(ic, jc, px);
            } else {
                swap_pix32(ic, jc, reinterpret_cast<uint32_t *>(px));
            }
            px += pixel_bytes;  /* next pixel */
        }
    }
}

/**
 * Image::swap_pix32
 * @brief Swap the color components of a pixel given by a pointer to its 32-bit
 * representation in the bitmap.
 *
 * @note The byte order representation is assumed to be little endian(least
 * significant byte first).
 *
 * @note We can cast the bitmap as an array of 32-bit words because the row
 * stride is aligned to a 32-bit boundary.
 *
 * @see https://stackoverflow.com/questions/6499183
 *      https://codereview.stackexchange.com/questions/199935
 */
core_inline
void Image::swap_pix32(const uint32_t ic, const uint32_t jc, uint32_t *pixel)
{
    /*
     * Const bitmask and bitshift arrays of each byte index.
     */
    static const std::array<uint32_t,4> mask = {0x000000FF,
                                                0x0000FF00,
                                                0x00FF0000,
                                                0xFF000000};
    static const std::array<uint32_t,4> shift = {0, 8, 16, 24};

    /*
     * Swap the ic- and jc-components of a given color.
     *
     * Move ic->jc by:
     *  1. extract the ic-component(col & mask[ic]),
     *  2. left shift by (8*ic) bits(>> shift[ic]),
     *  3. right shift by (8*jc) bits(<< shift[jc]).
     *
     * Conversely, jc->ic is the reverse of ic->jc:
     *  1. extract the jc-component(col & mask[jc]),
     *  2. left shift by (8*jc) bits(>> shift[jc]),
     *  3. right shift by (8*ic) bits(<< shift[ic]).
     *
     *  1. extract the jc-component(col & mask[jc]),
     *  2. shift right by(jc*8) bits(<< shift[jc]),
     *  3. shift left by(ic*8) bits(>> shift[ic]).
     *
     * Bitwise OR the shifted components with the complementary
     * (col & ~(mask[ic] | mask[jc])) to compute the new color.
     */
    *pixel = (((*pixel & mask[ic]) >> shift[ic]) << shift[jc]) | /* ic -> jc */
             (((*pixel & mask[jc]) >> shift[jc]) << shift[ic]) | /* jc -> ic */
               (*pixel & ~(mask[ic] | mask[jc]));                /* ~ic & ~jc */
}

/**
 * Image::swap_pix8
 * @brief Swap the color components of a given pixel given by its pointer
 * address in the bitmap.
 *
 * @note Use the native byte order representation to swap the color components.
 */
core_inline
void Image::swap_pix8(const uint32_t ic, const uint32_t jc, uint8_t *pixel)
{
    std::swap(pixel[ic], pixel[jc]);
}

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GRAPHICS_IMAGE_H_ */
