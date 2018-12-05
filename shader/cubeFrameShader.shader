#version 330 core
in vec3 Normal; // 法向量
in vec3 FragPos; // 片段位置

out vec4 FragColor;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;  // 光源位置
uniform vec3 cameraPos; // 摄像机位置

void main()
{
    // 环境光照
    float ambientStrength = 0.1; 
    vec3 ambient = lightColor * ambientStrength;

    // 慢反射光照
    
    vec3 norm = normalize(Normal);  
    vec3 lightDir = normalize(lightPos - FragPos); // 光线方向
    float diff = max(dot(norm, lightDir), 0.0); // 漫反射光
    vec3 diffuse = diff * lightColor;

    // 镜面反射光照
    float specularStrenght = 0.5;
    vec3 viewDir = normalize(cameraPos - FragPos); // 视线向量
    vec3 reflectDir = reflect(-lightDir,norm); //反射光向量
    float spec = pow(max(dot(viewDir, reflectDir),0.0),32); // 镜面反射
    vec3 specular = specularStrenght * spec * lightColor;

    vec3 resultColor = (ambient + diffuse + specular) * objColor;

    FragColor = vec4(resultColor, 1.0);

}
