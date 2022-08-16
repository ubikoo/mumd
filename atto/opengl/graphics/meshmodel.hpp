/*
 * meshmodel.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GRAPHICS_MODEL_H_
#define ATTO_OPENGL_GRAPHICS_MODEL_H_

#include <cmath>
#include <vector>
#include "atto/opengl/base.hpp"
#include "atto/opengl/graphics/mesh.hpp"

namespace atto {
namespace gl {

/** ---- MeshModel ------------------------------------------------------------
 * @brief MeshModel maintains a collection of mesh objects and provides an
 * interface to Assimp's library.
 * MeshModel takes a filename of a format recognised by Assimp and loads an
 * Assimp model with the supported extensions. If successful, processes each
 * individual mesh in the Assimp scene and retrieves the vertices and faces.
 *
 * @see OpenGL mesh and polygon file format(ply):
 *      https://learnopengl.com/Model-Loading/MModel
 *      http://paulbourke.net/dataformats/ply
 */
struct MeshModel : Drawable {
    /** ---- MeshModel interface ----------------------------------------------
     * MeshModel member variables.
     */
    std::vector<std::unique_ptr<Mesh>> m_meshes;

    /**
     * Handle and draw member functions.
     */
    void handle(const Event &event) override {}
    void draw(void *data = nullptr) override;

    /** Process an Assimp mesh and retrieve vertex and face data. */
    void process(
        const aiMesh *mesh,
        std::vector<Mesh::Vertex> &vertices,
        std::vector<Mesh::Face> &faces);

    /**
     * MeshModel constructor/destructor.
     */
    MeshModel(const GLuint &program,
          const std::string &name,
          const std::string &filename);
    ~MeshModel() = default;

    /* Delete copy constructor/assignment. */
    MeshModel(const MeshModel &other) = delete;
    MeshModel &operator=(const MeshModel &other) = delete;
};

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GRAPHICS_MODEL_H_ */
