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

#include <memory>
#include "atto/opengl/opengl.hpp"
#include "rop.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * Rop::Rop
 * @brief
 * Render a textured sphere with an image onto a framebuffer render texture.
 * Render a textured quad with the framebuffer render texture.
 */
Rop::Rop(const std::string &filename, const size_t n1, const size_t n2)
{
    /*
     * Create the textured sphere vao.
     */
    {
        std::vector<GLuint> shaders{
            gl::create_shader(GL_VERTEX_SHADER, "data/sphere.vert"),
            gl::create_shader(GL_FRAGMENT_SHADER, "data/sphere.frag")};
        m_sphere_program = gl::create_program(shaders);
        std::cout << gl::get_program_info(m_sphere_program) << "\n";

        /*
         * Load the 2d-image from the specified filename.
         */
        std::unique_ptr<gl::Image> image = std::make_unique<gl::Image>(
            filename,
            false);
        m_sphere_texture = gl::create_texture2d(
            GL_RGBA,                    /* texture internal format */
            image->width(),             /* texture width */
            image->height(),            /* texture height */
            image->pixelformat(),       /* pixel format */
            GL_UNSIGNED_BYTE,           /* pixel type */
            image->bitmap());           /* pixel data */
        glBindTexture(GL_TEXTURE_2D, m_sphere_texture);
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
         * Create a mesh over a sphere with screen size
         * and set the mesh vertex attributes in the program.
         */
        m_sphere_mesh = gl::Mesh::Sphere(
            m_sphere_program,            /* shader program object */
            "sphere",                   /* vertex attributes prefix */
            n1,                         /* n1 vertices */
            n2,                         /* n2 vertices */
            1.0,                        /* radius */
            0.0,                        /* phi_lo */
            M_PI,                       /* phi_hi */
            -M_PI,                      /* theta_lo */
            M_PI);                      /* theta_hi */
    }

    /*
     * Create the quad vao.
     */
    {
        std::vector<GLuint> shaders{
            gl::create_shader(GL_VERTEX_SHADER, "data/quad.vert"),
            gl::create_shader(GL_FRAGMENT_SHADER, "data/quad.frag")};
        m_quad_program = gl::create_program(shaders);
        std::cout << gl::get_program_info(m_quad_program) << "\n";

        /*
         * Create a mesh over a rectangle with screen size
         * and set the mesh vertex attributes in the program.
         */
        m_quad_mesh = gl::Mesh::Plane(
            m_quad_program,              /* shader program object */
            "quad",                     /* vertex attributes prefix */
            n1,                         /* n1 vertices */
            n2,                         /* n2 vertices */
            -1.0,                       /* xlo */
            1.0,                        /* xhi */
            -1.0,                       /* ylo */
            1.0);                       /* yhi */
    }

    /*
     * Create a framebuffer with color and depth attachments.
     */
    {
        glBindTexture(GL_TEXTURE_2D, m_sphere_texture);
        m_fbo_width = gl::get_texture_width(GL_TEXTURE_2D);
        m_fbo_height = gl::get_texture_height(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        m_fbo = gl::create_framebuffer(
            m_fbo_width,
            m_fbo_height,
            1,                      /* 1 color attachment */
            GL_RGBA,                /* color buffer internal format */
            &m_fbo_color_texture,
            GL_DEPTH_COMPONENT24,   /* depth buffer internal format */
            &m_fbo_depth_texture,
            GL_LINEAR,
            GL_LINEAR_MIPMAP_LINEAR);
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

    if (event.type == gl::Event::MouseScroll) {
        texmix(texmix() + 0.01*event.mousescroll.yoffset);
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
     * Update the drawable state.
     */
    {
        /* Compute the orthographic projection matrix */
        float time = (float) glfwGetTime();

        /* Update the sphere modelviewprojection matrix */
        {
            float ang_x = 1.0f * (float) M_PI;
            float ang_y = 0.8f * time;
            float ang_z = 0.6f * time;

            math::mat4f m = math::mat4f::eye;
            m = math::rotate(m, math::vec3f(1.0f, 0.0f, 0.0f), ang_x);

            math::vec4f dir_y = math::dot(m, math::vec4f(0.0f,1.0f,0.0f,1.0f));
            m = math::rotate(m, math::vec3f(dir_y), ang_y);

            math::vec4f dir_z = math::dot(m, math::vec4f(0.0f,0.0f,1.0f,1.0f));
            m = math::rotate(m, math::vec3f(dir_z), ang_z);

            std::array<GLfloat,2> size = gl::Renderer::framebuffer_sizef();
            float ratio = size[0] / size[1];

            math::mat4f proj = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
            m_sphere_mvp = math::dot(proj, m);
        }

        /* Update the quad modelviewprojection matrix */
        {
            float ang_x = 0.4f * time;
            float ang_y = 0.4f * time;
            float ang_z = 0.4f * time;

            math::mat4f m = math::mat4f::eye;
            m = math::rotate(m, math::vec3f(0.0f, 0.0f, 1.0f), ang_z);
            m = math::rotate(m, math::vec3f(0.0f, 1.0f, 0.0f), ang_y);
            m = math::rotate(m, math::vec3f(1.0f, 0.0f, 0.0f), ang_x);

            float ratio = (float) m_fbo_width / m_fbo_height;
            math::mat4f proj = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
            m_quad_mvp = math::dot(proj, m);
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
     * Render into the framebuffer rendertexture
     */
    {
        /* Bind the framebuffer */
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
        auto viewport = gl::Renderer::viewport();
        gl::Renderer::viewport({0, 0, m_fbo_width, m_fbo_height});
        gl::Renderer::clear(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

        /* Bind the sphere shader */
        glUseProgram(m_sphere_program);

        /* Set window dimensions. */
        float width = (float) m_fbo_width;
        float height = (float) m_fbo_height;
        gl::set_uniform(m_sphere_program, "u_mix", GL_FLOAT, &m_texmix);
        gl::set_uniform(m_sphere_program, "u_width", GL_FLOAT, &width);
        gl::set_uniform(m_sphere_program, "u_height", GL_FLOAT, &height);
        gl::set_uniform_matrix(
            m_sphere_program,
            "u_mvp",
            GL_FLOAT_MAT4,
            true,
            m_sphere_mvp.data());

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::set_uniform(m_sphere_program, "u_texsampler", GL_SAMPLER_2D, &texunit);
        gl::active_bind_texture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, m_sphere_texture);

        /* Draw the sphere mesh */
        m_sphere_mesh->draw();

        /* Unbind the shader program object. */
        glUseProgram(0);

        /* Unbind the framebuffer */
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        gl::Renderer::viewport(viewport);
        gl::Renderer::clear(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
    }

    /*
     * Render into the window framebuffer
     */
    {
        /* Get window dimensions. */
        std::array<GLfloat,2> sizef = gl::Renderer::framebuffer_sizef();

        /* Bind the quad shader */
        glUseProgram(m_quad_program);
        gl::set_uniform(m_quad_program, "u_mix", GL_FLOAT, &m_texmix);
        gl::set_uniform(m_quad_program, "u_width", GL_FLOAT, &sizef[0]);
        gl::set_uniform(m_quad_program, "u_height", GL_FLOAT, &sizef[0]);
        gl::set_uniform_matrix(
            m_quad_program,
            "u_mvp",
            GL_FLOAT_MAT4,
            true,
            m_quad_mvp.data());

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::set_uniform(m_quad_program, "u_texsampler", GL_SAMPLER_2D, &texunit);
        gl::active_bind_texture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, m_fbo_color_texture);

        /* Draw the quad mesh */
        m_quad_mesh->draw();

        /* Unbind the shader program object. */
        glUseProgram(0);
    }
}
