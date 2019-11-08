#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{   
    // 计算当前片段到光源的距离     
    float nlen = length(FragPos.xyz - lightPos);
    // 转化为0-1之间
    nlen = nlen / far_plane;

    gl_FragDepth = nlen;
    
}