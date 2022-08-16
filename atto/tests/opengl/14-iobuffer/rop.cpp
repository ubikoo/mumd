/*
 * rop.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/opengl.hpp"
#include "iobuffer.hpp"
#include "rop.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * Rop::Rop
 * @brief Map render operator.
 * Begin by rendering the image to the map framebuffer.
 * Run the map shader program over a double framebuffer.
 * End by rendering the map framebuffer to the screen.
 */
Rop::Rop(const std::string &filename, const size_t width, const size_t height)
{
    /*
     * Map begin shader
     */
    {
        /*
         * Build the begin shader program.
         */
        std::vector<GLuint> shaders{
            gl::create_shader(GL_VERTEX_SHADER, "data/map-begin.vert"),
            gl::create_shader(GL_FRAGMENT_SHADER, "data/map-begin.frag")};
        map_begin.program = gl::create_program(shaders);
        std::cout << gl::get_program_info(map_begin.program) << "\n";

        /*
         * Load the 2d-image from the specified filename.
         */
        gl::Image image(filename, true);
        map_begin.texture = gl::create_texture2d(
            GL_RGBA8,                   /* texture internal format */
            image.width(),              /* texture width */
            image.height(),             /* texture height */
            image.pixelformat(),        /* pixel format */
            GL_UNSIGNED_BYTE,           /* pixel type */
            image.bitmap());            /* pixel data */

        glBindTexture(GL_TEXTURE_2D, map_begin.texture);
        gl::set_texture_mipmap(
            GL_TEXTURE_2D,
            GL_TRUE);                   /* generate mipmap */
        gl::set_texture_wrap(
            GL_TEXTURE_2D,
            GL_CLAMP_TO_EDGE,           /* wrap_s */
            GL_CLAMP_TO_EDGE);          /* wrap_t */
        gl::set_texture_filter(
            GL_TEXTURE_2D,
            GL_LINEAR,                  /* filter_min */
            GL_LINEAR);                 /* filter_mag */
        glBindTexture(GL_TEXTURE_2D, 0);

        /*
         * Create a mesh and set the vertex attributes in the program.
         */
        map_begin.quad = gl::Mesh::Plane(
            map_begin.program,      /* shader program object */
            "quad",                 /* vertex attributes prefix */
            2,                      /* n1 vertices */
            2,                      /* n2 vertices */
            -1.0,                   /* xlo */
             1.0,                   /* xhi */
            -1.0,                   /* ylo */
             1.0);                  /* yhi */
    }

    /*
     * Map run shader
     */
    {
        /*
         * Build the run shader program.
         */
        std::vector<GLuint> shaders{
            gl::create_shader(GL_VERTEX_SHADER, "data/map-run.vert"),
            gl::create_shader(GL_FRAGMENT_SHADER, "data/map-run.frag")};
        map_run.program = gl::create_program(shaders);
        std::cout << gl::get_program_info(map_run.program) << "\n";

        /*
         * Create a mesh and set the vertex attributes in the program.
         */
        map_run.quad = gl::Mesh::Plane(
            map_run.program,        /* shader program object */
            "quad",                 /* vertex attributes prefix */
            2,                      /* n1 vertices */
            2,                      /* n2 vertices */
            -1.0,                   /* xlo */
             1.0,                   /* xhi */
            -1.0,                   /* ylo */
             1.0);                  /* yhi */

        /*
         * Create the map input/output framebuffer objects.
         */
        map_run.buffer[map_run.read_ix] = std::make_unique<IOBuffer>(width, height, GL_RGB32F);
        map_run.buffer[map_run.write_ix] = std::make_unique<IOBuffer>(width, height, GL_RGB32F);
        map_run.width = width;
        map_run.height = height;
    }

    /*
     * Map end shader
     */
    {
        /*
         * Build the run shader program.
         */
        std::vector<GLuint> shaders{
            gl::create_shader(GL_VERTEX_SHADER, "data/map-end.vert"),
            gl::create_shader(GL_FRAGMENT_SHADER, "data/map-end.frag")};
        map_end.program = gl::create_program(shaders);
        std::cout << gl::get_program_info(map_end.program) << "\n";

        /*
         * Create a mesh and set the vertex attributes in the program.
         */
        map_end.quad = gl::Mesh::Plane(
            map_end.program,        /* shader program object */
            "quad",                 /* vertex attributes prefix */
            2,                      /* n1 vertices */
            2,                      /* n2 vertices */
            -1.0,                   /* xlo */
             1.0,                   /* xhi */
            -1.0,                   /* ylo */
             1.0);                  /* yhi */
    }
}

/** ---------------------------------------------------------------------------
 * Rop::handle
 * @brief Handle the event.
 */
void Rop::handle(const gl::Event &event)
{
    GLFWwindow *window = gl::Renderer::window();
    if (window == nullptr) {
        return;
    }

    if (event.type == gl::Event::Key &&
        event.key.code == GLFW_KEY_UP) {
        if (map_run.iterations <
            static_cast<size_t>(map_run.width * map_run.height)) {
            map_run.iterations++;
        }
        map_run.iterations++;
        std::cout << "map_run.iterations " << map_run.iterations << "\n";
    }
    if (event.type == gl::Event::Key &&
        event.key.code == GLFW_KEY_DOWN) {
        if (map_run.iterations > 0) {
            map_run.iterations--;
        }
        std::cout << "map_run.iterations " << map_run.iterations << "\n";
    }
}

/** ---------------------------------------------------------------------------
 * Rop::draw
 * @brief Render the drawable.
 */
void Rop::draw(void *data)
{
    GLFWwindow *window = gl::Renderer::window();
    if (window == nullptr) {
        return;
    }

    /*
     * Specify draw state modes.
     */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /*
     * Map begin shader.
     */
    {
        /* Swap read/write buffer indices. */
        std::swap(map_run.read_ix, map_run.write_ix);

        /* Bind the framebuffer for writing */
        map_run.buffer[map_run.write_ix]->bind();
        auto viewport = gl::Renderer::viewport();
        gl::Renderer::viewport({0, 0, map_run.width, map_run.height});
        gl::Renderer::clear(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

        /* Bind the begin shader */
        glUseProgram(map_begin.program);

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::set_uniform(map_begin.program, "u_texsampler", GL_SAMPLER_2D, &texunit);
        gl::active_bind_texture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, map_begin.texture);

        /* Draw the quad mesh */
        map_begin.quad->draw();

        /* Unbind the shader program object. */
        glUseProgram(0);

        /* Unbind the framebuffer */
        map_run.buffer[map_run.write_ix]->unbind();
        gl::Renderer::viewport(viewport);
        gl::Renderer::clear(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
    }

    /*
     * Map run shader.
     */
    for (size_t iter = 0; iter < map_run.iterations; ++iter) {
        /* Swap read/write buffer indices. */
        std::swap(map_run.read_ix, map_run.write_ix);

        /* Bind the framebuffer for writing */
        map_run.buffer[map_run.write_ix]->bind();
        auto viewport = gl::Renderer::viewport();
        gl::Renderer::viewport({0, 0, map_run.width, map_run.height});
        gl::Renderer::clear(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

        /* Bind the begin shader */
        glUseProgram(map_run.program);

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::set_uniform(map_run.program, "u_texsampler",  GL_SAMPLER_2D, &texunit);
        gl::active_bind_texture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            map_run.buffer[map_run.read_ix]->color_texture());

        /* Draw the quad mesh */
        map_run.quad->draw();

        /* Unbind the shader program object. */
        glUseProgram(0);

        /* Unbind the framebuffer */
        map_run.buffer[map_run.write_ix]->unbind();
        gl::Renderer::viewport(viewport);
        gl::Renderer::clear(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
    }

    /*
     * Map end shader.
     */
    {
        /* Swap read/write buffer indices. */
        std::swap(map_run.read_ix, map_run.write_ix);

        /* Bind the begin shader */
        glUseProgram(map_end.program);

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::set_uniform(map_end.program, "u_texsampler", GL_SAMPLER_2D, &texunit);
        gl::active_bind_texture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            map_run.buffer[map_run.read_ix]->color_texture());

        /* Draw the quad mesh */
        map_end.quad->draw();

        /* Unbind the shader program object. */
        glUseProgram(0);
    }
}
