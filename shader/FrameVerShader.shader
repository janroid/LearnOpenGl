#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 fTexCoord;

void main(){
    fTexCoord = aTex;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}