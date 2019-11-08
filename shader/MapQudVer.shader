#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform bool reverse_normals;

void main()
{
    gl_Position = model * vec4(aPos, 1.0);
    if(reverse_normals){
        
    }
}