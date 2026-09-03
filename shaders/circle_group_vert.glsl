#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 circle_data;

uniform vec2 screen_size;

out vec2 frag_pos;


void main() {
    vec2 converted_pos  = circle_data.xy / screen_size * 2 - 1.0f;
    vec2 converted_size = circle_data.z / screen_size * 2;

    converted_pos.y = - converted_pos.y;
    converted_size.y = - converted_size.y;

    frag_pos = aPos * 2 - 1;

    gl_Position = vec4(aPos * converted_size + converted_pos, 0.0, 1.0);
}
