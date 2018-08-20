#version 410
layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec3 uv1;
layout (location = 3) in vec3 uv2;
layout (location = 4) in vec3 uv3;
layout (location = 5) in vec3 uv4;
uniform mat4 model;
out vec3 light_norm_vs;
out vec3 uvs[4];
void main()
{
    light_norm_vs = normalize(mat3(model)*norm);
    gl_Position = model*vec4(vert,1.0);
    uvs[0] = uv1;
    uvs[1] = uv2;
    uvs[2] = uv3;
    uvs[3] = uv4;
}
