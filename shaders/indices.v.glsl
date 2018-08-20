#version 410
layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 norm;

uniform mat4 model;
uniform mat4 camera_vp;
uniform vec4 orient;
out vec3 wnorm;
out float z_deepth;
void main()
{
    wnorm = normalize(mat3(model)*norm);
    z_deepth = length(mat3(model)*vec3(0,0,01))*(dot(vert, orient.xyz) - orient.w);
    gl_Position = camera_vp * model * vec4(vert, 1.0f);
}
