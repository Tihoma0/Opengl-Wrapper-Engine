#version 450 core

uniform int num_segments;
layout(vertices = 4) out;

in vec4 vcolor[];
in float vwidth[];
out vec4 tc_color[];
out float tc_width[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tc_color[gl_InvocationID] = vcolor[gl_InvocationID];
    tc_width[gl_InvocationID] = vwidth[gl_InvocationID];
    if (gl_InvocationID == 0)
    {
        float segments_along_length = float(num_segments);

        // Getesteter Tausch: Jetzt sind 0 und 2 die Länge, 1 und 3 die Breite
        gl_TessLevelOuter[0] = segments_along_length; // Linke Kante
        gl_TessLevelOuter[2] = segments_along_length; // Rechte Kante

        gl_TessLevelOuter[1] = 1.0f;                 // Untere Kante
        gl_TessLevelOuter[3] = 1.0f;                 // Obere Kante

        // Auch die inneren Level müssen getauscht werden!
        gl_TessLevelInner[0] = 1.0f;                 // Innere Unterteilung für Breite
        gl_TessLevelInner[1] = segments_along_length; // Innere Unterteilung für Länge
    }

}