#version 330 core

uniform float u_width;
uniform float u_height;

in vec4 vert_col;
out vec4 frag_col;

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
    vec3 r = get_coord();
    frag_col = vert_col;
}
