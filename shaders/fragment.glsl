#version 330 core
in vec2 texCoord;

uniform sampler2D u_texture;
uniform vec4 uvs;
uniform vec4 color;

layout(location = 0) out vec4 oColor;

void main() {
    oColor = texture(u_texture, texCoord);//vec4(texCoord, 0.0f, 1.0f);//
}
