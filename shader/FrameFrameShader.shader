#version 330 core

in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D fTexture;

const float offset = 1.0f / 300.0f;

void main(){

    vec2 offsetMap[9] = vec2[](
        vec2(-offset, offset),
        vec2(0.0f,offset),
        vec2(offset,offset),
        vec2(-offset, 0),
        vec2(0.0f,0),
        vec2(offset,0),
        vec2(-offset, -offset),
        vec2(0.0f,-offset),
        vec2(offset,-offset)
    );

    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );

    vec3 simpleMap[9];
    for(int i = 0; i < 9; i++){
        simpleMap[i] = texture(fTexture, fTexCoord + offsetMap[i]).rgb;
    }

    vec3 c = vec3(0.0);
    for(int i = 0; i < 9; i++){
        c = c + simpleMap[i]*kernel[i];
    }

    FragColor = vec4(c,1.0f);


    // vec3 c = texture(fTexture, fTexCoord).rgb;
    // FragColor = vec4(c,1.0f);
}