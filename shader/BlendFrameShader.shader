#version 330 core

in vec3 fNormal;
in vec3 fPos;

out vec4 FragColor;

uniform samplerCube aTexture;
uniform vec3 cameraPos;

void main(){
    //折射
    // float ratio = 1.00 / 1.52;
    // vec3 pos = normalize(fPos - cameraPos);
    // vec3 R = refract(pos,normalize(fNormal),ratio);
    // FragColor = vec4(texture(aTexture, R).rgb,1.0);

    // 反射
    vec3 I = normalize(fPos - cameraPos);
    vec3 R = reflect(I, normalize(fNormal));
    FragColor = vec4(texture(aTexture, R).rgb, 1.0);


}