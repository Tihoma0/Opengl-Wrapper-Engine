#version 450 core

layout(quads, equal_spacing) in;

uniform vec2 screen_size;
uniform float width;

void main()
{
    float v = gl_TessCoord.x;
    float t = gl_TessCoord.y;

    vec2 p0 = gl_in[0].gl_Position.xy;
    vec2 p1 = gl_in[1].gl_Position.xy;
    vec2 p2 = gl_in[2].gl_Position.xy;


    float u = 1.0 - t;

    vec2 position =
    p0 * (u * u) +
    p1 * (2.0f * u * t) +
    p2 * (t * t);
    vec2 tangens =
    p0 * (-2.0f * u) +
    p1 * (2.0f * u - 2.0f * t) +
    p2 * (2.0f * t);

    vec2 perpendicular = normalize(vec2(-tangens.y, tangens.x));
    vec2 offset = (v - 0.5) * 2 * width * perpendicular / screen_size;
    gl_Position = vec4(position + offset, 0.0, 1.0);
}