#version 330 core

in vec2 fTexCoords;

out vec4 FragColor;

uniform sampler2D textureScn;

void main(){
    FragColor = texture(textureScn, fTexCoords);

}