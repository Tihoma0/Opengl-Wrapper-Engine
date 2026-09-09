#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 pos;
uniform vec2 size;
uniform vec2 screen_size;
uniform vec4 uv;

out vec2 fragUv;

void main() {
    vec2 converted_pos  = (aPos * size + pos) / screen_size * 2 - 1.0f;

    converted_pos.y = - converted_pos.y;

    fragUv = aPos * uv.zw + uv.xy;


    gl_Position = vec4(converted_pos, 0.0, 1.0);
}
