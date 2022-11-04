/*
 * mesh.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GRAPHICS_MESH_H_
#define ATTO_OPENGL_GRAPHICS_MESH_H_

#include <cmath>
#include <vector>
#include "atto/opengl/base.hpp"
#include "atto/opengl/buffer.hpp"
#include "atto/opengl/vertex-array.hpp"
#include "atto/opengl/glsl/program.hpp"
#include "atto/opengl/glsl/attribute.hpp"
#include "atto/opengl/graphics/drawable.hpp"

namespace atto {
namespace gl {

/** ---- Mesh -----------------------------------------------------------------
 * @brief Mesh maintains a face-vertex (ie, shared-vertex) representation of
 * a mesh. It consists of a collection of vertices and of a list of faces.
 * Each face is specified by a list of indices of the vertices forming its
 * boundary.
 * Each vertex is specified by its position, normal, color and texture
 * coordinates.
 * Edges are implicit in the sequence of vertices composing the face.
 *
 * @par Mesh Geometry
 * A mesh with (n1 * n2) vertices along the first and second dimensions is
 * defined as an orientable manifold with topology:
 *
 *      (i,j+1)--------(i+1,j+1)
 *           | \      |
 *           |  \     |
 *           |   \    |
 *           |    \   |
 *           |     \  |
 *      (i,j)--------(i+1,j)
 *
 *  where (0 <= i <= n1-1) and (0 <= j <= n2-1), resulting:
 *      (n1-1) * (n2-1) quads in a lattice with (n1 * n2) vertices,
 *      2 triangles per quad, or 2*(n1 - 1)*(n2 - 1) lattice triangles,
 *      3 indices per triangle, or 6*(n1 - 1)*(n2 - 1) triangle indices.
 *
 * @par Mesh Data structrure
 * Meshes are represented as a collection of vertices, edges and faces.
 * A mesh data structure maintains the mesh data:
 *  - geometry: vertex positions and normals.
 *  - attributes: vertex color and texture coordinates.
 *  - connectivity: adjency relationships between mesh elements.
 *
 * And supports a set of functions on that data:
 *  - rendering
 *  - geometry queries: vertex/edge/face queries.
 *  - manipulation: remove/add a vertex/face, vertex split, edge collapse.
 *
 * @par Shared-vertex representation
 * Shared-vertex representations are compact and easy to populate.
 * It allows constant time complexity O(1) lookups of the vertices
 * of a given face.
 *
 * Indexed rendering requires a list of vertices of each face to be
 * transmitted to the GPU. The vertices themselves are stored in a
 * vertex buffer object holding the position/color/normal structures.
 *
 * Changes in shape, but not geometry, can be dynamically updated
 * by simply resending the vertex data without updating the face
 * connectivity.
 *
 * Shared-vertex representations are also static and encode little
 * information regarding the structure of the mesh - traversals and
 * mesh manipulation are inefficient.
 *
 * @par Half-edge representation
 * Half-edge is a boundary representation that models explicitly the
 * vertices, edges, and faces of the mesh with additional adjacency
 * information stored inside.
 * This permits a more efficient manipulation of the mesh - edge
 * collapsing, mesh traversal, adjacency queries:
 *  - which faces use this vertex?
 *  - which edges use this vertex?
 *  - which faces border this edge?
 *  - which edges border this face?
 *  - which faces are adjacent to this face?
 *
 * @see OpenGL mesh and polygon file format(ply):
 *      https://learnopengl.com/Model-Loading/Mesh
 *      http://paulbourke.net/dataformats/ply
 *
 *      Mesh data structures:
 *      https://www.redblobgames.com/x/1722-b-rep-triangle-meshes
 *      https://en.wikipedia.org/wiki/Polygon_mesh
 *      https://www.flipcode.com/archives/The_Half-Edge_Data_Structure.shtml
 *      https://towardsdatascience.com/mesh-data-structure-d8b1a61d749e
 *      https://martindevans.me/game-development/2016/03/30/
 *          Procedural-Generation-For-Dummies-Half-Edge-Geometry
 */
struct Mesh : Drawable {
    /** ---- Mesh helper structures -------------------------------------------
     * @brief Vertex represents the geometry (position and normal) and
     * attributes (colour and texture coordinates) of the mesh.
     */
    struct Vertex {
        GLfloat position[3];
        GLfloat normal[3];
        GLfloat color[3];
        GLfloat texcoord[2];
    };

    /**
     * @brief Face is specified by the indices in shared vertex represention.
     */
    struct Face {
        GLuint index[3];
    };

    /** ---- Mesh interface ---------------------------------------------------
     * Mesh member variables.
     */
    std::string m_name;                 /* mesh name */
    std::vector<Vertex> m_vertices;     /* vertex list */
    std::vector<Face> m_faces;          /* indexed face list */
    GLuint m_vao;                       /* vertex array object */
    GLuint m_vbo;                       /* vertex buffer object */
    GLuint m_ebo;                       /* element buffer object */

    /** Mesh accessors/mutators. */
    std::vector<Vertex> &vertices(void) { return m_vertices; }
    const std::vector<Vertex> &vertices(void) const { return m_vertices; }

    Vertex &vertex(size_t index) { return m_vertices[index]; }
    const Vertex &vertex(size_t index) const { return m_vertices[index]; }

    std::vector<Face> &faces(void) { return m_faces; }
    const std::vector<Face> &faces(void) const { return m_faces; }

    /** Copy the mesh data to the vertex buffer object. */
    void copy(void) const;

    /** Handle and draw member functions. */
    void handle(const Event &event) override {}
    void draw(void *data = nullptr) override;

    /**
     * Mesh factory functions.
     */
    static std::vector<Mesh::Face> Grid(const size_t n1, const size_t n2);

    static std::unique_ptr<Mesh> Plane(
        const GLuint &program,
        const std::string &name,
        size_t n1,
        size_t n2,
        GLfloat xlo,
        GLfloat xhi,
        GLfloat ylo,
        GLfloat yhi);

    static std::unique_ptr<Mesh> Sphere(
        const GLuint &program,
        const std::string &name,
        size_t n1,
        size_t n2,
        GLfloat radius,
        GLfloat theta_lo,
        GLfloat theta_hi,
        GLfloat phi_lo,
        GLfloat phi_hi);

    static std::unique_ptr<Mesh> Load(const std::string &filename);

    /**
     * Mesh constructor/destructor.
     */
    Mesh(const GLuint &program,
         const std::string &name,
         const std::vector<Vertex> &vertices,
         const std::vector<Face> &faces);
    ~Mesh() = default;

    /* Delete copy constructor/assignment. */
    Mesh(const Mesh &other) = delete;
    Mesh &operator=(const Mesh &other) = delete;
};

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GRAPHICS_MESH_H_ */
