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
#include "rop.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * Rop::Rop
 */
Rop::Rop(
    const std::string &filename1,
    const std::string &filename2,
    const size_t n1,
    const size_t n2,
    const int width,
    const int height)
{
    /*
     * Create image drawable
     */
    {
        /*
         * Create the shader program object.
         */
        std::vector<GLuint> shaders{
            gl::create_shader(GL_VERTEX_SHADER, "data/drawimg.vert"),
            gl::create_shader(GL_FRAGMENT_SHADER, "data/drawimg.frag")};
        m_drawimg.program = gl::create_program(shaders);
        std::cout << gl::get_program_info(m_drawimg.program) << "\n";

        /*
         * Load the first image from the specified filename.
         */
        m_drawimg.image1 = std::make_unique<gl::Image>(filename1, true);
        m_drawimg.texture1 = gl::create_texture2d(
            GL_RGBA8,                           /* texture internal format */
            m_drawimg.image1->width(),          /* texture width */
            m_drawimg.image1->height(),         /* texture height */
            m_drawimg.image1->pixelformat(),    /* pixel format */
            GL_UNSIGNED_BYTE,                   /* pixel type */
            m_drawimg.image1->bitmap());        /* pixel data */

        glBindTexture(GL_TEXTURE_2D, m_drawimg.texture1);
        gl::set_texture_mipmap(
            GL_TEXTURE_2D,
            GL_TRUE);                       /* generate mipmap */
        gl::set_texture_wrap(
            GL_TEXTURE_2D,
            GL_CLAMP_TO_EDGE,               /* wrap_s */
            GL_CLAMP_TO_EDGE);              /* wrap_t */
        gl::set_texture_filter(
            GL_TEXTURE_2D,
            GL_LINEAR,                      /* filter_min */
            GL_LINEAR);                     /* filter_mag */
        glBindTexture(GL_TEXTURE_2D, 0);

        /*
         * Load the second image from the specified filename.
         */
        m_drawimg.image2 = std::make_unique<gl::Image>(filename2, true);
        m_drawimg.texture2 = gl::create_texture2d(
            GL_RGBA8,                       /* texture internal format */
            m_drawimg.image2->width(),        /* texture width */
            m_drawimg.image2->height(),       /* texture height */
            m_drawimg.image2->pixelformat(),  /* pixel format */
            GL_UNSIGNED_BYTE,               /* pixel type */
            m_drawimg.image2->bitmap());      /* pixel data */

        glBindTexture(GL_TEXTURE_2D, m_drawimg.texture2);
        gl::set_texture_mipmap(
            GL_TEXTURE_2D,
            GL_TRUE);                       /* generate mipmap */
        gl::set_texture_wrap(
            GL_TEXTURE_2D,
            GL_CLAMP_TO_EDGE,               /* wrap_s */
            GL_CLAMP_TO_EDGE);              /* wrap_t */
        gl::set_texture_filter(
            GL_TEXTURE_2D,
            GL_LINEAR,                      /* filter_min */
            GL_LINEAR);                     /* filter_mag */
        glBindTexture(GL_TEXTURE_2D, 0);

        /*
         * Create a mesh over a rectangle with screen size and set the mesh
         * vertex attributes in the program.
         */
        m_drawimg.quad = gl::Mesh::Plane(
            m_drawimg.program,                /* shader program object */
            "quad",                         /* vertex attributes prefix name */
            n1,                             /* n1 vertices */
            n2,                             /* n2 vertices */
            -1.0,                           /* xlo */
             1.0,                           /* xhi */
            -1.0,                           /* ylo */
             1.0);                          /* yhi */
    }

    /*
     * Create framebuffer drawable
     */
    {
        /*
         * Create the shader program object
         */
        std::vector<GLuint> shaders{
            gl::create_shader(GL_VERTEX_SHADER, "data/drawfbo.vert"),
            gl::create_shader(GL_FRAGMENT_SHADER, "data/drawfbo.frag")};
        m_drawfbo.program = gl::create_program(shaders);
        std::cout << gl::get_program_info(m_drawfbo.program) << "\n";

        /*
         * Create a mesh over a rectangle with screen size and set the mesh
         * vertex attributes in the program.
         */
        m_drawfbo.quad = gl::Mesh::Plane(
            m_drawfbo.program,                /* shader program object */
            "quad",                         /* vertex attributes prefix name */
            2,                              /* n1 vertices */
            2,                              /* n2 vertices */
            -1.0,                           /* xlo */
             1.0,                           /* xhi */
            -1.0,                           /* ylo */
             1.0);                          /* yhi */

        /*
         * Create a framebuffer with color and depth attachments.
         */
        m_drawfbo.width = width;
        m_drawfbo.height = height;
        m_drawfbo.framebuffer = gl::create_framebuffer(
            m_drawfbo.width,
            m_drawfbo.height,
            1,                      /* 1 color attachment */
            GL_RGBA,                /* color buffer internal format */
            &m_drawfbo.color_texture,
            GL_DEPTH_COMPONENT24,   /* depth buffer internal format */
            &m_drawfbo.depth_texture,
            GL_LINEAR,
            GL_LINEAR_MIPMAP_LINEAR);
    }

    /*
     * Set window framebuffer.
     */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/** ---------------------------------------------------------------------------
 * Rop::handle
 */
void Rop::handle(const gl::Event &event)
{}

/** ---------------------------------------------------------------------------
 * Rop::draw
 */
void Rop::draw(void *data)
{
    GLFWwindow *window = gl::Renderer::window();
    if (window == nullptr) {
        return;
    }

    /*
     * Update drawable state.
     */
    {
        /* Update the img projection matrix and mixing alpha fraction */
        {
            float time = (float) glfwGetTime();

            float ang_x = 1.6 * time;
            float ang_y = 0.8 * time;
            float ang_z = 0.4 * time;

            math::mat4f m = math::mat4f::eye;
            m = math::rotate(m, math::vec3f(0.0f, 0.0f, 1.0f), ang_z);
            m = math::rotate(m, math::vec3f(0.0f, 1.0f, 0.0f), ang_y);
            m = math::rotate(m, math::vec3f(1.0f, 0.0f, 0.0f), ang_x);

            std::array<GLfloat,2> size = gl::Renderer::framebuffer_sizef();
            float ratio = size[0] / size[1];
            math::mat4f p = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);

            m_drawimg.mvp = dot(p, m);
            m_drawimg.alpha = 0.5*(std::cos(0.2 * time) + 1.0);
        }

        /* Update the modelviewprojection matrix */
        {
            float time = (float) glfwGetTime();

            float ang_x = 0.1f * time;
            float ang_y = 0.1f * time;
            float ang_z = 0.1f * time;

            math::mat4f m = math::mat4f::eye;
            m = math::rotate(m, math::vec3f(0.0f, 0.0f, 1.0f), ang_z);
            m = math::rotate(m, math::vec3f(0.0f, 1.0f, 0.0f), ang_y);
            m = math::rotate(m, math::vec3f(1.0f, 0.0f, 0.0f), ang_x);

            float ratio = (float) m_drawfbo.width / m_drawfbo.height;
            math::mat4f p = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
            m_drawfbo.mvp = dot(p, m);
        }
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
     * Render into the framebuffer texture
     */
    {
        /* Bind the framebuffer object and clear the color and depth buffers */
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_drawfbo.framebuffer);

        std::array<GLint, 4> viewport = gl::Renderer::viewport();
        gl::Renderer::viewport({0, 0, m_drawfbo.width, m_drawfbo.height});
        gl::Renderer::clear(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

        /* Bind the shader program object. */
        glUseProgram(m_drawimg.program);

        /* Set fbo uniforms. */
        float width = (float) m_drawfbo.width;
        float height = (float) m_drawfbo.height;

        gl::set_uniform(m_drawimg.program, "u_width", GL_FLOAT, &width);
        gl::set_uniform(m_drawimg.program, "u_height", GL_FLOAT, &height);
        gl::set_uniform(m_drawimg.program, "u_alpha", GL_FLOAT, &m_drawimg.alpha);
        gl::set_uniform_matrix(
            m_drawimg.program,
            "u_mvp",
            GL_FLOAT_MAT4,
            true,
            m_drawimg.mvp.data());

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit1 = 0;
        gl::set_uniform(m_drawimg.program, "u_texsampler1", GL_SAMPLER_2D, &texunit1);
        gl::active_bind_texture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit1, m_drawimg.texture1);

        GLenum texunit2 = 1;
        gl::set_uniform(m_drawimg.program, "u_texsampler2", GL_SAMPLER_2D, &texunit2);
        gl::active_bind_texture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit2, m_drawimg.texture2);

        /* Draw the mesh */
        m_drawimg.quad->draw();

        /* Unbind the shader program object. */
        glUseProgram(0);

        /* Restore the framebuffer */
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        gl::Renderer::viewport(viewport);
    }

    /*
     * Render into the window framebuffer
     */
    {
        /* Get window dimensions. */
        std::array<GLfloat,2> sizef = gl::Renderer::framebuffer_sizef();

        /* Bind the quad shader */
        glUseProgram(m_drawfbo.program);
        gl::set_uniform(m_drawfbo.program, "u_width", GL_FLOAT, &sizef[0]);
        gl::set_uniform(m_drawfbo.program, "u_height", GL_FLOAT, &sizef[1]);
        gl::set_uniform_matrix(
            m_drawfbo.program,
            "u_mvp",
            GL_FLOAT_MAT4,
            true,
            m_drawfbo.mvp.data());

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::set_uniform(m_drawfbo.program, "u_texsampler", GL_SAMPLER_2D, &texunit);
        gl::active_bind_texture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, m_drawfbo.color_texture);

        /* Draw the quad mesh */
        m_drawfbo.quad->draw();

        /* Unbind the shader program object. */
        glUseProgram(0);
    }
}
