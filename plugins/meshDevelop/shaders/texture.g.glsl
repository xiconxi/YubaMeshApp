#version 410 core

layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 itex_params[][4];
out vec3 tex_params[4];

int getMainTexIndex() {
    float maxAlpha = 0;
    int index = 0;
    for(int i = 0; i < 4; i++) {
        if(maxAlpha< itex_params[0][i].z){
            maxAlpha = itex_params[0][i].z;
            index = i;
        }
    }
    return index;
}

void main()
{
    int index = getMainTexIndex();
    for(int i = 0; i<3; i++) {
        tex_params = itex_params[i];
        vec3 tmp = tex_params[0];
        tex_params[0] = tex_params[index];
        tex_params[index] = tmp;
        gl_Position = vec4(2*tex_params[0].xy-1,0.0f,1.0f);
        EmitVertex();
    }
    gl_PrimitiveID  =  gl_PrimitiveIDIn;
    EndPrimitive();
}
