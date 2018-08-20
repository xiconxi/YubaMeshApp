#version 410
out vec4 color;
in GS_FS{
    vec3 light_norm;
    vec3 uvs[4];
    noperspective  vec3 e_dis;
} fs_in;
uniform sampler2D face_tex;

uniform vec4 base_color = vec4(1.0f, 1.0f, 1.0f,.3f);
const vec4 wire_color = vec4(24/255.0, 25/255.0, 76/255.0, 1);
void wireframe(inout vec4 color) {
    color =  mix(wire_color, color, smoothstep(-0.001, 0.003,min(min(fs_in.e_dis.x,fs_in.e_dis.y),fs_in.e_dis.z)));
}

vec3 grid_texture(int w, int h, int gw){
    vec3 res = vec3(0);
    for(int i=0;i<4; i++){
        if(sin(fs_in.uvs[i].x*3.1415926*w/gw)*sin(fs_in.uvs[i].y*3.1415926*h/gw) > 0 )
            res += vec3(0.2)*fs_in.uvs[i].z;
        else
            res += vec3(0,0,0.2)*fs_in.uvs[i].z;
    }
    return res;
}

vec3 tri_texture(float s) {
    if(sin(fs_in.e_dis[0]*3.1415926*s)*sin(fs_in.e_dis[1]*3.1415926*s)*sin(fs_in.e_dis[2]*3.1415926*s) < 0) //
        return vec3(0.5);
    return vec3(0);
}

vec3 real_texture() {
//    return vec3(0.5);
    return (texture(face_tex, fs_in.uvs[0].xy)*fs_in.uvs[0].z +
            texture(face_tex, fs_in.uvs[1].xy)*fs_in.uvs[1].z +
            texture(face_tex, fs_in.uvs[2].xy)*fs_in.uvs[2].z +
            texture(face_tex, fs_in.uvs[3].xy)*fs_in.uvs[3].z).xyz;
}

void main()
{
    //    vec3 tex_base = grid_texture(5184,6912,30);
    vec3 tex_base = tri_texture(5);
    if(gl_FrontFacing) tex_base = real_texture();
    color.xyz = tex_base*(0.2f+abs(dot(vec3(0,0,-1),fs_in.light_norm))); // vec3(0.3,0.3,0.6)
    color.w = 1.0f;
    color = mix(vec4(base_color.xyz,1.0), color , base_color.w);
    wireframe(color);
}
