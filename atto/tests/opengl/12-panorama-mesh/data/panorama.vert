#version 330 core

#define PI 3.141592653589793

uniform float u_width;
uniform float u_height;
uniform mat4 u_mvp;

layout (location = 0) in vec3 quad_position;
layout (location = 1) in vec3 quad_normal;
layout (location = 2) in vec3 quad_color;
layout (location = 3) in vec2 quad_texcoord;

out vec4 vert_quad_normal;
out vec4 vert_quad_color;
out vec2 vert_quad_texcoord;

/**
 * sphere_coord
 * @brief Compute the spherical coordinates of the specified vertex assumming
 * -1 <= x,y <= 1.
 *
 * The equirectangular representation of the spherical coordinate system
 * maps a vertex with coordinates (x,y) to the polar and azimuth angles.
 * The spherical coordinate system is represented as
 *
 *  theta = 0  ----------------------------
 *             /                          /
 *             /           (x,y)          /  -1 <= y <= 1
 *             /                          /
 *  theta = pi ----------------------------
 *        phi = phi    -1 <= x <= 1   phi = -phi
 *
 * where the maximum and minimum range of the polar and azimuth angles
 * is symmetric to the range of the vertex cartesian coordinates.
 */
vec4 sphere_coord(vec3 pos)
{
    float phi = -PI*pos.x;
    float theta = 0.5*PI*(1.0 - pos.y);
    return vec4(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta), 1.0);
}

/**
 * vertex shader main
 */
void main(void)
{
    gl_Position = u_mvp * sphere_coord(quad_position);
    vert_quad_normal = vec4(quad_normal, 1.0);
    vert_quad_color = vec4(quad_color, 1.0);
    vert_quad_texcoord = quad_texcoord;
}
