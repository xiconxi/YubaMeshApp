#version 410
out vec4 color;
in vec3 wnorm;
noperspective in vec4 wire;
uniform vec3 base_color;

const vec4 wire_color = vec4(24/255.0, 25/255.0, 76/255.0, 1);

void angular_bisector_wireframe(inout vec4 color) {
    if(wire.x > wire.w)
        color =  mix(wire_color, color, smoothstep(-0.001, 0.003, abs(wire.y-wire.z)));
    else if(wire.y > wire.w)
        color =  mix(wire_color, color, smoothstep(-0.001, 0.003, abs(wire.x-wire.z)));
    else if(wire.z > wire.w)
        color =  mix(wire_color, color, smoothstep(-0.001, 0.003, abs(wire.y-wire.x)));
}

void edge_bisector_wireframe(inout vec4 color) {
    if(wire.x < wire.w)
        color =  mix(wire_color, color, smoothstep(-0.001, 0.003, abs(wire.y-wire.z)));
    else if(wire.y < wire.w)
        color =  mix(wire_color, color, smoothstep(-0.001, 0.003, abs(wire.x-wire.z)));
    else if(wire.z < wire.w)
        color =  mix(wire_color, color, smoothstep(-0.001, 0.003, abs(wire.y-wire.x)));
}

void triangle_wireframe(inout vec4 color) {
//    color =  mix(vec4(0.3,0,0,1), color, smoothstep(-0.001, 0.002,min(min(wire.x,wire.y),wire.z)));
    color =  mix(vec4(0.3,0,0,1), color, smoothstep(-0.001, 0.002, min(min(wire.x,wire.y),wire.z) ));
}

void main()
{
    color.xyz = base_color+vec3(gl_FrontFacing?0.6:0.2)*(0.2f+abs(dot(vec3(0,-1,0),wnorm)));
//    triangle_wireframe(color);
//    angular_bisector_wireframe(color);
    edge_bisector_wireframe(color);
    color.w = 1.0;
}
