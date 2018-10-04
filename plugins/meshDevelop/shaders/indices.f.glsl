#version 410
out vec4 color;
in vec3 wnorm;
in vec3 base_color;
void main()
{
    color.xyz = base_color+vec3(gl_FrontFacing ? 0.6:0.1)*(0.2f+abs(dot(vec3(0,-1,0),wnorm)));
    color.w = 1.0;
}
