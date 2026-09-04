#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 circle_data;
layout(location = 2) in vec4 instance_color;

uniform vec2 screen_size;

out vec2 frag_pos;
out float radius;
out vec4 color;

void main() {
    vec2 converted_pos  = circle_data.xy / screen_size * 2 - 1.0f;
    vec2 converted_size = circle_data.z / screen_size * 2;

    converted_pos -= converted_size;

    converted_size *= 2;

    converted_pos.y = - converted_pos.y;
    converted_size.y = - converted_size.y;

    frag_pos = aPos * 2 - 1;
    radius = circle_data.z;
    color = instance_color;

    gl_Position = vec4(aPos * converted_size + converted_pos, 0.0, 1.0);
}
