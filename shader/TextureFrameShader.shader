#version 330 core

in vec3 glColor;
in vec2 glTexture;

out vec4 FragColor;

uniform sampler2D aTexture1;
uniform sampler2D aTexture2;
uniform float prop;

void main(){
    // 镜像
    //FragColor = mix(texture(aTexture1,glTexture),texture(aTexture2,vec2(1.0 - glTexture.x, glTexture.y)), 0.3) * vec4(glColor, 1.0f);

    FragColor = mix(texture(aTexture1,glTexture),texture(aTexture2,glTexture), prop);
}