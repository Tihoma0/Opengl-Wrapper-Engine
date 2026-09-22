#version 450 core
layout(location = 0) in vec2 aPos;

uniform vec2 screen_size;


void main() {
    vec2 ndc = aPos / screen_size * 2.0f - 1.0f;
    ndc.y = -ndc.y;

    gl_Position = vec4(ndc / 5.0f, 0.0, 1.0);


}