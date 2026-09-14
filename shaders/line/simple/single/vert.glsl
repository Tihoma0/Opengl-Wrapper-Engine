#version 330 core

layout(location = 0) in vec2 aPos;

uniform vec2 screen_size;
uniform vec2 start;
uniform vec2 vector;

void main() {
    vec2 converted_pos = (aPos * vector + start) / screen_size * 2.0 - 1.0;
    converted_pos.y = -converted_pos.y;
    gl_Position = vec4(converted_pos, 0.0, 1.0);
}