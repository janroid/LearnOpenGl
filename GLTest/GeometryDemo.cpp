#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <Camera.cpp>
#include <Shader_m.cpp>

using namespace std;

class GeometryDemo {
	float curTime, deltaTime;
	float width, height;
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	
public:

	float points[20] = {
	 -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
	};

	GeometryDemo(float w, float h)
	{
		width = w;
		height = h;

	}

	void init(GLFWwindow* window) {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标脚垫
	
		Shader_m goShader = Shader_m("D:/VSWorkspace/LearnGL/shader/GoVerShader.shader", "D:/VSWorkspace/LearnGL/shader/GoFrameShader.shader", "D:/VSWorkspace/LearnGL/shader/GoShader.shader");

		unsigned VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2*sizeof(float)));
		
		glBindVertexArray(0);


		while (!glfwWindowShouldClose(window)) {
			deltaTime = glfwGetTime() - curTime;
			curTime = glfwGetTime();

			processInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			goShader.use();
			glBindVertexArray(VAO);
			glDrawArrays(GL_POINTS, 0, 4);
			


			glfwSwapBuffers(window);
			glfwPollEvents();

		}

	}

	unsigned int  loadTexture(std::string path, GLint wrap) {
		unsigned int textureID;
		glGenTextures(1, &textureID);

		//加载纹理
		int width, height, nrChannels;
		//stbi_set_flip_vertically_on_load(true);// 翻转图片
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			//设置纹理环绕，过滤方式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//设置边缘颜色，
			//float broderColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
			//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, broderColor);
		}
		else {
			std::cout << "Faild to load texture" << std::endl;
		}
		stbi_image_free(data);

		return textureID;
	}

	unsigned int loadCubeTexture(vector<string> path) {
		unsigned int textureID;
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		for (int i = 0; i < path.size(); i++) {
			int width, height, nrChannels;

			unsigned char *data = stbi_load(path[i].c_str(), &width, &height, &nrChannels, 0);
			if (data) {
				GLenum format;
				if (nrChannels == 1)
					format = GL_RED;
				else if (nrChannels == 3)
					format = GL_RGB;
				else if (nrChannels == 4)
					format = GL_RGBA;

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			}
			else {
				std::cout << "Faild to load texture cube" << std::endl;
			}
			stbi_image_free(data);
		}
		//设置纹理环绕，过滤方式
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return textureID;

	}

	bool isFirstMouse = true;
	double mouseX, mouseY;
	void mource_callback(GLFWwindow *window, double xpos, double ypos) {
		if (isFirstMouse) {
			mouseX = xpos;
			mouseY = ypos;

			isFirstMouse = false;
		}

		float subX = xpos - mouseX;
		float subY = mouseY - ypos;

		mouseX = xpos;
		mouseY = ypos;

		camera.ProcessMouseMovement(subX, subY, GL_TRUE);
	}

	void mource_scroll(GLFWwindow *window, double xpos, double ypos) {
		camera.ProcessMouseScroll(ypos);
	}

	void processInput(GLFWwindow *window) {
		if (deltaTime == 0)
			return;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}

private:




};