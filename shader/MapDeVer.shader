#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

out vec2 fTexcoord;

uniform mat4 model;
uniform mat4 lightMatrix;

void main(){
    fTexcoord = aTexcoord;
    gl_Position = lightMatrix * model * vec4(aPos,1.0f);

}