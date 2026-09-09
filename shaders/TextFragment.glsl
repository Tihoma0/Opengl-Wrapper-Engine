#version 330 core

uniform vec4 color;

uniform sampler2D tex;

in vec2 fragUv;

out vec4 fragColor;

void main() {
    fragColor = texture(tex, fragUv).r * color;
}
