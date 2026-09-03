#version 450 core
layout(location = 0) in vec2 aPos;

uniform vec2 pos;
uniform vec2 size;
uniform vec2 screen_size;

out vec2 uv;

void main() {
    vec2 converted_pos = pos / screen_size * 2 - 1;
    converted_pos.y *= -1;
    vec2 converted_size = size / screen_size * 2;
    converted_size.y *= -1;
    uv = vec2(aPos.x, 1.0f - aPos.y);
    gl_Position = vec4(aPos * converted_size + converted_pos, 0.0, 1.0);
}