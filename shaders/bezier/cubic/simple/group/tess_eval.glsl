#version 450 core

layout(quads, equal_spacing) in;

uniform vec2 screen_size;

in vec4 tc_color[];
in float tc_width[];
out vec4 fcolor;

void main()
{
    float v = gl_TessCoord.x;
    float t = gl_TessCoord.y;

    vec2 p0 = gl_in[0].gl_Position.xy;
    vec2 p1 = gl_in[1].gl_Position.xy;
    vec2 p2 = gl_in[2].gl_Position.xy;
    vec2 p3 = gl_in[3].gl_Position.xy;


    float u = 1.0 - t;

    vec2 position =
        p0 * (t * t * t)
        + p1 * (3 * t * t * u)
        + p2 * (3 * t * u * u)
        + p3 * (u * u * u);
    vec2 tangent =
        p0 * (3.0 * t * t)
        + p1 * (6.0 * t * u - 3.0 * t * t)
        + p2 * (3.0 * u * u - 6.0 * t * u)
        - p3 * (3.0 * u * u);
    vec2 dir = normalize(tangent);
    vec2 perpendicular = vec2(-dir.y, dir.x);
    float offset_side = v - 0.5;
    vec2 current_width = tc_width[0] / screen_size;

    vec2 final_position = position + (perpendicular * offset_side) * current_width;

    gl_Position = vec4(final_position, 0.0, 1.0);
    fcolor = tc_color[0];
}