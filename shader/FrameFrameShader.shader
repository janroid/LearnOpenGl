#version 330 core

in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D aTexture;

void main(){
    vec3 c = texture(aTexture, fTexCoord).rgb
    FragColor = vec4(c,1.0f);
}