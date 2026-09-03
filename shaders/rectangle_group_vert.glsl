#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 rectangle_data;

uniform vec2 screen_size;

void main() {
    vec2 converted_pos  = rectangle_data.xy / screen_size * 2 - 1.0f;
    vec2 converted_size = rectangle_data.zw / screen_size * 2;

    converted_pos.y = - converted_pos.y;
    converted_size.y = - converted_size.y;


    gl_Position = vec4(aPos * converted_size + converted_pos, 0.0, 1.0);
}
