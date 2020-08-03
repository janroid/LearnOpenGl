#version 330 core

in vec3 fNormal;
in vec3 FragPos;


out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 ambColor;
uniform vec3 lightColor;
uniform vec3 cameraPos;

void main(){
    vec3 mColor = vec3(0.0f, 1.0f, 0.0f);
    vec3 eyeDir = normalize(cameraPos - FragPos);
    vec3 amb = ambColor;
    vec3 mNormal = normalize(fNormal);
    
    vec3 fVector = normalize(-lightDir);
    vec3 diff = lightColor * (max(dot(mNormal,fVector),0.0f));

    vec3 sVector = reflect(-lightDir, mNormal);
    float N = pow(max(dot(eyeDir, sVector), 0.0f), 32.0f);
    vec3 spec = 0.5 * N * lightColor;

    vec3 Co = (amb + diff + spec) * mColor;

    FragColor = vec4(Co, 1.0f);

}