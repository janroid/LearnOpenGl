#version 330 core

in vec2 glTexture;

out vec4 FragColor;

uniform sampler2D aTexture1;
uniform sampler2D aTexture2;

void main(){
    FragColor = mix(texture(aTexture1, glTexture), texture(aTexture2, glTexture), 0.4);

}