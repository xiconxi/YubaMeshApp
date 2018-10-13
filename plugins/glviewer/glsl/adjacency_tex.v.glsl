#version 410
layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 norm;

out vec3 vnorm;

void main()
{
    gl_Position = vec4(vert, 1.0f);
    vnorm = norm;
}
