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
    vec2 src = vert_quad_texcoord;

    // vec2 dst;
    // if (src.x < 0.5) {
    //     dst.x = 2.0*src.x;
    //     dst.y = 0.5*src.y;
    // } else {
    //     dst.x = 2.0 - 2.0*src.x;
    //     dst.y = 1.0 - 0.5*src.y;
    // }
    vec2 dst = vec2(src.x + src.y, src.x + 2.0*src.y);
    dst = mod(dst, 1.0);

    frag_color = texture(u_texsampler, dst);
}
