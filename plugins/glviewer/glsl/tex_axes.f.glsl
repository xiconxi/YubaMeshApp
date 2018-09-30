#version 410
out vec4 color;
in vec3 wnorm;
in float center_dis;
uniform vec3 base_color;
void main()
{
    color.xyz = (sin(center_dis*50*3.1415926)>0.8?vec3(-0.1f):base_color)+vec3(gl_FrontFacing?0.6:0.2)*(0.2f+abs(dot(vec3(0,-1,0),wnorm)));
    color.w = 1.0;
}
