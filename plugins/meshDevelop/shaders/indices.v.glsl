#version 410
layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 norm;
layout (location = 2) in int v_cluster;

uniform mat4 model;
uniform mat4 camera_vp;
out vec3 wnorm;
out vec3 base_color;
const vec3 color_table[12] = vec3[12](
            vec3(255,0,0)/255, vec3(255,128,0)/255, vec3(255,255,0)/255,
            vec3(128,255,0)/255, vec3(0,255,0)/255, vec3(0,255,128)/255,
            vec3(0,255,255)/255, vec3(0,128,255)/255, vec3(0,0,255)/255,
            vec3(128,0,255)/255, vec3(255,0,255)/255, vec3(255,0,128)/255);

void main()
{
    base_color = color_table[v_cluster]*0.32f;
    wnorm = normalize(mat3(model)*norm);
    gl_Position = camera_vp * model * vec4(vert, 1.0f);
}
