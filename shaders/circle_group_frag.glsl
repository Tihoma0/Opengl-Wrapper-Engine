#version 330 core

in vec2 frag_pos;
in float radius;
in vec4 color;

out vec4 FragColor;

void main() {
    float dist = length(frag_pos) * radius;
    float alpha = min(1.0f, radius - dist);

    FragColor = vec4(color.rgb, color.a * alpha);
}