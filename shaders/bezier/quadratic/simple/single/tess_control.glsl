#version 450 core

uniform int num_segments;
layout(vertices = 3) out;

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 1.0f;
        gl_TessLevelOuter[1] = float(num_segments);
    }
}