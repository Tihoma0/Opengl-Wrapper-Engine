#version 330 core

layout(location = 0) in vec2 aPos; // quad from -1 -1 to 1 1
layout(location = 1) in vec4 data; // start, mormal
layout(location = 2) in vec2 sizes; // length, width
layout(location = 3) in vec4 color;
uniform vec2 screen_size;

out vec4 fcolor;
void main() {
    vec2 perpendicular = vec2(-data.w, data.z);
    vec2 along = data.zw * sizes.x;
    vec2 along_offset = aPos.x * along;
    vec2 perpendicular_offset = aPos.y * sizes.y * perpendicular;
    vec2 position = data.xy + along * 0.5f + along_offset + perpendicular_offset;
    vec2 ndc_position = position / screen_size * 2 - 1.0f;
    ndc_position.y = - ndc_position.y;
    fcolor = color;
    gl_Position = vec4(ndc_position, 0.0, 1.0);
}