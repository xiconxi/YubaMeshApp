#version 410
out vec4 color;
in vec3 wnorm;
in float z_deepth;
uniform vec3 base_color = vec3(0.2f);
void main()
{
    color.xyz = base_color+vec3(gl_FrontFacing ? 0.6:0.1)*(0.2f+abs(dot(vec3(0,-1,0),wnorm)));
    color.xyz = mix(vec3(1,0,0), color.xyz, smoothstep(-0.01, 0.01, abs(z_deepth)) );
    color.w = 1.0;
}
