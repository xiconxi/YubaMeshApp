#version 410
layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec3 vt1;
layout (location = 3) in vec3 vt2;
layout (location = 4) in vec3 vt3;
layout (location = 5) in vec3 vt4;
out vec3 itex_params[4];
void main()
{
    itex_params[0] = vt1;
    itex_params[1] = vt2;
    itex_params[2] = vt3;
    itex_params[3] = vt4;
}
