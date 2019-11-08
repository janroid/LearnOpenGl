#version 330 core

out vec4 FragColor;

uniform samplerCube fTexture;  // 阴影
uniform sampler2D lTexture;  // 物体
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float far_plane;

in VS_OUT {
    vec2 fTexcoord;
    vec3 fNormal; 
    vec3 fPos;// 顶点在世界坐标位置

} vs_in;

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

// 计算阴影
float ShadowCalculation(vec3 fpos){
    // 未优化锯齿
    // vec3 fragToLight = fpos - lightPos;
    // float currentDepth = length(fragToLight); // 当前片段距离光源的距离

    // float closestDepth = texture(fTexture,fragToLight).r; // 当前片段在阴影贴图中的深度值

    // closestDepth *= far_plane; // 因为深度贴图中保存的深度值是0-1，在这里转化为实际距离值

    // float shadow = currentDepth - 0.05 > closestDepth ? 1.0 : 0.0;


    // 优化锯齿后
    vec3 fragToLight = fpos - lightPos;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.15;
    float samples = 20;
    float viewDistance = length(viewPos - fpos); 
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    
    for (int i = 0; i < samples; i++){
        float closestDepth = texture(fTexture, fragToLight + gridSamplingDisk[i]*diskRadius).r;
        closestDepth *= far_plane;
        if(currentDepth - bias > closestDepth){
            shadow += 1.0;
        }
    }

    shadow = shadow / float(samples);

    // 绘制阴影
    // float closestDepth = texture(fTexture, fragToLight).r;
    // closestDepth *= far_plane;

    // FragColor = vec4(vec3(closestDepth / far_plane),1.0f);  

    return shadow;
}

void main(){
    vec3 fColor = texture(lTexture,vs_in.fTexcoord).rgb;
    vec3 lightColor = vec3(0.3);
    vec3 normal = normalize(vs_in.fNormal);
    // 环境光
    vec3 ambient = 0.3 * fColor;

    // 慢反射
    vec3 lightDir = normalize(lightPos - vs_in.fPos);
    float diff = max(dot(lightDir, normal),0.0);
    vec3 diffuse = diff * lightColor;

    // 镜面反射
    vec3 viewDir = normalize(viewPos - vs_in.fPos);
    vec3 halfwayDir  = normalize(lightDir + viewDir);
    float spec = 0.0;
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64);
    vec3 specular = spec * lightColor;

    float shadow = ShadowCalculation(vs_in.fPos);

    vec3 lighting = (ambient + (1 - shadow) * (diffuse + specular)) * fColor;

    FragColor = vec4(lighting, 1.0f);  
}