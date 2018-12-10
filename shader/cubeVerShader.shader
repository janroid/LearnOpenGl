#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// 反射光计算
// uniform vec3 lightColor;
// uniform vec3 lightPos;  // 光源位置
// uniform vec3 cameraPos; // 摄像机位置

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    FragPos = vec3(model * vec4(aPos,1.0f));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoord;

    gl_Position =  projection * view * vec4(FragPos, 1.0f); 

    //  // 环境光照
    // float ambientStrength = 0.1; 
    // vec3 ambient = lightColor * ambientStrength;

    // // 慢反射光照
    
    // vec3 norm = normalize(Normal);  
    // vec3 lightDir = normalize(lightPos - FragPos); // 光线方向
    // float diff = max(dot(norm, lightDir), 0.0); // 漫反射光
    // vec3 diffuse = diff * lightColor;

    // // 镜面反射光照
    // float specularStrenght = 0.5;
    // vec3 viewDir = normalize(cameraPos - FragPos); // 视线向量
    // vec3 reflectDir = reflect(-lightDir,norm); //反射光向量
    // float spec = pow(max(dot(viewDir, reflectDir),0.0),32); // 镜面反射
    // vec3 specular = specularStrenght * spec * lightColor;

    // hunheColor = ambient + diffuse + specular;

}