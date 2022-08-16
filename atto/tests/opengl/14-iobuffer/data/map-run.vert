#version 330 core

uniform mat4 u_mvp;

layout (location = 0) in vec3 quad_position;
layout (location = 1) in vec3 quad_normal;
layout (location = 2) in vec3 quad_color;
layout (location = 3) in vec2 quad_texcoord;

out vec4 vert_quad_normal;
out vec4 vert_quad_color;
out vec2 vert_quad_texcoord;

/*
 * vertex shader main
 */
void main(void)
{
    gl_Position = vec4(quad_position, 1.0);
    vert_quad_normal = vec4(quad_normal, 1.0);
    vert_quad_color = vec4(quad_color, 1.0);
    vert_quad_texcoord = quad_texcoord;
}
