#version 330 core

in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D aTexture;

void main()
{
    FragColor = vec4(vec3(texture(aTexture, TextureCoord)), 1.0f);

}