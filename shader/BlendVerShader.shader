#version 330 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

out vec3 fNormal;
out vec3 fPos;

uniform mat4 model;
uniform vec3 cenPos; // 球心坐标

void main(){
    vec3 aNormal = normalize(aPos - cenPos);
    fNormal = mat3(transpose(inverse(model))) * aNormal;
    fPos = vec3(model * vec4(aPos,1.0f));
    gl_Position = projection * view * model * vec4(aPos,1.0f);
}
