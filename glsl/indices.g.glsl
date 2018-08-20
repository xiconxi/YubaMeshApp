#version 410 core

layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;
in vec3 light_norm_vs[];
in vec3 uvs[][4];
out vec3 light_norm;
uniform mat4 camera_vp;

///
/// \return vec3(ha,hb,hc)
vec3 altitude(vec3 p1, vec3 p2, vec3 p3)
{
//    return vec3(1,1,1);
    float a = length(p2-p3);
    float b = length(p3-p1);
    float c = length(p2-p1);
    float alpha = acos((b*b + c*c - a*a )/(2.0*b*c));
    float beta = acos((a*a + c*c - b*b )/(2.0*a*c));
    return vec3(abs(c*sin(beta)),abs(c*sin(alpha)),abs(b*sin(alpha)));
}
out GS_FS{
    vec3 light_norm;
    vec3 uvs[4];
    noperspective  vec3 e_dis;
} gs_out;

void main()
{
    vec3 tri_dis = altitude(gl_in[0].gl_Position.xyz,gl_in[1].gl_Position.xyz,gl_in[2].gl_Position.xyz);
    for(int i=0;i< 3; i++) {
        gs_out.light_norm = light_norm_vs[i];
        gs_out.e_dis = vec3(0,0,0);
        gs_out.e_dis[i] = tri_dis[i];
        gs_out.uvs[0] = uvs[i][0];
        gs_out.uvs[1] = uvs[i][1];
        gs_out.uvs[2] = uvs[i][2];
        gs_out.uvs[3] = uvs[i][3];
        gl_Position = camera_vp*gl_in[i].gl_Position;
        EmitVertex();
    }
    gl_PrimitiveID  =  gl_PrimitiveIDIn;
    EndPrimitive();
}
