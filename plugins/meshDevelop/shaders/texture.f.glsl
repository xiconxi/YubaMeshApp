#version 410
uniform sampler2D face_tex;
out vec4 color;

in vec3 tex_params[4];
void main()
{
    color = (texture(face_tex, tex_params[0].xy)*tex_params[0].z +
            texture(face_tex, tex_params[1].xy)*tex_params[1].z +
            texture(face_tex, tex_params[2].xy)*tex_params[2].z +
            texture(face_tex, tex_params[3].xy)*tex_params[3].z);
    color.w = 1.0f;
}
//in vec2 tex_params;
//void main()
//{
//    color = texture(face_tex, tex_params);
//    color.w = 1.0f;
//}
