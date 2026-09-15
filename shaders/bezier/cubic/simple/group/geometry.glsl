#version 450 core

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

uniform vec2 screen_size;
flat in vec4 v_color[];
flat in float v_width[];
out vec4 fcolor;

void main()
{
    vec2 ndc_width = v_width[0] / screen_size * 2;
    vec2 p0 = gl_in[0].gl_Position.xy;
    vec2 p1 = gl_in[1].gl_Position.xy;

    vec2 direction = normalize(p1 - p0);
    vec2 normal = vec2(-direction.y, direction.x);

    vec2 offset = normal * ndc_width * 0.5;

    fcolor = v_color[0];

    gl_Position = vec4(p0 + offset, 0, 1);
    EmitVertex();

    gl_Position = vec4(p0 - offset, 0, 1);
    EmitVertex();

    gl_Position = vec4(p1 + offset, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(p1 - offset, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}