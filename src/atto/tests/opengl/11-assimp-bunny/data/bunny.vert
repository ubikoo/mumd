#version 330 core

uniform mat4 u_mvp;

layout (location = 0) in vec3 bunny_position;
layout (location = 1) in vec3 bunny_normal;
layout (location = 2) in vec3 bunny_color;
layout (location = 3) in vec2 bunny_texcoord;

out vec4 vert_bunny_normal;
out vec4 vert_bunny_color;
out vec2 vert_bunny_texcoord;

/*
 * vertex shader main
 */
void main(void)
{
    gl_Position = u_mvp * vec4(bunny_position, 1.0);
    vert_bunny_normal = vec4(bunny_normal, 1.0);
    vert_bunny_color = vec4(bunny_color, 1.0);
    vert_bunny_texcoord = bunny_texcoord;
}
