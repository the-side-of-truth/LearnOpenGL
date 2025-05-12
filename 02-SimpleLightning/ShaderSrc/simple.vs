#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexPos;

out vec3 WPos;
out vec3 WNormal;
out vec2 TexPos;

uniform mat4 MVP;
uniform mat3 NormalMat;

void main(){
    // 位置的处理
    gl_Position = MVP * vec4(aPos, 1.0f);
    WPos = aPos;
    // 法向量的处理
    WNormal = normalize(NormalMat * aNormal);
    // 纹理坐标
    TexPos = aTexPos;
}