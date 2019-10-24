#version 330 core

out vec4 FragColor;
//in vec3 fNormal;
in vec2 fTexcoord;

uniform sampler2D fTexture;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth){
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main(){
    float depthValue = texture(fTexture,fTexcoord).r;
    FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    //FragColor = vec4(vec3(depthValue),1.0f);
    // FragColor = texture(fTexture, fTexcoord);
}
