#version 410
out vec4 color;
in vec3 wnorm;
void main()
{
    color.xyz = vec3(0.4f,0,0)+vec3(gl_FrontFacing ? 0.6:0.1)*(0.2f+abs(dot(vec3(0,-1,0),wnorm)));
    color.w = 1.0;
}
