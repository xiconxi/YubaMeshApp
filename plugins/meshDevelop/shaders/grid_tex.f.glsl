#version 410
out vec4 color;
in vec3 wnorm;
in vec2 uvs;

uniform sampler2D tex;

vec3 grid_texture(int w, int h, int gw){
    if(sin(uvs.x*3.1415926*w/gw)*sin(uvs.y*3.1415926*h/gw) > 0 )
        return vec3(0.2);
    else
        return vec3(0,0,0.2);
}

vec3 real_texture() {
    return texture(tex, uvs).xyz*0.3;
}

void main()
{
    color.xyz = grid_texture(5184,6912,100)+vec3(gl_FrontFacing ? 0.5:0.1)*(0.2f+abs(dot(vec3(0,-1,0),wnorm)));
//    color.xyz = real_texture()+vec3(gl_FrontFacing ? 0.5:0.1)*(0.2f+abs(dot(vec3(0,-1,0),wnorm)));
    color.w = 1.0;
}
