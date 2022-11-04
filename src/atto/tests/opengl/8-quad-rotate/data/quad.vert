#version 330 core

uniform mat4 u_mvp;

layout (location = 0) in vec4 a_pos;
layout (location = 1) in vec4 a_col;
out vec4 vert_col;

/*
 * vertex shader main
 */
void main(void)
{
    gl_Position = u_mvp * a_pos;
    vert_col = a_col;
}
