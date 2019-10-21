#version 330 core

out vec4 FragColor;
//in vec3 fNormal;
in vec2 fTexcoord;

uniform sampler2D fTexture;

void main(){
    float depthValue = texture(fTexture,fTexcoord).r;

    FragColor = vec4(vec3(depthValue),1.0f);
    // FragColor = texture(fTexture, fTexcoord);
}
