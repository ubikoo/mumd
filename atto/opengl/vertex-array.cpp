/*
 * vertex-array.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/vertex-array.hpp"

namespace atto {
namespace gl {

/**
 * create_vertex_array
 * @brief Generate a new vertex array object name and bind it to the context.
 *
 * A vertex array object (VAO) stores the state needed to specify per-vertex
 * attribute data to the OpenGL rendering pipeline. Without a VAO, before the
 * draw call of an object, for each active vertex attribute in the shader,
 * we needed to:
 *  - glEnableVertexAttribArray to enable the attribute
 *  - glBindBuffer(GL_ARRAY_BUFFER) to bind the buffer object that contains
 *    the data for this attribute
 *  - glVertexAttribPointer to define the location and format of the data
 *    for the attribute in the buffer object bound to target GL_ARRAY_BUFFER
 *
 * The steps are repeated for each vertex attribute of each drawable.
 * VAOs minimize the effort and remember buffer object states for rendering
 * different objects.
 * A VAO is created with glGenVertexArray and bound with glBindVertexArray.
 * Once bound, the following functions change the state of the VAO:
 *  - glVertexAttribPointer
 *  - glEnableVertexAttribArray / glDisableVertexAttribArray
 *  - glBindBuffer(GL_ELEMENT_ARRAY_BUFFER)
 * If no VAO is bound, all functions above fail except for glBindBuffer.
 *
 * @note Vertex Array Object and Vertex Buffer Object
 * The function glBindBuffer(GL_ARRAY_BUFFER) is not on the list, even though
 * it is part of the attribute setup for rendering.
 * The binding to GL_ARRAY_BUFFER is not part of a VAO because the association
 * between a buffer object and a vertex attribute does not happen when you call
 * glBindBuffer(GL_ARRAY_BUFFER).
 * The association happens when you call glVertexAttribPointer.
 *
 * This means the behaviour of glVertexAttribPointer depends on whatever buffer
 * was bound to GL_ARRAY_BUFFER target at the time it was called.
 * But once called, what is bound to GL_ARRAY_BUFFER no longer matters.
 * The buffer objects's reference has been copied to the VAO.
 *
 * glVertexAttribPointer conflates two logically distinct operations:
 *  - Where and how to fetch the data from memory.
 *  - What the data looks like.
 *
 * The buffer object currently bound to GL_ARRAY_BUFFER, the offset pointer
 * and stride define where the data is stored and how to fetch it.
 * The offset pointer is not a real pointer but is (awkwardly) a cast of an
 * integer byte offset to a pointer.
 * The other parameters specify format of what a single unit of data looks
 * like (i.e., the vertex format of the array).
 *
 * @note glVertexAttribFormat and glBindVertexBuffer (OpenGL>=4.5)
 * Commonly there will be far more locations to pull the data from than vertex
 * formats. Even if the objects came from the same buffer, they will be located
 * at different offset positions within the buffer.
 *
 * With glVertexAttribPointer, it is not possible to update just the offset.
 * We need to speficy the whole "buffer + format" setup every time.
 *
 * glVertexAttribFormat and glBindVertexBuffer fix this problem.
 * glVertexAttribFormat specifies the vertex format parameters for an attribute.
 * Each of its parameters have the exact same meaning as the parameters from
 * the equivalent call to glVertexAttribPointer.
 * glBindVertexBuffer directly specifies the buffer object and takes the byte
 * offset as an actual (64-bit) integer.
 *
 * The two separate concepts are now separate functions, it is possible to have
 * a vertex array object that stores a format, bind it, then bind the vertex
 * buffers for each object or group of objects and render.
 * Changing vertex buffer binding state is cheaper than vertex format state.
 *
 * @note The usage pattern of a Vertex Array Object is:
 *  - At scene initialisation stage, for each drawable object
 *      - Bind the VAO.
 *      - Bind a Buffer Object containing vertex array indices to
 *      GL_ELEMENT_ARRAY_BUFFER target.
 *      - Bind the Buffer Object containing vertex attribute data
 *      to the GL_ARRAY_BUFFER target.
 *      - Set vertex data state for this visual object(vertices,
 *      normals, texture coordinates etc.), i.e., specifiy how
 *      OpenGL should interpret the buffer data at draw point.
 *      This interpretation is store in the Vertex Array Object.
 *      - Unbind the Vertex Array Object.
 *
 *  - At render point, for each drawable object:
 *      - Bind the VAO.
 *      - Call a glDraw* function
 *      - Unbind the Vertex Array Object.
 *
 * Binding the VAO at render point restores all the vertex data state specified
 * in the initialisation stage and permits switching between sets of objects
 * efficiently. Typical OpenGL draw modes are:
 *      GL_POINTS
 *      GL_LINE_STRIP
 *      GL_LINE_LOOP
 *      GL_LINES
 *      GL_LINE_STRIP_ADJACENCY
 *      GL_LINES_ADJACENCY
 *      GL_TRIANGLE_STRIP
 *      GL_TRIANGLE_FAN
 *      GL_TRIANGLES
 *      GL_TRIANGLE_STRIP_ADJACENCY
 *      GL_TRIANGLES_ADJACENCY
 *      GL_PATCHES
 *
 * @see glGenVertexArrays
 *      glDeleteVertexArrays
 *      glBindVertexArray
 *      glIsVertexArray
 *      glEnableVertexAttribArray
 *      glDisableVertexAttribArray
 *      glVertexAttribPointer
 *      glVertexAttribBinding
 *      glVertexAttribFormat
 *      glVertexAttribDivisor
 *      glDrawArrays
 *      glDrawElements
 *      glDrawRangeElements
 *      glMultiDrawArrays
 *      glMultiDrawElements
 *      https://paroj.github.io/gltut/Positioning/Tutorial%2005.html
 *      https://www.khronos.org/opengl/wiki/Vertex_Shader
 *      https://www.khronos.org/opengl/wiki/Vertex_Specification
 *      https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices
 *
 * @see Vertex array objects and buffer object:
 *      https://stackoverflow.com/questions/37972229
 *      https://stackoverflow.com/questions/34486197
 *      https://stackoverflow.com/questions/7718976
 *      https://stackoverflow.com/questions/13603956
 *      https://stackoverflow.com/questions/37985684
 *      https://stackoverflow.com/questions/46133849
 *      https://stackoverflow.com/questions/21652546
 *      https://gamedev.stackexchange.com/questions/99236
 *      https://gamedev.stackexchange.com/questions/97063
 *
 *      Vertex buffer objects and Pixel buffer objects:
 *      http://www.songho.ca/opengl/gl_vertexarray.html
 *      http://www.songho.ca/opengl/gl_vbo.html
 *      http://www.songho.ca/opengl/gl_pbo.html
 */
GLuint create_vertex_array(void)
{
    GLuint array;
    glGenVertexArrays(1, &array);
    glBindVertexArray(array);
    core_assert(glIsVertexArray(array), "failed to generate vertex array");
    glBindVertexArray(0);
    return array;
}

/**
 * destroy_vertex_array
 * @brief Delete a vertex array object.
 */
void destroy_vertex_array(const GLuint &array)
{
    glDeleteVertexArrays(1, &array);
}

} /* gl */
} /* atto */
