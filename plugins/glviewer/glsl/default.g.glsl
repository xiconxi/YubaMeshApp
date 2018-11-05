#version 410 core

layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 camera_vp;
in vec3 vnorm[];
out vec3 wnorm;
noperspective out vec4 wire;


vec4 altitude(vec3 p1, vec3 p2, vec3 p3)
{
    float a = length(p2-p3);
    float b = length(p3-p1);
    float c = length(p2-p1);
    float alpha = acos((b*b + c*c - a*a )/(2.0*b*c));
    float beta = acos((a*a + c*c - b*b )/(2.0*a*c));
    float p = (a+b+c)/2;
    return vec4(abs(c*sin(beta)),abs(c*sin(alpha)),abs(b*sin(alpha)),sqrt((p-a)*(p-b)*(p-c)/p));
}

vec4 edge_bisector_altitude(vec3 p1, vec3 p2, vec3 p3){
    float a = length(p2-p3);
    float b = length(p3-p1);
    float c = length(p2-p1);
    float alpha = acos((b*b + c*c - a*a )/(2.0*b*c));
    float beta = acos((a*a + c*c - b*b )/(2.0*a*c));
    return vec4(1,1,1,0.33f)*(abs(c*sin(beta))+abs(c*sin(alpha))+abs(b*sin(alpha)))/3.0;
}

void main()
{
//    vec4 tri_dis = altitude(gl_in[0].gl_Position.xyz,gl_in[1].gl_Position.xyz,gl_in[2].gl_Position.xyz);
    vec4 tri_dis = edge_bisector_altitude(gl_in[0].gl_Position.xyz,gl_in[1].gl_Position.xyz,gl_in[2].gl_Position.xyz);
    for(int i = 0; i< 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        wnorm = vnorm[i];
        wire = vec4(0,0,0, tri_dis.w);
        wire[i] = tri_dis[i];
        EmitVertex();
    }
    gl_PrimitiveID  =  gl_PrimitiveIDIn;
    EndPrimitive();
}
