#version 450 core

uniform int num_segments;
layout(vertices = 3) out;

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    if (gl_InvocationID == 0)
    {
        float segments_along_length = float(num_segments);

        gl_TessLevelOuter[0] = segments_along_length;
        gl_TessLevelOuter[2] = segments_along_length;

        gl_TessLevelOuter[1] = 1.0f;
        gl_TessLevelOuter[3] = 1.0f;
        gl_TessLevelInner[0] = 1.0f;
        gl_TessLevelInner[1] = segments_along_length;
    }
}