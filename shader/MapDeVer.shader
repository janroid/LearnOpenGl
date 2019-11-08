#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

out VS_OUT {
    vec2 fTexcoord;
    vec3 fNormal; 
    vec3 fPos; // 顶点在世界坐标位置
} vs_out;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform bool reverse_normals;

void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0);

    vs_out.fTexcoord = aTexcoord;
    vs_out.fPos = vec3(model * vec4(aPos,1.0f));

    if(reverse_normals){
        vs_out.fNormal = transpose(inverse(mat3(model))) * (-1 * aNormal);
    }else{
        vs_out.fNormal = transpose(inverse(mat3(model))) * aNormal;
    }

    
}