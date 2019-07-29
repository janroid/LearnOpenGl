#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 gColor;

void main() {

    // float angle = radians(90.0f);

    // mat3 roteM = mat3(
    //     cos(angle), sin(angle),0,
    //     -sin(angle), cos(angle),0,
    //     0, 0, 1
    // );

    gl_Position = vec4(aPos, 1.0f);
    gColor = aColor;
}