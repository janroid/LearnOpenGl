#version 330 core

out vec4 FragColor;

in vec2 fTexcoord;
uniform sampler2D fTexture;

void main(){
    // gl_FragDepth = gl_FragCoord.z;

    FragColor = texture(fTexture,fTexcoord);
}