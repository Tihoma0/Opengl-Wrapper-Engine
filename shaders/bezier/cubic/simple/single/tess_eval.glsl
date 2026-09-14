#version 450 core

layout(isolines) in;

void main()
{
    float t = gl_TessCoord.x;

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

    gl_Position = vec4(position, 0.0, 1.0);
}