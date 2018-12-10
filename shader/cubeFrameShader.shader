#version 330 core
in vec3 Normal; // 法向量
in vec3 FragPos; // 片段位置
in vec2 TexCoords; // 纹理定点

out vec4 FragColor;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light{
    vec3 position; // 光源位置
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//uniform vec3 objColor;
uniform vec3 cameraPos; // 摄像机位置
uniform Material material;
uniform Light light;

void main()
{
    // 环境光照
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // 慢反射光照
    vec3 norm = normalize(Normal);  // 法线向量
    vec3 lightDir = normalize(light.position - FragPos); // 光线方向
    float diff = max(dot(norm, lightDir), 0.0); // 漫反射光
    vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

    // 镜面反射光照
    vec3 viewDir = normalize(cameraPos - FragPos); // 视线向量
    vec3 reflectDir = reflect(-lightDir,norm); //反射光向量
    float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess); // 镜面反射
    vec3 specular = light.specular * (spec * texture(material.specular, TexCoords).rgb);
    vec3 emission = texture(material.emission, TexCoords).rgb;

    vec3 resultColor = ambient + diffuse + specular + emission;

    // vec3 resultColor = hunheColor * objColor; // vex计算反射光照
    FragColor = vec4(resultColor, 1.0);

}
