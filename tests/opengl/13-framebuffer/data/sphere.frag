#version 330 core

uniform float u_mix;
uniform float u_width;
uniform float u_height;
uniform sampler2D u_texsampler;

in vec4 vert_sphere_normal;
in vec4 vert_sphere_color;
in vec2 vert_sphere_texcoord;

out vec4 frag_color;

/*
 * fragment shader main
 */
void main(void)
{
    vec4 tex_color = texture(u_texsampler, vert_sphere_texcoord);
    vec4 pos_color = vec4(
        gl_FragCoord.x / u_width,
        gl_FragCoord.y / u_height,
        0,
        1);
    frag_color = mix(tex_color, pos_color, clamp(u_mix, 0, 1));
}
