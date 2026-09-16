#version 450 core

uniform int num_segments;
layout(vertices = 3) out;

in vec4 v_color[];
in float v_width[];
out vec4 tc_color[];
out float tc_width[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tc_color[gl_InvocationID] = v_color[gl_InvocationID];
    tc_width[gl_InvocationID] = v_width[gl_InvocationID];
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