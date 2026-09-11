#version 330 core

layout(location = 0) in vec2 position;

uniform vec2 screen_size;

void main() {
    vec2 ndc = position / screen_size * 2.0 - 1.0;
    gl_Position = vec4(ndc, 0.0, 1.0);
}