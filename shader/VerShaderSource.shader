#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCoord;
//out vec3 ourColor;
out vec2 texCoord;

uniform mat4 model; //模型矩阵
uniform mat4 view; // 观察矩阵
uniform mat4 projection;  // 投影矩阵

void main()
{
	//gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);  -- 矩阵实例
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	//ourColor = aColor;
	texCoord = aTexCoord;
};