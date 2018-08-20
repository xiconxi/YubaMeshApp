#version 410
layout (location = 0) in vec3 vert;

uniform mat4 model;
uniform mat4 camera_vp;

out int vert_id;
void main()
{
    gl_Position = camera_vp*model*vec4(vert,1.0);
    gl_Position /= gl_Position.w;

    vert_id = gl_VertexID;
}
