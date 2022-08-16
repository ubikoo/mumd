#version 330 core

uniform mat4 u_mvp;

layout (location = 0) in vec4 a_pos;
layout (location = 1) in vec4 a_col;
layout (location = 2) in vec3 a_offset;

out vec4 vert_col;

/*
 * vertex shader main
 */
void main(void)
{
    vec4 offset = vec4(a_offset, 1.0);
    gl_Position = u_mvp * (a_pos + offset);
    vert_col = a_col;
}
