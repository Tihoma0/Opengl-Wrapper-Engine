#version 450 core

uniform int num_segments;
layout(vertices = 4) out;

in vec4 vcolor[];
out vec4 tc_color[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tc_color[gl_InvocationID] = vcolor[gl_InvocationID];
    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 1.0f;
        gl_TessLevelOuter[1] = float(num_segments);
    }
}