#version 330
struct Material {
    bool hasDiffuseTex;
    bool hasSpecularTex;
    bool hasEmitTex;
    sampler2D diffuseTex0;
    sampler2D specularTex0;
    sampler2D emitTex;
    float shininess;
}; 

// PhongShader参数结构体
struct PhongParams{
    vec3 obj2Eye; // 物体指向眼睛的向量
    vec3 obj2Lihgt; // 物体指向光源的向量
    vec3 objNormal; // 物体自己的法向量
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// 点光源
struct PointLight {
    // 手电筒参数
    vec3 position;

    // 光照强度参数
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 衰减参数
    float constant;
    float linear;
    float quadratic;
};

// 聚光灯(手电筒)
struct SpotLight {
    // 手电筒参数
    vec3 position;
    vec3 direction; // 手电筒中心射线方向，从光源指向外界
    float cutOff_Inner;   // 视锥体余弦内边界值
    float cutOff_outer;   // 视锥体余弦外边界值

    // 光照强度参数
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 衰减参数
    float constant;
    float linear;
    float quadratic;
};

// 定向光
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

in vec3 WPos;
in vec3 VPos; // 相机坐标系中的位置
in vec3 WNormal;
in vec2 TexPos;

out vec4 FragColor;

uniform vec3 CamCenter;
uniform bool hasSpotLight;
uniform SpotLight spotLight;

uniform bool hasDirLight;
uniform DirLight dirLight;

uniform PointLight pointLight;
uniform float emitOffset; // 保证在[0,1]内
uniform sampler2D projectImg;

uniform int resClass;// 返回的结果类型
/*
返回类型：
0 -> 常规rgb
1 -> 反相
2 -> 灰度
3 -> 核效果
*/

uniform Material m;

vec3 spotLihgtRender(SpotLight _light, Material _m, vec3 _WPos, vec3 _WNormal, vec3 _CamCenter);
vec3 dirLihgtRender(DirLight _light, Material _m, vec3 _WPos, vec3 _WNormal, vec3 _CamCenter);
vec3 pointLihgtRender(PointLight _light, Material _m, vec3 _WPos, vec3 _WNormal, vec3 _CamCenter);
vec3 phongShader(inout PhongParams params);

void main()
{
    /*---------Phong Shader---------*/
    // 手电筒光源
    vec3 spotColor = vec3(0.0);
    if(hasSpotLight){
        spotColor = spotLihgtRender(spotLight, m, WPos, WNormal, CamCenter);
        // spotColor = vec3(0.5);
    }

    // 定向光
    vec3 dirColor = vec3(0.0);
    if(hasDirLight){
        dirColor = dirLihgtRender(dirLight,  m, WPos, WNormal, CamCenter);
        // dirColor = vec3(0.5f);
    }

    // 点光源
    // vec3 pointColor = pointLihgtRender(pointLight, m, WPos, WNormal, CamCenter);

    // 自发光(假设各向同)
    // float emit = 0.8;
    // if(dot(normalize(CamCenter - WPos), WNormal) < 0) emit = 0;
    // vec3 emitColor = emit * vec3(texture(m.emitTex, TexPos + vec2(emitOffset, 0.0f))) * vec3(texture(m.specularTex0, TexPos));

    // 累加结果
    vec3 result = 1.0 * dirColor + 1.0 * spotColor; 

    // 根据返回类型返回结果
    if(resClass == 0){// 常规rgb
        FragColor = vec4(result , 1.0);
    }else if(resClass == 1){// 反相
        FragColor = vec4(1.0 - result , 1.0);
    }else if(resClass == 2){// 灰度
        float average = 0.2126 * result.r + 0.7152 * result.g + 0.0722 * result.b;
        FragColor = vec4(average, average, average, 1.0);
    }
    
}

vec3 phongShader(inout PhongParams params){
    vec3 result = vec3(0.0f);
    // 环境光
    result += params.ambient;

    // 漫反射
    params.obj2Lihgt = normalize(params.obj2Lihgt);
    params.objNormal = normalize(params.objNormal);
    result += params.diffuse * max(0, dot(params.obj2Lihgt, params.objNormal));

    // 镜面反射
    params.obj2Eye = normalize(params.obj2Eye);
    vec3 halfVec = normalize(params.obj2Eye + params.obj2Lihgt);
    result += params.specular * pow(max(0, dot(halfVec, params.objNormal)), params.shininess);

    return result;
}

vec3 spotLihgtRender(SpotLight _light, Material _m, vec3 _WPos, vec3 _WNormal, vec3 _CamCenter){
    // 计算手电筒夹角
    float curSpotCos = dot(normalize(_WPos - _light.position), normalize(_light.direction));
    float spot = clamp((curSpotCos - _light.cutOff_outer) / (_light.cutOff_Inner - _light.cutOff_outer), 0, 1);

    // 计算衰减系数
    float distance2Light = length(_light.position - _WPos);
    float attenuation = 1.0 / (_light.constant + _light.linear * distance2Light + _light.quadratic * distance2Light * distance2Light);

    // 读取材质纹理
    vec3 diffuseTexColor = vec3(0.5f);
    if(m.hasDiffuseTex) diffuseTexColor = vec3(texture(_m.diffuseTex0, TexPos));
    vec3 specularTexColor = vec3(1.0f);
    if(m.hasSpecularTex) specularTexColor = vec3(texture(_m.specularTex0, TexPos));
    
    // PhongShader
    PhongParams params = PhongParams(_CamCenter - _WPos, 
        _light.position - _WPos, 
        _WNormal, 
        _light.ambient * diffuseTexColor, 
        _light.diffuse * diffuseTexColor, 
        _light.specular * specularTexColor,  
        _m.shininess
    );

    // 投影图案
    // float fSpotTan = sqrt(1 - _light.cutOff_outer * _light.cutOff_outer) / _light.cutOff_outer;
    // float project = 0.9;
    // vec3 projectColor = vec3(0.0, 0.0, 0.0);
    // vec2 fCoords = -vec2(VPos) / VPos.z; // 相机空间中焦距为1的平面上的坐标
    // vec2 texR = length(fCoords) / fSpotTan * 0.5f * normalize(fCoords) + vec2(0.5f);
    // projectColor = vec3(texture(projectImg, texR));

    return spot * attenuation * (phongShader(params));

}

vec3 dirLihgtRender(DirLight _light, Material _m, vec3 _WPos, vec3 _WNormal, vec3 _CamCenter){
    vec3 obj2Lihgt = normalize(-_light.direction);
    vec3 obj2Eye = normalize(_CamCenter - _WPos);
    // 读取材质纹理
    vec3 diffuseTexColor = vec3(texture(_m.diffuseTex0, TexPos));
    vec3 specularTexColor = vec3(texture(_m.specularTex0, TexPos));
    PhongParams params = PhongParams(obj2Eye, 
        obj2Lihgt, 
        _WNormal, 
        _light.ambient * diffuseTexColor, 
        _light.diffuse * diffuseTexColor, 
        _light.specular * specularTexColor,  
        _m.shininess
    );
    return phongShader(params);
}

vec3 pointLihgtRender(PointLight _light, Material _m, vec3 _WPos, vec3 _WNormal, vec3 _CamCenter){
    // 衰减系数
    float distance2Light = length(_light.position - _WPos);
    float attenuation = 1.0 / (_light.constant + _light.linear * distance2Light + _light.quadratic * distance2Light * distance2Light);
    // Phong Shader
    vec3 obj2Lihgt = normalize(_light.position - _WPos);
    vec3 obj2Eye = normalize(_CamCenter - _WPos);
    // 读取材质纹理
    vec3 diffuseTexColor = vec3(texture(_m.diffuseTex0, TexPos));
    vec3 specularTexColor = vec3(texture(_m.specularTex0, TexPos));
    PhongParams params = PhongParams(obj2Eye, 
        obj2Lihgt, 
        _WNormal, 
        _light.ambient * diffuseTexColor, 
        _light.diffuse * diffuseTexColor, 
        _light.specular * specularTexColor,  
        _m.shininess
    );
    return attenuation * phongShader(params);
}