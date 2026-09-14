#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 data; // x y vx vy
layout(location = 2) in vec4 color;

uniform vec2 screen_size;

out vec4 fcolor;

void main() {
    vec2 converted_pos = (aPos * data.zw + data.xy) / screen_size * 2.0 - 1.0;
    converted_pos.y = -converted_pos.y;
    fcolor = color;
    gl_Position = vec4(converted_pos, 0.0, 1.0);
}