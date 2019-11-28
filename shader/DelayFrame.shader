#version 330 core

out vec4 FragColor;

in vec2 fCoords;

uniform sampler2D gPos;
uniform sampler2D gNormal;
uniform sampler2D gColor;

struct Light {
    vec3 pos;
    vec3 color;

    float mlinear;
    float quadratic;
};

const int NR_LIGHTS = 32;
uniform Light light[NR_LIGHTS];
uniform vec3 viewPos;

void main(){
    vec3 fNormal = texture(gNormal,fCoords).rgb;
    vec3 fPos = texture(gPos, fCoords).rgb;
    vec4 tmp = texture(gColor, fCoords);
    vec3 diffuseColor = tmp.rgb;
    float specN = tmp.a;

    vec3 viewDir = normalize(viewPos - fPos);
    vec3 fColor = diffuseColor * 0.1f;
    for (int i = 0; i < NR_LIGHTS; i++){
        vec3 lightDir = normalize(light[i].pos - fPos);
        
        // 慢反射
        vec3 diffuse = max(dot(fNormal, lightDir), 0.0f) * diffuseColor * light[i].color;

        // 镜面反射
        vec3 halfDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(halfDir, fNormal), 0.0f), 16.0f);
        vec3 specular = spec * specN * light[i].color;

        // 衰减
        float distance = length(light[i].pos - fPos);
        float attenuation = 1.0f / (1.0f + light[i].mlinear * distance + light[i].quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;

        fColor += diffuse + specular;
    }

    FragColor = vec4(fColor, 1.0f);
}