#version 330 core

uniform float u_width;
uniform float u_height;
uniform float u_alpha;
uniform sampler2D u_texsampler1;
uniform sampler2D u_texsampler2;

in vec4 vert_quad_normal;
in vec4 vert_quad_color;
in vec2 vert_quad_texcoord;

out vec4 frag_color;

/*
 * get_coord
 * Compute fragment position in normalized device coordinates.
 */
vec3 get_coord (void)
{
    float du = 2.0 / u_width;
    float dv = 2.0 / u_height;
    float dw = 2.0;
    return vec3(gl_FragCoord.x*du-1.0,
                gl_FragCoord.y*dv-1.0,
                gl_FragCoord.z*dw-1.0);
}

/*
 * fragment shader main
 */
void main(void)
{
    vec3 coord = get_coord();
    vec4 tex_color1 = texture(u_texsampler1, vert_quad_texcoord);
    vec4 tex_color2 = texture(u_texsampler2, vert_quad_texcoord);
    vec4 pos_color = vert_quad_color;
    frag_color = mix(tex_color1, tex_color2, clamp(u_alpha, 0.0, 1.0));
}
