#version 330 core

in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D aTexture;

void main(){
    FragColor = texture(aTexture, fTexCoord);

}