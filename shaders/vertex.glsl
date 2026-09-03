#version 330 core

layout(location = 0) in vec2 aPos;
//layout(location = 1) in vec2 objOffset;
//layout(location = 2) in vec2 objScale;
//layout(location = 3) in float objRotation;

//uniform vec2 screenSize;



out vec2 texCoord;


void main() {
//    float xaspect = screenSize.y / screenSize.x;
//    float yaspect = screenSize.x / screenSize.y;
//    float cosine = cos(objRotation);
//    float sine = sin(objRotation);
//    mat2 rotation = mat2(cosine, -sine, sine, cosine);
//    vec2 pos2d = vec2(aPos.x, -aPos.y);
//    pos2d.x *= objScale.x;
//    pos2d.y *= objScale.y;
//    pos2d *= rotation;
//    pos2d += vec2(0.5, 0.5) + objOffset;
//    if (xaspect > yaspect) {
//        pos2d.x *= xaspect;
//    } else {
//        pos2d.y *= yaspect;
//    }
    gl_Position = vec4(aPos, 0.0, 1.0);
    texCoord = vec2(aPos.x*2.0f + 0.5f, aPos.y*2.0f + 0.5f);
}
