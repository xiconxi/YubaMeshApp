#version 410 core

layout(triangles_adjacency, invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 model;
uniform mat4 camera_vp;
in vec3 vnorm[];
out vec3 wnorm;
out vec2 uv;

void main()
{
    for(int i = 0; i<3; i++) {
        gl_Position = camera_vp*model*gl_in[2*i].gl_Position;
        wnorm = normalize(mat3(model)*vnorm[2*i]);
        uv = gl_in[2*i+1].gl_Position.xy;
        EmitVertex();
    }
    gl_PrimitiveID  =  gl_PrimitiveIDIn;
    EndPrimitive();
}
