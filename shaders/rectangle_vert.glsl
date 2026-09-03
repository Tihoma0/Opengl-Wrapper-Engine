#version 450 core
layout(location = 0) in vec2 aPos;

uniform vec2 pos;
uniform vec2 size;
uniform ivec2 screen_size;

void main() {
    vec2 converted_pos = pos / screen_size * 2 - 1;
    converted_pos.y *= -1;
    vec2 converted_size = size / screen_size * 2;
    converted_size.y *= -1;
    gl_Position = vec4(aPos * converted_size + converted_pos, 0.0, 1.0);
}