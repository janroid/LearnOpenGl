#version 330 core

out vec4 FragColor;

// 冯氏光照
in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoord;

uniform vec3 cameraPos;

struct Material {
    sampler2D diffField;
    sampler2D specField;
    float shininess;
};

uniform Material mater;

struct DirLight {
    vec3 lightDir;

    vec3 ambColor;
    vec3 diffColor;
    vec3 specColor;
};
uniform DirLight dirLight;

struct PointLight {
    vec3 pos;

    vec3 ambColor;
    vec3 diffColor;
    vec3 specColor;

    float constant;
    float linear;
    float quadratic;
};
#define PINT_NUM 4
uniform PointLight pointLight[PINT_NUM];

struct SpotLight {
    vec3 ambColor;
    vec3 diffColor;
    vec3 specColor;

    float constant;
    float linear;
    float quadratic;

    // 聚光
    float cutOff;  // 聚光半角余弦值大小
    float outOff;  // 外圆锥
    vec3 dir;  // 聚光方向
};
uniform SpotLight spotLight;

// 平行光向量，法向量，观察向量
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 eyeDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 eyeDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 eyeDir);

void main()
{
    vec3 fNormal = normalize(Normal);
    vec3 eyeDir = normalize(cameraPos - FragPos); // 观察向量

    vec3 result = CalcDirLight(dirLight, fNormal, eyeDir);

    for (int i = 0; i < PINT_NUM; i++){
        result += CalcPointLight(pointLight[i], fNormal, eyeDir);
    }

    result += CalcSpotLight(spotLight, fNormal, eyeDir);

    FragColor = vec4(result, 1.0f);

}

vec3 CalcDirLight(DirLight light,vec3 normal, vec3 eyeDir){
    vec3 ambient =light.ambColor * vec3(texture(mater.diffField,TextureCoord));

    vec3 fVector = normalize(-light.lightDir);
    vec3 diffuse = light.diffColor * (max(dot(normal, fVector),0.0) * vec3(texture(mater.diffField, TextureCoord)));

    vec3 refVector = reflect(-fVector, normal);
    float spec = pow(max(dot(eyeDir, refVector),0.0),mater.shininess);
    vec3 specular = light.specColor * (spec * vec3(texture(mater.specField, TextureCoord)));

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light ,vec3 normal, vec3 eyeDir){
    vec3 ambient =light.ambColor * vec3(texture(mater.diffField,TextureCoord));

    vec3 eyeVector = normalize(light.pos - FragPos);
    vec3 diffuse = light.diffColor * (max(dot(normal, eyeVector),0.0) * vec3(texture(mater.diffField, TextureCoord)));

    vec3 refVector = reflect(-eyeVector, normal);
    float spec = pow(max(dot(eyeDir, refVector),0.0),mater.shininess);
    vec3 specular = light.specColor * (spec * vec3(texture(mater.specField, TextureCoord)));

    float distance = length(light.pos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return (ambient * attenuation + diffuse * attenuation + specular * attenuation);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 eyeDir){
    vec3 ambient =light.ambColor * vec3(texture(mater.diffField,TextureCoord));

    vec3 eyeVector = normalize(cameraPos - FragPos);
    vec3 diffuse = light.diffColor * (max(dot(normal, eyeVector),0.0) * vec3(texture(mater.diffField, TextureCoord)));

    vec3 refVector = reflect(-eyeVector, normal);
    float spec = pow(max(dot(eyeDir, refVector),0.0),mater.shininess);
    vec3 specular = light.specColor * (spec * vec3(texture(mater.specField, TextureCoord)));

    float distance = length(cameraPos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    float theta = dot(-eyeVector, normalize(light.dir));
    float intensity = clamp((theta - light.outOff)/(light.cutOff - light.outOff),0.0f,1.0f);

    return (ambient * attenuation + diffuse * attenuation * intensity + specular * attenuation * intensity);
}