/*
 * mesh.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/graphics/mesh.hpp"

namespace atto {
namespace gl {

/** ---- Mesh -----------------------------------------------------------------
 * @brief Create a mesh with a given name bound to a shader program object
 * from a list of vertices and faces.
 *
 * The mesh maintains two vertex buffer objects - vbo and ebo - holding the
 * vertex attributes and the face vertex indices respectively. These are
 * accessed from the specified shader program object.
 * The shader program object needs to have three active vertex attributes:
 * position, color, and texture coordinates - regardless of whether these
 * will be used in the shader.
 *
 * The following convention is used here. Every mesh has a name that prefixes
 * the vertex attribute names, e.g. "world_sphere". The vertex attribute
 * variables in the shader are then assumed to be named accordingly as:
 *      world_sphere_position
 *      world_sphere_normal
 *      world_sphere_color
 *      world_sphere_texcoord
 *
 * The shader program pointer is queried for each vertex attribute.
 * The vertex buffer data is then specified by its location on the vertex
 * buffer object and by its format.
 *
 * The specified interpretation is stored in the currently bound vertex array
 * object and will be used when a draw call is made.
 */
Mesh::Mesh(
    const GLuint &program,
    const std::string &name,
    const std::vector<Vertex> &vertices,
    const std::vector<Face> &faces)
    : m_name(name)
    , m_vertices(vertices)
    , m_faces(faces)
{
    core_assert(!m_name.empty(), "invalid mesh name");
    core_assert(!m_vertices.empty(), "invalid mesh vertices");
    core_assert(!m_faces.empty(), "invalid mesh faces");

    /*
     * Create vertex array object.
     */
    m_vao = create_vertex_array();
    glBindVertexArray(m_vao);

    /*
     * Create a buffer storage for the vertex data with layout:
     *  {(xyz)_0,
     *   (nor)_0,
     *   (rgb)_0,
     *    (uv)_0,
     *      ...
     *   (xyz)_n
     *   (nor)_n
     *   (rgb)_n
     *    (uv)_n}
     */
    GLsizeiptr vertex_data_size = m_vertices.size() * sizeof(Vertex);
    m_vbo = create_buffer(
        GL_ARRAY_BUFFER,
        vertex_data_size,
        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,            /* target binding point */
        0,                          /* offset in data store */
        vertex_data_size,           /* data store size in bytes */
        m_vertices.data());         /* pointer to data source */

    /*
     * Create a buffer storage for the face indices with layout:
     *  {(v0,v1,v2)_0,
     *      ...
     *   v0,v1,v2)_n}
     */
    GLsizeiptr index_data_size = m_faces.size() * sizeof(Face);
    m_ebo = create_buffer(
        GL_ELEMENT_ARRAY_BUFFER,
        index_data_size,
        GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,    /* target binding point */
        0,                          /* offset in data store */
        index_data_size,            /* data store size in bytes */
        m_faces.data());            /* pointer to data source */

    /*
     * Specify how OpenGL interprets the mesh vertex attributes.
     */
    enable_attribute(program, m_name + std::string("_position"));
    attribute_pointer(
        program,
        m_name + std::string("_position"),
        GL_FLOAT_VEC3,
        11*sizeof(GLfloat), /* byte offset between consecutive attributes */
        0,                  /* byte offset of first element in the buffer */
        false);             /* normalized flag */

    /*
     * Specify how OpenGL interprets the mesh vertex attributes.
     */
    enable_attribute(program, m_name + std::string("_normal"));
    attribute_pointer(
        program,
        m_name + std::string("_normal"),
        GL_FLOAT_VEC3,
        11*sizeof(GLfloat), /* byte offset between consecutive attributes */
        3*sizeof(GLfloat),  /* byte offset of first element in the buffer */
        false);             /* normalized flag */

    enable_attribute(program, m_name + std::string("_color"));
    attribute_pointer(
        program,
        m_name + std::string("_color"),
        GL_FLOAT_VEC3,
        11*sizeof(GLfloat), /* byte offset between consecutive attributes */
        6*sizeof(GLfloat),  /* byte offset of first element in the buffer */
        false);             /* normalized flag */

    enable_attribute(program, m_name + std::string("_texcoord"));
    attribute_pointer(
        program,
        m_name + std::string("_texcoord"),
        GL_FLOAT_VEC2,
        11*sizeof(GLfloat), /* byte offset between consecutive attributes */
        9*sizeof(GLfloat),  /* byte offset of first element in the buffer */
        false);             /* normalized flag */

    /*
     * Unbind vertex array object.
     */
    glBindVertexArray(0);
}

/**
 * Mesh::copy
 * @brief Copy the mesh data to the vertex buffer object.
 */
void Mesh::copy(void) const
{
    GLsizeiptr vertex_data_size = m_vertices.size() * sizeof(gl::Mesh::Vertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,        /* target binding point */
        0,                      /* offset in data store */
        vertex_data_size,       /* data store size in bytes */
        m_vertices.data());     /* pointer to data source */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * Mesh::draw
 * @brief Mesh draw function.
 */
void Mesh::draw(void *data)
{
    /*
     * The number of elements to be rendered is the number of vertex
     * indices per primitive times the total number of primitives.
     */
    GLsizei n_elements = 3*m_faces.size();
    glBindVertexArray(m_vao);
    glDrawElements(
        GL_TRIANGLES,       /* what kind of primitives to render */
        n_elements,         /* number of elements to be rendered */
        GL_UNSIGNED_INT,    /* type of the values in indices */
        (GLvoid *) 0);      /* offset of first index in the data array */
    glBindVertexArray(0);
}

/** ---- Mesh factory functions -----------------------------------------------
 * Mesh::Grid
 * @brief Create a grid defined by an indexed face topology on a lattice with
 * (n1 * n2) vertices along the first and second dimensions:
 *
 *  (n1 - 1) * (n2 - 1) quads in a lattice with (n1 * n2) vertices,
 *  2 triangles per quad, or 2 * (n1 - 1)*(n2 - 1) lattice triangles,
 *  3 indices per triangle, or 6 * (n1 - 1)*(n2 - 1) triangle indices.
 *
 * The grid represents an orientable manifold with topology:
 *
 *      (i,j+1)--------(i+1,j+1)
 *           | \      |
 *           |  \     |
 *           |   \    |
 *           |    \   |
 *           |     \  |
 *      (i,j)--------(i+1,j)
 *
 *  where (0 <= i <= n1-1) and (0 <= j <= n2-1).
 *  The first triangle is represented by the vertices:
 *      {    (i,j), (i+1,j), (i,j+1)}
 *  The second triangle is represented by the vertices:
 *      {(i+1,j+1), (i,j+1), (i+1,j)}
 */
std::vector<Mesh::Face> Mesh::Grid(const size_t n1, const size_t n2)
{
    core_assert(n1 > 1 && n2 > 1, "invalid grid dimensions");

    std::vector<Mesh::Face> faces(2 * (n1 - 1) * (n2 - 1));

    for (size_t j = 0; j < n2-1; ++j) {
        for (size_t i = 0; i < n1-1; ++i) {
            size_t k, v0, v1, v2;

            /* first triangle(upward hypotenuse) */
            k = i + j*(n1-1);

            v0 = i   +     j*n1;
            v1 = i+1 +     j*n1;
            v2 = i   + (j+1)*n1;

            faces[k].index[0] = v0;
            faces[k].index[1] = v1;
            faces[k].index[2] = v2;

            /* second triangle(downward hypotenuse) */
            k = i + j*(n1-1) + (n1-1)*(n2-1);

            v0 = (i+1) + (j+1)*n1;
            v1 =     i + (j+1)*n1;
            v2 = (i+1) +     j*n1;

            faces[k].index[0] = v0;
            faces[k].index[1] = v1;
            faces[k].index[2] = v2;
        }
    }

    return faces;
}

/**
 * Mesh::Plane
 * @brief Create a plane represented by (n1 * n2) vertices on a rectangle
 * region in the xy-plane, bounded by lower (xlo, ylo) and upper (xhi, yhi).
 *
 * The vertex (x,y) coordinates are linearly interpolated between lower
 * (xlo, ylo) and upper (xhi, yhi) positions along the n1 and n2 lattice
 * directions.
 * The normal of each vertex points in the (0,0,1) direction.
 * The vertex colors (r,g,b) map the texture coordinates to a rg-colormap.
 * The vertex texture coordinates (u,v) are linearly interpolated between
 * (0,0) and (1,1).
 */
std::unique_ptr<Mesh> Mesh::Plane(
    const GLuint &program,
    const std::string &name,
    size_t n1,
    size_t n2,
    GLfloat xlo,
    GLfloat xhi,
    GLfloat ylo,
    GLfloat yhi)
{
    core_assert(n1 > 1 && n2 > 1, "invalid mesh dimensions");
    core_assert(xlo < xhi && ylo < yhi, "invalid coordinates");

    /* Create mesh vertices. */
    std::vector<Mesh::Vertex> vertices(n1 * n2);

    GLfloat dx = (xhi - xlo) / (GLfloat) (n1 - 1);
    GLfloat dy = (yhi - ylo) / (GLfloat) (n2 - 1);

    GLfloat du = 1.0f / (GLfloat) (n1 - 1);
    GLfloat dv = 1.0f / (GLfloat) (n2 - 1);

    for (size_t j = 0; j < n2; ++j) {
        for (size_t i = 0; i < n1; ++i) {
            size_t k = i + j*n1;

            /* Vertex positions are in the xy-plane by default. */
            vertices[k].position[0] = xlo + (GLfloat) i * dx;
            vertices[k].position[1] = ylo + (GLfloat) j * dy;
            vertices[k].position[2] = 0.0;

            /* Vertex normals point in the z-direction by default. */
            vertices[k].normal[0] = 0.0f;
            vertices[k].normal[1] = 0.0f;
            vertices[k].normal[2] = 1.0f;

            /* Encode the vertex colors with their uv-coordinates. */
            vertices[k].color[0] = (GLfloat) i * du;
            vertices[k].color[1] = (GLfloat) j * dv;
            vertices[k].color[2] = 0.0f;

            /* Vertex uv-coordinates lie in the unit square. */
            vertices[k].texcoord[0] = (GLfloat) i * du;
            vertices[k].texcoord[1] = (GLfloat) j * dv;
        }
    }

    /* Create mesh indexed face set. */
    std::vector<Mesh::Face> faces = Mesh::Grid(n1, n2);

    /* Create mesh. */
    return std::make_unique<Mesh>(program, name, vertices, faces);
}


/**
 * Mesh::Sphere
 * @brief Create a mesh with (n1 * n2) vertices, mapping vertex positions
 * onto a region in a sphere with a given radius and bounded by lower
 * (theta_lo, phi_lo) and upper (theta_hi, phi_hi) spherical coordinates.
 *
 * The vertex spherical coordinates are linearly interpolated between the
 * lower (theta_lo, phi_lo) and upper (theta_hi, phi_hi) points along n1
 * and n2 lattice directions.
 * The normal of each vertex points in outward radial direction as defined
 * by its spherical coordinates.
 * The vertex colors (r,g,b) map the texture coordinates to a rg-colormap.
 * The vertex texture coordinates (u,v) are linearly interpolated between
 * (0,0) and (1,1).
 */
std::unique_ptr<Mesh> Mesh::Sphere(
    const GLuint &program,
    const std::string &name,
    size_t n1,
    size_t n2,
    GLfloat radius,
    GLfloat theta_lo,
    GLfloat theta_hi,
    GLfloat phi_lo,
    GLfloat phi_hi)
{
    core_assert(n1 > 1 && n2 > 1, "invalid mesh dimensions");
    core_assert(radius > 0.0, "invalid radius");
    core_assert(theta_lo < theta_hi, "invalid polar angle");
    core_assert(phi_lo < phi_hi, "invalid azimuth angle");

    /* Create mesh vertices. */
    std::vector<Mesh::Vertex> vertices(n1*n2);

    GLfloat dtheta = (theta_hi - theta_lo) / (GLfloat) (n1 - 1);
    GLfloat dphi = (phi_hi - phi_lo) / (GLfloat) (n2 - 1);

    GLfloat du = 1.0f / (GLfloat) (n1 - 1);
    GLfloat dv = 1.0f / (GLfloat) (n2 - 1);

    for (size_t j = 0; j < n2; ++j) {
        for (size_t i = 0; i < n1; ++i) {
            size_t k = i + j*n1;

            /* Get the spherical */
            GLfloat theta = theta_lo + (GLfloat) j*dtheta;
            GLfloat phi = phi_lo + (GLfloat) i*dphi;

            /* Vertex positions are just the normal scaled by the radius. */
            vertices[k].position[0] = radius * std::sin(theta) * std::cos(phi);
            vertices[k].position[1] = radius * std::sin(theta) * std::sin(phi);
            vertices[k].position[2] = radius * std::cos(theta);

            /* Vertex normals point in the z-direction by default. */
            vertices[k].normal[0] = std::sin(theta) * std::cos(phi);
            vertices[k].normal[1] = std::sin(theta) * std::sin(phi);
            vertices[k].normal[2] = std::cos(theta);

            /* Encode the vertex colors with their uv-coordinates. */
            vertices[k].color[0] = (GLfloat) i * du;
            vertices[k].color[1] = (GLfloat) j * dv;
            vertices[k].color[2] = 0.0f;

            /* Vertex uv-coordinates lie in the unit square. */
            vertices[k].texcoord[0] = (GLfloat) i * du;
            vertices[k].texcoord[1] = (GLfloat) j * dv;
        }
    }

    /* Create mesh indexed face set. */
    std::vector<Mesh::Face> faces = Mesh::Grid(n1, n2);

    /* Create mesh. */
    return std::make_unique<Mesh>(program, name, vertices, faces);
}

} /* gl */
} /* atto */
