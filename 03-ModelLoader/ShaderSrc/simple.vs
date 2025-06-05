#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexPos;
layout (location = 3) in mat4 instanceMat;

out vec3 WPos; // 世界坐标系中的位置
out vec3 VPos; // 相机坐标系中的位置
out vec3 WNormal;
out vec2 TexPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool useInstance;

void main(){
    mat4 finalModelMat = model;
    if(useInstance){
        finalModelMat = finalModelMat * instanceMat;
    }
    mat3 NormalMat =  transpose(inverse(mat3(finalModelMat)));
    // 位置的处理
    gl_Position = projection * view * finalModelMat * vec4(aPos, 1.0f);
    WPos = vec3(finalModelMat * vec4(aPos, 1.0f));
    VPos = vec3(view * vec4(WPos, 1.0f));
    // 法向量的处理
    WNormal = normalize(NormalMat * normalize(aNormal));
    // 纹理坐标
    TexPos = aTexPos;
}