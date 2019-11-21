#version 330 core

out vec4 FragColor;

in vec2 fCoords;

uniform sampler2D image;
uniform float mul;

uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main(){
    vec2 offset = 1.0f / textureSize(image, 0);
    offset = offset * mul;

    vec3 color = texture(image, fCoords).rgb * weight[0];

    if (horizontal){
        for (int i = 1; i < 5; i++){
            color += texture(image, vec2(fCoords.x + offset.x * i, fCoords.y)).rgb * weight[i];
            color += texture(image, vec2(fCoords.x - offset.x * i, fCoords.y)).rgb * weight[i];
        }
    }else{
        for (int i = 1; i < 5; i++){
            color += texture(image, vec2(fCoords.x, fCoords.y + offset.y * i)).rgb * weight[i];
            color += texture(image, vec2(fCoords.x, fCoords.y - offset.y * i)).rgb * weight[i];
        }
    }

    FragColor = vec4(color, 1.0f);
}