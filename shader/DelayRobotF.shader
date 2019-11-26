#version 330 core

layout (location = 0) out vec3 FragPos;
layout (location = 1) out vec3 FragNormal;
layout (location = 2) out vec4 FragColor; 

in vec3 Normal;
in vec2 TexCoords;
in vec3 fPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular;

void main(){
    FragPos = fPos;
    FragNormal = normalize(Normal);

    vec3 color = texture(texture_diffuse1,TexCoords).rgb;
    FragColor.rbg = color;

    vec4 spec = texture(texture_specular, TexCoords);
    FragColor.a = spec.r;
}