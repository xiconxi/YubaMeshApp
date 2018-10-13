#version 410 core

layout(triangles_adjacency, invocations = 1) in;
layout(points, max_vertices = 3) out;

in int vert_id[];

layout(stream = 0) out ivec3 face;
uniform sampler2D select_mask;

void main()
{
    bool flag[3];
    for(int i = 0 ; i< 3; i++) {
        flag[i]  = true;
        vec2 uv = gl_in[2*i].gl_Position.xy;
        if(uv.x > -0.999  && uv.x < 0.999 && uv.y > -0.999 && uv.y < 0.999){
            uv += 1;uv /= 2;
            flag[i] = texture(select_mask,uv).x > 0.2f;
        }else
            flag[i] = false;
    }

    if(flag[0] && flag[1] && flag[2]){
        face = ivec3(vert_id[0],vert_id[2],vert_id[4]);
        EmitStreamVertex(0);
        EndStreamPrimitive(0);
    }
}
