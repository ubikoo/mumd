/*
 * meshmodel.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/graphics/meshmodel.hpp"

namespace atto {
namespace gl {

/** ---- MeshModel ------------------------------------------------------------
 * @brief
 * Interface to Assimp's library.
 */
MeshModel::MeshModel(
    const GLuint &program,
    const std::string &name,
    const std::string &filename)
{
    /*
     * Read file via Assimp
     * aiProcess_Triangulate ensures triangles are the model's only primitive.
     * aiProcess_GenSmoothNormals computes normal vectors for each vertex.
     */
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filename,
        aiProcess_Triangulate       |
        aiProcess_GenSmoothNormals  |
        aiProcess_CalcTangentSpace);
    core_assert(scene != NULL, importer.GetErrorString());

    /*
     * Initialize the meshes in the scene one by one.
     */
    core_assert(scene->HasMeshes(), "scene contains no meshes");
    for (size_t i = 0; i < scene->mNumMeshes; ++i) {
        std::vector<Mesh::Vertex> vertices;
        std::vector<Mesh::Face> faces;
        process(scene->mMeshes[i], vertices, faces);
        m_meshes.push_back(std::make_unique<Mesh>(
            program, name, vertices, faces));
    }
}

/**
 * MeshModel::process
 * @brief Process an Assimp mesh and retrieve vertex and face data.
 */
void MeshModel::process(
    const aiMesh *mesh,
    std::vector<Mesh::Vertex> &vertices,
    std::vector<Mesh::Face> &faces)
{
    core_assert(mesh != nullptr, "invalid mesh");

    /*
     * Process mesh vertices.
     */
    vertices.clear();
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
        Mesh::Vertex vertex{};

        /* Mesh vertex positions */
        {
            vertex.position[0] = mesh->mVertices[i].x;
            vertex.position[1] = mesh->mVertices[i].y;
            vertex.position[2] = mesh->mVertices[i].z;
        }

        /* Mesh vertex normals */
        if (mesh->HasNormals()) {
            vertex.normal[0] = mesh->mNormals[i].x;
            vertex.normal[1] = mesh->mNormals[i].y;
            vertex.normal[2] = mesh->mNormals[i].z;
        }

        /*
         * Mesh vertex colors.
         * A mesh may contain 0 to AI_MAX_NUMBER_OF_COLOR_SETS vertex colors
         * per vertex. We assume we will not use models where a vertex has
         * multiple colors, so we always take the first set (0).
         */
        if (mesh->HasVertexColors(0)) {
            vertex.color[0] = mesh->mColors[0][i].r;
            vertex.color[1] = mesh->mColors[0][i].g;
            vertex.color[2] = mesh->mColors[0][i].b;
        }

        /*
         * Mesh vertex texture coordinates.
         * A mesh may contain 0 to AI_MAX_NUMBER_OF_TEXTURECOORDS per vertex.
         * We assume we will not use models where a vertex has multiple texture
         * coordinates, so we always take the first set (0).
         */
        if (mesh->HasTextureCoords(0)) {
            vertex.texcoord[0] = mesh->mTextureCoords[0][i].x;
            vertex.texcoord[1] = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    /*
     * Process mesh faces.
     */
    faces.clear();
    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        core_assert(mesh->mFaces[i].mNumIndices == 3, "invalid mesh face");
        faces.push_back({
            mesh->mFaces[i].mIndices[0],
            mesh->mFaces[i].mIndices[1],
            mesh->mFaces[i].mIndices[2]
        });
    }
}

/**
 * MeshModel::draw
 * @brief MeshModel draw function.
 */
void MeshModel::draw(void *data)
{
    for (auto &mesh : m_meshes) {
        mesh->draw(data);
    }
}

} /* gl */
} /* atto */
