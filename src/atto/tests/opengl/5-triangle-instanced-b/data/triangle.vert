#version 330 core

uniform vec3 u_offset[1000];
uniform mat4 u_mvp;

layout (location = 0) in vec4 a_pos;
layout (location = 1) in vec4 a_col;

out vec4 vert_col;

/*
 * vertex shader main
 */
void main(void)
{
    vec4 offset = vec4(u_offset[gl_InstanceID], 1.0);
    gl_Position = u_mvp * (a_pos + offset);
    vert_col = a_col;
}
