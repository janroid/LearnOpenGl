#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

struct Light {
    vec3 ambColor;
    vec3 diffColor;
    vec3 specColor;

    float constant;
    float linear;
    float quadratic;
};

struct SunPos {
    vec3 pos;
};

uniform Light light;
uniform SunPos sunpos[2];
uniform vec3 cameraPos;

vec3 CalcPointLight(Light light, vec3 normal, vec3 eyeDir, vec3 sunPos);

void main()
{
    vec3 fNormal = normalize(Normal);
    vec3 eyeDir = normalize(cameraPos - FragPos);

    vec3 result = CalcPointLight(light, fNormal, eyeDir, sunpos[0].pos);
    result += CalcPointLight(light, fNormal, eyeDir, sunpos[1].pos);

    FragColor = vec4(result, 1.0f);
}

vec3 CalcPointLight(Light light ,vec3 normal, vec3 eyeDir, vec3 sunPos){
    vec3 ambient =light.ambColor * vec3(texture(texture_diffuse1,TexCoords));

    vec3 eyeVector = normalize(sunPos - FragPos);
    vec3 diffuse = light.diffColor * (max(dot(normal, eyeVector),0.0) * vec3(texture(texture_diffuse1, TexCoords)));

    vec3 refVector = reflect(-eyeVector, normal);
    float spec = pow(max(dot(eyeDir, refVector),0.0),64);
    vec3 specular = light.specColor * (spec * vec3(texture(texture_diffuse1, TexCoords)));

    // float distance = length(sunPos - FragPos);
    // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return (ambient + diffuse + specular);
}