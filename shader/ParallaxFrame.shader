#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 fPos;
    vec2 fCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float hScale;
uniform bool parallax;

vec2 ParallaxMapping(vec2 coords, vec3 viewDir){
    const float minLayer = 8;
    const float maxLayer = 32;
    float layerNum = mix(maxLayer,minLayer,abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

    float layerDepth = 1.0 / layerNum;
    float curLayDepth = 0.0;

    vec2 p = viewDir.xy / viewDir.z * hScale;
    vec2 deltaTexCoords = p / layerNum; // 每次改变的coords间隔
    vec2 curCoords = coords;
    float curTexDepth = texture(depthMap, curCoords).r;

    while(curLayDepth < curTexDepth){
        curLayDepth = curLayDepth + layerDepth;
        curCoords = curCoords - deltaTexCoords;

        curTexDepth = texture(depthMap, curCoords).r;
    }

    vec2 prevTexCoords = curCoords + deltaTexCoords;
    float afterDepth  = curTexDepth - curLayDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - curLayDepth + layerDepth;
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + curCoords * (1.0 - weight);


    return finalTexCoords;
}

void main(){
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.fCoords;
    if(parallax){
        texCoords = ParallaxMapping(fs_in.fCoords,  viewDir);
        if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0){
            discard;
        }
    }
             
    // Obtain normal from normal map
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
   
    // Get diffuse color
    vec3 color = texture(diffuseMap, texCoords).rgb;
    // Ambient
    vec3 ambient = 0.1 * color;
    // Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // Specular    
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(halfwayDir, normal), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}