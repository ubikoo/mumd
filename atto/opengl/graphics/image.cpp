/*
 * image.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/graphics/image.hpp"

namespace atto {
namespace gl {

/**
 * Image::pixelformat
 * @brief Return the pixel base format congruous with the pixel bit depth.
 */
GLenum Image::pixelformat(void) const
{
    const uint32_t pixelsize = m_bpp / 8;
    return (pixelsize == 1 ? GL_RED  :
            pixelsize == 2 ? GL_RG   :
            pixelsize == 3 ? GL_RGB  :
            pixelsize == 4 ? GL_RGBA : GL_NONE);
}

/**
 * Image::resize
 * @brief Resize the image bitmap with dimensions width and height
 * in pixel units, and a pixel bit depth in bits per pixel.
 *
 * @note Any bitmap previously allocated will be destroyed and replaced
 * with the new one.
 */
void Image::resize(
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp)
{
    /*
     * Bitmap width and height dimensions in pixel units.
     */
    core_assert(width > 0, core::str_format("invalid width: %u", width));
    core_assert(height > 0, core::str_format("invalid height: %u", height));
    m_width = width;
    m_height = height;

    /*
     * Bitmap internal pixel bit depth layout in bits per pixel.
     */
    core_assert(bpp == 32 || bpp == 24 || bpp == 16 || bpp == 8,
        core::str_format("invalid internal pixel layout: %u\n", bpp));
    m_bpp = bpp;

    /*
     * Set the bitmap row stride in bytes, rounded to the next
     * 32-bit boundary, and with a pixel buffer size in bytes.
     */
    m_pitch = 4*(((m_width * m_bpp) + 31) / 32);
    m_size = m_height * m_pitch;

    /*
     * Create the pixel buffer array.
     */
    m_bitmap = std::move(core::make_align_unique_array<uint8_t>(m_size));
}

/**
 * Image::infolog
 * @brief Print image information log.
 */
std::string Image::infolog(const char *comment) const
{
    std::ostringstream ss;
    if (comment != nullptr) {
        ss << core::str_format("%s\n", comment);
    }

    ss << core::str_format(
        "width:    %u\n"
        "height:   %u\n"
        "bpp:      %u\n"
        "channels: %u\n"
        "pitch:    %u\n"
        "size:     %u\n"
        "bitmap:   %p\n",
        m_width,
        m_height,
        m_bpp,
        m_bpp >> 3,
        m_pitch,
        m_size,
        m_bitmap ? *m_bitmap : nullptr);

    return ss.str();
}

/**
 * Image::copy
 * @brief Copy image bitmap data to 2d-texture object.
 */
void Image::copy(const GLuint &texture) const
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,                  /* level of detail - 0 is base image */
        GL_RGBA8,           /* texture internal format */
        width(),            /* texture width */
        height(),           /* texture height */
        0,                  /* border parameter - must be 0 (legacy) */
        pixelformat(),      /* pixel format */
        GL_UNSIGNED_BYTE,   /* type of the pixel data(GLubyte) */
        bitmap());          /* pointer to the pixel data */
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * Image::load
 * @brief Load an image bitmap from a file and flip the image vertically if
 * required(do not flip by default). The number of desired pixel components
 * is 0(same as file data).
 */
void Image::load(
    const std::string &filename,
    const bool flip_vertically,
    const int32_t n_channels)
{
    /* Assert we have a valid filename. */
    core_assert(!filename.empty(), "invalid filename");

    unsigned char *data = NULL;
    try {
        /*
         * Load image data from the file using stb image loader:
         *
         *  x = width
         *  y = height
         *  n = # 8-bit components per pixel
         *  '0' = load all available components
         *
         * The pixel data consists of h-scanlines, each with w-pixels,
         * with each pixel consisting of n-interleaved 8-bit components.
         * The first pixel is at the top-left-most in the image.
         * There is no padding between image scanlines or between pixels,
         * regardless of the format.
         * An output image with n-components has the following interleaved
         * order in each pixel:
         *
         *  n = #comp      components
         *     1           grey
         *     2           grey, alpha
         *     3           red, green, blue
         *     4           red, green, blue, alpha
         */
        int w, h, n;
        stbi_set_flip_vertically_on_load(flip_vertically ? 1 : 0);
        data = stbi_load(filename.c_str(), &w, &h, &n, n_channels);
        core_assert(data != NULL,
            core::str_format("failed to load image %s", filename.c_str()));

        /*
         * Retrieve image width and height dimensions, in pixel
         * units, and corresponding pixel internal layout in
         * bits per pixel.
         */
        uint32_t width = (uint32_t) w;
        core_assert(width > 0, core::str_format("invalid width %u", width));

        uint32_t height = (uint32_t) h;
        core_assert(height > 0, core::str_format("invalid height %u", height));

        uint32_t bpp = (uint32_t) (8*(n_channels == 0 ? n : n_channels));
        core_assert(bpp == 32 || bpp == 24 || bpp == 16 || bpp == 8,
            core::str_format("invalid pixel layout: %u\n", bpp));

        /*
         * Resize the bitmap pixel buffer and copy the data.
         * There is no padding between image scanlines or between pixels,
         * regardless of format for a stb loaded image, so we just used
         * the image width to get the next source scanline.
         */
        core_assert(sizeof(uint8_t) == sizeof(unsigned char), "invalid size");
        resize(width, height, bpp);
        const uint32_t pixel_bytes = m_bpp >> 3;
        std::memcpy(*m_bitmap, data, m_width*m_height*pixel_bytes);

        /*
         * Free image data.
         */
        stbi_image_free(data);
    } catch (std::exception &e) {
        /*
         * Ensure image data is released before throwing.
         */
        stbi_image_free(data);
        core_throw(e.what());
    }
}

/**
 * Image::write_png
 * @brief Save a bitmap image to a file with a png format.
 */
void Image::write_png(const std::string &filename, const bool flip_vertically)
{
    /* Write bitmap image to file. Return 0 on failure and non-0 on success. */
    stbi_flip_vertically_on_write(flip_vertically ? 1 : 0);
    const uint32_t pixel_bytes = m_bpp >> 3;
    int rc = stbi_write_png(
        filename.c_str(),       /* name of the file */
        m_width,                 /* image width */
        m_height,                /* image height */
        pixel_bytes,            /* # components per pixel */
        *m_bitmap,               /* bitmap data */
        m_pitch);                /* scanline stride in bytes */
    core_assert(rc, "failed to write image");
}

/**
 * Image::write_ppma
 * @brief Write an image bitmap to a file using a simple PPM file format.
 * A PPM file consists of two parts, a header followed by the image data.
 * The PPM header starts with a two-byte number (as ASCII) identifying
 * the format of the image data, P3 for plain ASCII and P6 for binary.
 *
 * Plain PPM file layout. The layout of a plain PPM file is the same as
 * the binary one except for the following:
 *
 *  1. There is exactly one image in a file.
 *  2. The magic number is P3 instead of P6.
 *  3. Each sample in the raster is represented as an ASCII decimal number.
 *  4. Each sample in the raster has white space before and after it.
 *  5. No line should be longer than 70 characters.
 *
 * @see http://paulbourke.net/dataformats/ppm
 *      http://netpbm.sourceforge.net/doc/ppm.html
 *      https://en.wikipedia.org/wiki/Netpbm_format
 */
void Image::write_ppma(const std::string &filename, const bool flip_vertically)
{
    /* Assert we have a valid bitmap. */
    core_assert(!is_empty(), "empty image bitmap");

    core::FileOut fp;
    try {
        /* Write the image bitmap as plain PPM file. */
        fp.open(filename.c_str());

        /* Write header. */
        std::ostringstream header;
        header << "P3\n"
               << std::to_string(m_width)  << "#width\n"
               << std::to_string(m_height) << "#height\n"
               << std::to_string(255)      << "#colors\n";
        fp.writeline(header.str());

        /* Write bitmap data. */
        for (uint32_t y = 0; y < m_height; ++y) {
            for (uint32_t x = 0; x < m_width; ++x) {
                uint8_t *px = flip_vertically
                    ? pixel(x, m_height - 1 - y)
                    : pixel(x, y);

                uint8_t r = *px;
                uint8_t g = m_bpp > 8  ? *(px + 1) : 0;
                uint8_t b = m_bpp > 16 ? *(px + 2) : 0;

                fp.writeline(core::str_format("%u %u %u\n", r, g, b));
            }
        }

        /* Finished, close the PPM file and return. */
        fp.close();
    } catch (std::exception &e) {
        /* Ensure the file stream is closed before throwing. */
        fp.close();
        core_throw(e.what());
    }
}

/**
 * Image::write_ppmb
 * @brief Write an image bitmap to a file using a simple PPM file format.
 * A PPM file consists of two parts, a header followed by the image data.
 * The PPM header starts with a two-byte number (as ASCII) identifying
 * the format of the image data, P3 for plain ASCII and P6 for binary.
 *
 * Binary PPM file layout:
 *
 *  1. The two-byte "magic number" identifying the file type, P6.
 *  2. Whitespace(blanks, TABs, CRs, LFs).
 *  3. Image width in pixel units (as ASCII decimal).
 *  4. Whitespace.
 *  5. Image height in pixel units (as ASCII decimal).
 *  6. Whitespace.
 *  7. Maximum color value (as ASCII decimal).
 *     Must be less than 65536 and more than zero.
 *     The RGB format is assumed to be RGB888, i.e., 1 byte per
 *     color channel and a corresponding maximum value of 255.
 *  8. Whitespace(usually a newline).
 *  9. A raster of height rows, in order from top to bottom.
 *     Each row consists of width pixels, in order from left to right.
 *     Each pixel is a triplet of red, green, and blue samples, in that order.
 *     Each sample is represented in pure binary by either 1 or 2 bytes.
 *     If maximum color is less than 256, it is 1 byte. Otherwise,it is 2 bytes.
 *     The most significant byte is first.
 *
 * @see http://paulbourke.net/dataformats/ppm
 *      http://netpbm.sourceforge.net/doc/ppm.html
 *      https://en.wikipedia.org/wiki/Netpbm_format
 */
void Image::write_ppmb(const std::string &filename, const bool flip_vertically)
{
    /* Assert we have a valid bitmap. */
    core_assert(!is_empty(), "empty image bitmap");

    core::FileOut fp;
    try {
        /* Write the image bitmap as a binary PPM file. */
        fp.open(filename.c_str(), core::FileIn::Binary);

        /* Write header. */
        std::string header("P6\n" +
                           std::to_string(m_width)  + "#width\n" +
                           std::to_string(m_height) + "#height\n" +
                           std::to_string(255)      + "#colors\n");
        fp.write((void *) header.c_str(), header.size());

        /* Write bitmap data. */
        for (uint32_t y = 0; y < m_height; ++y) {
            for (uint32_t x = 0; x < m_width; ++x) {
                uint8_t *px = flip_vertically
                    ? pixel(x, m_height - 1 - y)
                    : pixel(x, y);

                uint8_t r = *px;
                uint8_t g = m_bpp > 8  ? *(px + 1) : 0;
                uint8_t b = m_bpp > 16 ? *(px + 2) : 0;

                fp.write((void *) &r, sizeof(uint8_t));
                fp.write((void *) &g, sizeof(uint8_t));
                fp.write((void *) &b, sizeof(uint8_t));
            }
        }

        /* Finished, close the PPM file and return. */
        fp.close();
    } catch (std::exception &e) {
        /* Ensure the file stream is closed before throwing. */
        fp.close();
        core_throw(e.what());
    }
}

} /* gl */
} /* atto */
