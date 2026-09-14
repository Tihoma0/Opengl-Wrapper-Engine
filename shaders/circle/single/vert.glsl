#version 330 core
layout(location = 0) in vec2 aPos;

uniform float radius;
uniform vec2 pos;

uniform vec2 screen_size;

out vec2 frag_pos;


void main() {
    vec2 converted_pos = pos * 2 / screen_size - 1;
    converted_pos.y *= -1;
    vec2 converted_radius = radius * 2 / screen_size;
    converted_radius.y *= -1;
    vec2 offset = converted_pos - converted_radius;
    frag_pos = aPos * 2 - 1;
    gl_Position = vec4(aPos * converted_radius * 2 + offset, 0.0, 1.0);
}