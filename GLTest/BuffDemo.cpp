#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>

#include <Camera.cpp>
#include <Shader_m.cpp>

using namespace std;

class BuffDemo {
	float curTime, deltaTime;
	float width, height;
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

public:
	const GLfloat PI = 3.14159265358979323846f;
	float majorRadius = 0.4;  // 大圆半径
	float minorRadius = 0.15; // 外圈半径
	int numMajor = 50;  // 大圆份数
	int numMinor = 50;  // 小圆份数a

	float floorPoint[20] = {
		-20.0f, -0.41f, 20.0f,0.0f, 0.0f,
		20.0f, -0.41f, 20.0f,10.0f, 0.0f,
		20.0f, -0.41f, -20.0f,10.0f, 10.0f,
		-20.0f, -0.41f, -20.0f, 0.0f, 10.0f
	};

	BuffDemo(float w, float h)
	{
		width = w;
		height = h;

	}

	void init(GLFWwindow* window) {
		glEnable(GL_DEPTH_TEST);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标脚垫

		// Make Torus Point
		
		double majorStep = 2.0f * PI / numMajor;
		double minorStep = 2.0f * PI / numMinor;

		vector<GLfloat> torusVertices;


		for (int i = 0; i < numMajor; ++i) {
			double a0 = i * majorStep;
			double a1 = a0 + majorStep;
			GLfloat x0 = (GLfloat)cos(a0);
			GLfloat y0 = (GLfloat)sin(a0);
			GLfloat x1 = (GLfloat)cos(a1);
			GLfloat y1 = (GLfloat)sin(a1);
			
			for (int j = 0; j <= numMinor; ++j) {
				double b = j * minorStep;
				GLfloat c = (GLfloat)cos(b);
				GLfloat r = minorRadius * c + majorRadius;
				GLfloat z = minorRadius * (GLfloat)sin(b);

				// 顶点
				torusVertices.push_back(x0 * r);
				torusVertices.push_back(y0 * r);
				torusVertices.push_back(z);
				// 法线
				torusVertices.push_back(x0 * c);
				torusVertices.push_back(y0 * c);
				torusVertices.push_back(z / minorRadius);
				// 顶点
				torusVertices.push_back(x1 * r);
				torusVertices.push_back(y1 * r);
				torusVertices.push_back(z);
				// 法线
				torusVertices.push_back(x1 * c);
				torusVertices.push_back(y1 * c);
				torusVertices.push_back(z / minorRadius);

				b = (j + 1) * minorStep;
				c = (GLfloat)cos(b);
				r = minorRadius * c + majorRadius;
				z = minorRadius * (GLfloat)sin(b);

				// 顶点
				torusVertices.push_back(x0 * r);
				torusVertices.push_back(y0 * r);
				torusVertices.push_back(z);
				// 法线
				torusVertices.push_back(x0 * c);
				torusVertices.push_back(y0 * c);
				torusVertices.push_back(z / minorRadius);
				// 顶点
				torusVertices.push_back(x1 * r);
				torusVertices.push_back(y1 * r);
				torusVertices.push_back(z);
				// 法线
				torusVertices.push_back(x1 * c);
				torusVertices.push_back(y1 * c);
				torusVertices.push_back(z / minorRadius);
			}
		}



		unsigned int floorTex = loadTexture("D:/VSWorkspace/LearnGL/res/floor.bmp", GL_NEAREST);
		Shader_m ourShader = Shader_m("D:/VSWorkspace/LearnGL/shader/BuffVer.shader", "D:/VSWorkspace/LearnGL/shader/BuffFrame.shader");
		Shader_m floorShader = Shader_m("D:/VSWorkspace/LearnGL/shader/BuffFloorVer.shader", "D:/VSWorkspace/LearnGL/shader/BuffFloorFrame.shader");

		unsigned int TVAO, TVBO, FVAO, FVBO;
		glGenBuffers(1, &TVBO);
		glGenVertexArrays(1, &TVAO);

		glBindVertexArray(TVAO);
		glBindBuffer(GL_ARRAY_BUFFER, TVBO);
		glBufferData(GL_ARRAY_BUFFER, torusVertices.size() * sizeof(GLfloat), &torusVertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &FVBO);
		glGenVertexArrays(1, &FVAO);
		
		glBindVertexArray(FVAO);
		glBindBuffer(GL_ARRAY_BUFFER, FVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(floorPoint), floorPoint, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// PBO 
		unsigned int blueTexs[5];
		unsigned int PBO;
		glGenBuffers(5, blueTexs);
		glGenBuffers(1, &PBO);

		float pixeBuffSize = width * height * 3 * sizeof(GL_BYTE);
		void* data = (void*)malloc(pixeBuffSize);
		memset(data, 0x00, pixeBuffSize);
		for (int i = 0; i < 5; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, blueTexs[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}

		glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO);
		glBufferData(GL_PIXEL_PACK_BUFFER, pixeBuffSize, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

		ourShader.use();
		ourShader.setVec3("lightDir", 0.2f, -1.0f, -0.5f);
		ourShader.setVec3("ambColor", 0.1f, 0.1f, 0.1f);
		ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		ourShader.setInt("textureUnit0", 0);
		ourShader.setInt("textureUnit1", 1);
		ourShader.setInt("textureUnit2", 2);
		ourShader.setInt("textureUnit3", 3);
		ourShader.setInt("textureUnit4", 4);

		floorShader.use();
		floorShader.setInt("aTexture", 0);
		int index = 0;

		while (!glfwWindowShouldClose(window)) {
			deltaTime = glfwGetTime() - curTime;
			curTime = glfwGetTime();
			

			processInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
			glm::mat4 model = glm::mat4(1.0f);

			view = camera.GetViewMatrix();
			projection = glm::perspective(glm::radians(camera.Zoom), width / height, 0.1f, 100.0f);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, floorTex);

			floorShader.use();
			floorShader.setMat4("view", view);
			floorShader.setMat4("projection", projection);
			floorShader.setMat4("model", model);
			glBindVertexArray(FVAO);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			ourShader.use();
			ourShader.setVec3("cameraPos", camera.Position);
			ourShader.setMat4("view", view);
			ourShader.setMat4("projection", projection);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
			model = glm::rotate(model, glm::radians((float)glfwGetTime() * -30.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
			ourShader.setMat4("model", model);

			glBindVertexArray(TVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, torusVertices.size() / 6);

			glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO);
			glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
			glActiveTexture(GL_TEXTURE0 + (index % 5));
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

			index += 1;
			glfwSwapBuffers(window);
			glfwPollEvents();

		}

		glDeleteBuffers(1,&TVBO);
		glDeleteVertexArrays(1, &TVAO);
		glDeleteBuffers(1, &FVBO);
		glDeleteVertexArrays(1, &FVAO);
		//glDeleteBuffers(1, &TEBO);

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


};
