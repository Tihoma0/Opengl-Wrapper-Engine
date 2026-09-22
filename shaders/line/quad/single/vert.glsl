#version 330 core

layout(location = 0) in vec2 aPos; // quad from -1 -1 to 1 1

uniform vec2 screen_size;
uniform vec2 start;
uniform vec2 normal;
uniform float length;
uniform float width;

void main() {
    vec2 perpendicular = vec2(-normal.y, normal.x);
    vec2 along = normal * length;
    vec2 along_offset = aPos.x * along;
    vec2 perpendicular_offset = aPos.y * width * perpendicular;
    vec2 position = start + along * 0.5f + along_offset + perpendicular_offset;
    vec2 ndc_position = position / screen_size * 2 - 1.0f;
    ndc_position.y = - ndc_position.y;
    gl_Position = vec4(ndc_position / 5.0f, 0.0, 1.0);
}