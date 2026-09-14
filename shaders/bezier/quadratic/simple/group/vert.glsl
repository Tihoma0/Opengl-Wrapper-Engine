#version 450 core
layout(location = 0) in vec2 control_point1;
layout(location = 1) in vec2 control_point2;
layout(location = 2) in vec2 control_point3;
layout(location = 3) in vec4 color;

uniform vec2 screen_size;

out vec4 vcolor;

void main() {
    vec2 aPos;
    if (gl_VertexID == 0) {
        aPos = control_point1;
    }
    if (gl_VertexID == 1) {
        aPos = control_point2;
    }
    if (gl_VertexID == 2) {
        aPos = control_point3;
    }
    vec2 ndc = aPos / screen_size * 2.0f - 1.0f;
    ndc.y = -ndc.y;
    gl_Position = vec4(ndc, 0.0, 1.0);
    vcolor = color;
}