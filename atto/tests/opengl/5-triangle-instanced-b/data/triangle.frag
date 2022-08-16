#version 330 core

in vec4 vert_col;
out vec4 frag_col;

/*
 * fragment shader main
 */
void main(void)
{
    frag_col = vert_col;
}
