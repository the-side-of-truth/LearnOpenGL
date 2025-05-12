#version 330
in vec3 WPos;
in vec3 WNormal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 CamCenter;

void main()
{
    /*---------Phong Shader---------*/
    vec3 result = vec3(0.0f);
    // 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    result = result + ambient * objectColor;
    // 漫反射
    float diffuseStrength = 1.0;
    vec3 cur2Light = normalize(lightPos - WPos);
    vec3 diffuse = diffuseStrength * lightColor * max(0, dot(cur2Light, WNormal));
    result = result + diffuse * objectColor;
    // 镜面反射
    float specularStrength = 0.5;
    vec3 halfVec = normalize(cur2Light + normalize(CamCenter - WPos));
    vec3 specular = specularStrength * lightColor * pow(max(0, dot(halfVec, WNormal)), 32);
    result = result + specular * objectColor;
    FragColor = vec4(result, 1.0);
}