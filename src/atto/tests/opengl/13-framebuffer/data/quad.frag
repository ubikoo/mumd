#version 330 core

uniform float u_mix;
uniform float u_width;
uniform float u_height;
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
    vec2 uv = vec2(gl_FragCoord.x / u_width, 1.0 - gl_FragCoord.y / u_height);
    frag_color = mix(
        texture(u_texsampler, vert_quad_texcoord),
        texture(u_texsampler, uv),
        clamp(u_mix, 0, 1));
}
