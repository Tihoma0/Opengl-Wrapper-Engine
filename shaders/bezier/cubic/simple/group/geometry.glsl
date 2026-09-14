#version 450 core

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float width;

flat in vec4 v_color[];
out vec4 g_color;

void main()
{
    vec2 p0 = gl_in[0].gl_Position.xy;
    vec2 p1 = gl_in[1].gl_Position.xy;

    vec2 direction = normalize(p1 - p0);
    vec2 normal = vec2(-direction.y, direction.x);

    vec2 offset = normal * width * 0.5;

    g_color = v_color[0];

    gl_Position = vec4(p0 + offset, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(p0 - offset, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(p1 + offset, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(p1 - offset, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}