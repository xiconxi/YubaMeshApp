#version 410
layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 camera_vp;
out vec3 wnorm;
out vec2 uvs;

void main()
{
    wnorm = normalize(mat3(model)*norm);
    gl_Position = camera_vp * model * vec4(vert, 1.0f);
    uvs = (uv+1.0f)/2.0f;
}
