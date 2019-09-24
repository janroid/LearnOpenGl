#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 fTexCoord;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main(){
    fTexCoord = aPos;

    vec4 pos = projection * view * model * vec4(aPos, 1.0f);
    gl_Position = pos.xyww;

}