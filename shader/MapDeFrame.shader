#version 330 core

out vec4 FragColor;

uniform sampler2D fTexture;  // 阴影
uniform sampler2D lTexture;  // 物体
uniform vec3 lightPos;
uniform vec3 viewPos;

in VS_OUT {
    vec2 fTexcoord;
    vec3 fNormal; 
    vec3 fPos;// 顶点在世界坐标位置
    vec4 fPosLspace; // 顶点在光观察控件位置

} vs_in;

// 计算阴影
float ShadowCalculation(vec4 pos,float bias){
    vec3 projCoord = pos.rgb / pos.w;
    projCoord = (projCoord * 0.5) + 0.5;

    float shadow = 0.0;
    vec2 texSize = 1.0 / textureSize(fTexture,0);
    float currentDepth = projCoord.z;
    for (int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            float closestDepth = texture(fTexture,(projCoord.xy + vec2(x,y) * texSize)).r; // 采样偏移一个像素
            shadow += (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    if (projCoord.z > 1.0){
        shadow = 0.0;
    }

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
    float spec = pow(max(dot(halfwayDir, normal), 0.0), 64);
    vec3 specular = spec * lightColor;

    float bias = max(0.05 * (1 - dot(lightDir, normal)), 0.005);
    float shadow = ShadowCalculation(vs_in.fPosLspace, bias);

    vec3 lighting = ambient + (diffuse + specular) * (1 - shadow) * fColor;

    FragColor = vec4(lighting, 1.0f);
}