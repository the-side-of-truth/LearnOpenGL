#version 330
struct Material {
    sampler2D diffuseTex;
    sampler2D specularTex;
    sampler2D emitTex;
    float shininess;
}; 
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 WPos;
in vec3 WNormal;
in vec2 TexPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 CamCenter;
uniform Material m;
uniform Light light;
uniform float emitOffset; // 保证在[0,1]内

void main()
{
    /*---------Phong Shader---------*/
    vec3 result = vec3(0.0f);
    // 环境光
    vec3 ambient = light.ambient * lightColor;
    
    // 漫反射
    vec3 cur2Light = normalize(lightPos - WPos);
    vec3 diffuse = light.diffuse * lightColor * max(0, dot(cur2Light, WNormal));
    
    // 镜面反射
    vec3 halfVec = normalize(cur2Light + normalize(CamCenter - WPos));
    vec3 specular = light.specular * lightColor * pow(max(0, dot(halfVec, WNormal)), m.shininess);

    // 自发光(假设各向同)
    float emit = 1.0;
    if(dot(normalize(CamCenter - WPos), WNormal) < 0) emit = 0;

    result = result + ambient * vec3(texture(m.diffuseTex, TexPos));
    result = result + diffuse * vec3(texture(m.diffuseTex, TexPos));
    result = result + 0.5 * specular * vec3(texture(m.specularTex, TexPos));
    result = result + 1.5 * emit * vec3(texture(m.emitTex, TexPos + vec2(emitOffset, 0.0f))) * vec3(texture(m.specularTex, TexPos));
    //FragColor = vec4(result, 1.0);
    FragColor = vec4(result , 1.0);
}