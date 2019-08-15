#version 330 core

out vec4 FragColor;

uniform vec3 ourColor;

// 冯氏光照
in vec3 FragPos;
in vec3 Normal;

uniform vec3 cameraPos;
uniform vec3 sunPos;
uniform vec3 sunColor;

uniform float ambStrength; // 环境光
uniform float specularStrength; // 镜面反射强度

// 高氏光照
// in vec3 fColor;

void main()
{
    // 环境光
    vec3 ambient = ambStrength * sunColor;

    // 慢反射
    vec3 fNormal = normalize(Normal);
    vec3 fVector = normalize(sunPos - FragPos);
    vec3 diffuse = max(dot(fNormal,fVector),0.0) * sunColor; //镜面反射
    // 计算反射光
    vec3 eyeVector = normalize(cameraPos - FragPos); // 观察向量
    vec3 refVector = reflect(-fVector, fNormal); // 反射光向量
    float spec = pow(max(dot(eyeVector, refVector),0.0),128);
    vec3 specular = specularStrength * spec * sunColor; // 反射光强度

    vec3 result = (ambient + diffuse + specular) * ourColor;

    // 高氏光照
    // vec3 result = fColor * ourColor;
    
    FragColor = vec4(result, 1.0f);
}