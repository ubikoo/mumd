#version 330 core

uniform sampler2D u_texsampler;

in vec4 vert_quad_normal;
in vec4 vert_quad_color;
in vec2 vert_quad_texcoord;

out vec4 frag_color;

/*
 * fragment shader main
 */
void main(void)
{
    frag_color = texture(u_texsampler, vert_quad_texcoord);
}
