#version 330 core
in vec3 Normal; // 法向量
in vec3 FragPos; // 片段位置
in vec2 TexCoords; // 纹理定点

out vec4 FragColor;

struct Material{
    sampler2D diffuse;  // 慢反射贴图
    sampler2D specular; // 镜面光贴图
    float shininess; // 镜面反射光强度
};

// 定向光
struct DirLight{
    vec3 direction; // 光方向向量
    vec3 ambient; // 环境光
    vec3 diffuse; // 慢反射光
    vec3 specular; // 镜面反射光
};

// 点光源
struct PointLight{
    vec3 position; // 点光源位置
    float Kc; // 常数项 
    float Kl; // 一次向
    float Kq; // 二次项
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4

// 聚光
struct SpotLight{
    vec3 direction; // 聚光方向向量
    float cutOff; // 内半径
    float outCutOff; // 外半径

    float Kc;
    float Kl;
    float Kq;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform DirLight dirLight;
uniform vec3 cameraPos; // 摄像机位置
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

//定向光 normal  平面法向量， viewDir 视线向量
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
// 计算点光源
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 
// 聚光
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // 属性
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos); // 视线方向

    // 定向光照
    vec3 result = CalcDirLight(dirLight, normal, viewDir);

    // 点光源
    for (int i = 0; i < NR_POINT_LIGHTS; i++){
        result += CalcPointLight(pointLight[i], normal, FragPos, viewDir);
    }

    // 聚光
    result += CalcSpotLight(spotLight, normal, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

// 定向光
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
 
    vec3 lightDir = normalize(-light.direction);  // 光源到片段的方向向量

    // 漫反射着色， （平面法向量）和（光源到片段方向向量）点乘
    float diff = max(dot(normal, lightDir), 0.0);

    //镜面光着色， 观察方向和反射光方向 点乘
    vec3 reflectDir = reflect(-lightDir, normal); // 计算反射光方向
    float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);  

    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);

}

// fragPos 片段位置
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos); // 光源到片段的方向向量
    
    //漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    
    //镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);// 计算反射光方向
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.Kc + light.Kl * distance + light.Kq * distance * distance);

    //合并
    vec3 ambient = light.ambient * attenuation * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * attenuation * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * attenuation * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(cameraPos - fragPos);
    // 慢反射
    float diff = max(dot(normal, lightDir), 0.0);
    //镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //衰落
    float distance = length(cameraPos - fragPos);
    float attenuation = 1.0 / (light.Kc + light.Kl * distance + light.Kq * distance * distance);

    // spotLight
    float theta = dot(lightDir, normalize(-light.direction)); // 聚光与片段光的夹角余弦值
    float epsilon = light.cutOff - light.outCutOff; //内外切角之前余弦差
    float intensity = clamp((theta - light.outCutOff)/epsilon, 0.0, 1.0);    // 计算聚光强度

    diff = diff * intensity * attenuation;
    spec = spec * intensity * attenuation;

    //合并
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.diffuse, TexCoords));

    return (ambient + diffuse + specular);

}






