#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

out vec3 fTexCoord;

uniform mat4 model;

void main(){
    fTexCoord = aPos;
    mat4 v = mat4(mat3(view));
    vec4 pos = projection * v * model * vec4(aPos, 1.0f);
    gl_Position = pos.xyww;

}