#version 410
out vec4 color;
in vec3 wnorm;
in vec2 uv;
uniform vec3 base_color;
uniform sampler2D tex;

vec3 grid_texture(int w, int h, int gw){
    if(sin(uv.x*3.1415926*w/gw)*sin(uv.y*3.1415926*h/gw) > 0 )
        return base_color;
    else
        return vec3(0, 0, base_color.b);
}

vec3 real_texture() {
    return texture(tex, uv).xyz*0.3;
}

void main()
{
//    color.xyz = grid_texture(5184,6912,80)+vec3(gl_FrontFacing ? 0.5:0.1)*(0.2f+abs(dot(vec3(0,-1,0),wnorm)));
    color.xyz = real_texture()+vec3(gl_FrontFacing ? 0.5:0.1)*(0.2f+abs(dot(vec3(0,-1,0),wnorm)));
    color.w = 1.0;
}
