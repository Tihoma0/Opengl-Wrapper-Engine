#version 330 core
uniform vec4 color;
uniform float radius;

in vec2 frag_pos;

out vec4 FragColor;

void main() {
    float dist = length(frag_pos) * radius;
    float alpha = min(1.0f, radius - dist);

    FragColor = vec4(color.rgb, color.a * alpha);
}