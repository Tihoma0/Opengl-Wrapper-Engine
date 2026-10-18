#version 330 core

uniform vec4 u_color;
out vec4 fragColor;

void main() {
    float r = float((gl_PrimitiveID * 3737) % 255) / 255.0;
    float g = float((gl_PrimitiveID * 737) % 255) / 255.0;
    float b = float((gl_PrimitiveID * 11337) % 255) / 255.0;

    fragColor = vec4(r, g, b, 0.3);
}