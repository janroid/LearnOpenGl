#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoords;
layout (location = 3) in mat4 aModel;

out vec2 texCoords;

uniform mat4 view;
uniform mat4 projection;

void main(){
    texCoords = aCoords;
    gl_Position = projection * view * aModel * vec4(aPos,1.0f);

}