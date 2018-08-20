#version 410 core
out vec4 color;
uniform float mesh_id;
void main()
{
    color = vec4(mesh_id, gl_PrimitiveID, 0, 1); // gl_PrimitiveID
}
