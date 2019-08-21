#version 330 core

out vec4 FragColor;

// 冯氏光照
in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoord;

uniform vec3 cameraPos;

uniform float specularStrength; // 镜面反射强度

struct Material {
    vec3 ambField;
    sampler2D diffField;
    sampler2D specField;
    float shininess;
};

uniform Material mater;

struct Sun {
    vec3 ambColor;
    vec3 diffColor;
    vec3 specColor;
    vec3 sunPos;
};

uniform Sun sun;

void main()
{
    // 环境光
    vec3 ambient =sun.ambColor * vec3(texture(mater.diffField,TextureCoord));

    // 慢反射
    vec3 fNormal = normalize(Normal);
    vec3 fVector = normalize(sun.sunPos - FragPos);
    vec3 diffuse = sun.diffColor * (max(dot(fNormal,fVector),0.0) * vec3(texture(mater.diffField,TextureCoord))) ; //镜面反射
    // 计算反射光
    vec3 eyeVector = normalize(cameraPos - FragPos); // 观察向量
    vec3 refVector = reflect(-fVector, fNormal); // 反射光向量
    float spec = pow(max(dot(eyeVector, refVector),0.0),mater.shininess);
    vec3 specular = sun.specColor * (spec * vec3(texture(mater.specField, TextureCoord))); // 反射光强度

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0f);
}