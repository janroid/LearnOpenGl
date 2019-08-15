#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; //法向量

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

// 高氏光照
// uniform vec3 cameraPos;
// uniform vec3 sunPos;
// uniform vec3 sunColor;

// uniform float ambStrength; // 环境光
// uniform float specularStrength; // 镜面反射强度

// out vec3 fColor;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // 高氏光照
    // vec3 FragPos = vec3(model * vec4(aPos, 1.0f));
    // vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

    // // 环境光
    // vec3 ambient = ambStrength * sunColor;

    // // 慢反射
    // vec3 fNormal = normalize(Normal);
    // vec3 fVector = normalize(sunPos - FragPos);
    // vec3 diffuse = max(dot(fNormal,fVector),0.0) * sunColor; //镜面反射
    // // 计算反射光
    // vec3 eyeVector = normalize(cameraPos - FragPos); // 观察向量
    // vec3 refVector = reflect(-fVector, fNormal); // 反射光向量
    // float spec = pow(max(dot(eyeVector, refVector),0.0),128);
    // vec3 specular = specularStrength * spec * sunColor; // 反射光强度

    // fColor = ambient + diffuse + specular;

    gl_Position = projection * view * model * vec4(aPos,1.0f);


}
