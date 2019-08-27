#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stb_image.h>

#include <Shader_m.cpp>
#include <Camera.cpp>

#include <iostream>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class LightingDemo
{

public:

	float curTime, deltaTime;
	int width, height;
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	float ambStrength = 0.1f; // 环境光
	float specularStrength = 0.5f; // 镜面反射强度
	float cutOff = 12.5f; // 聚光的角度
	float outOff = 17.5f; // 外圆锥

	float indexs[288] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 boxPos[10] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 lightPos[4] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	LightingDemo(int w, int h) {
		width = w;
		height = h;
	}

	void init(GLFWwindow* window) {
		glEnable(GL_DEPTH_TEST);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标脚垫

		Shader_m ourShader = Shader_m("D:/VSWorkspace/LearnGL/shader/LightingVerShader.shader", "D:/VSWorkspace/LearnGL/shader/LightingFrameShader.shader");
		Shader_m sunShader = Shader_m("D:/VSWorkspace/LearnGL/shader/SunVerShader.shader", "D:/VSWorkspace/LearnGL/shader/SunFrameShader.shader");

		unsigned int texture = loadTexture("D:/VSWorkspace/LearnGL/res/box.png", GL_REPEAT);
		unsigned int mirrorMap = loadTexture("D:/VSWorkspace/LearnGL/res/box_light.png", GL_REPEAT);

		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		unsigned int sunVAO;
		glGenVertexArrays(1, &sunVAO);

		glBindVertexArray(sunVAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		glm::vec3 sunColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 ourColor = glm::vec3(1.0f, 0.5f, 0.31f);

		ourShader.use();
		ourShader.setFloat("mater.shininess", 32.0f);

		ourShader.setInt("mater.diffField", 0);
		ourShader.setInt("mater.specField", 1);
		// 平行光
		ourShader.setVec3("dirLight.lightDir", -0.2f, -1.0f, -0.3f);
		ourShader.setVec3("dirLight.ambColor", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("dirLight.diffColor", 0.4f, 0.4f, 0.4f);
		ourShader.setVec3("dirLight.specColor", 0.5f, 0.5f, 0.5f);

		// point light 1
		ourShader.setVec3("pointLight[0].pos", lightPos[0]);
		ourShader.setVec3("pointLight[0].ambColor", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLight[0].diffColor", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLight[0].specColor", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLight[0].constant", 1.0f);
		ourShader.setFloat("pointLight[0].linear", 0.09);
		ourShader.setFloat("pointLight[0].quadratic", 0.032);
		// point light 2
		ourShader.setVec3("pointLight[1].pos", lightPos[1]);
		ourShader.setVec3("pointLight[1].ambColor", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLight[1].diffColor", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLight[1].specColor", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLight[1].constant", 1.0f);
		ourShader.setFloat("pointLight[1].linear", 0.09);
		ourShader.setFloat("pointLight[1].quadratic", 0.032);
		// point light 3
		ourShader.setVec3("pointLight[2].pos", lightPos[2]);
		ourShader.setVec3("pointLight[2].ambColor", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLight[2].diffColor", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLight[2].specColor", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLight[2].constant", 1.0f);
		ourShader.setFloat("pointLight[2].linear", 0.09);
		ourShader.setFloat("pointLight[2].quadratic", 0.032);
		// point light 4
		ourShader.setVec3("pointLight[3].pos", lightPos[3]);
		ourShader.setVec3("pointLight[3].ambColor", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLight[3].diffColor", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLight[3].specColor", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLight[3].constant", 1.0f);
		ourShader.setFloat("pointLight[3].linear", 0.09);
		ourShader.setFloat("pointLight[3].quadratic", 0.032);

		sunShader.use();
		sunShader.setVec3("sunColor", sunColor);

		curTime = glfwGetTime();
		float angle = 0;

		while (!glfwWindowShouldClose(window))
		{
			deltaTime = glfwGetTime() - curTime;
			curTime = glfwGetTime();
			angle = 20 * curTime;

			processInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mirrorMap);

			ourShader.use();
			projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
			view = camera.GetViewMatrix();
			
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);
			ourShader.setVec3("cameraPos", camera.Position);

			// spot light
			ourShader.setVec3("spotLight.ambColor", 0.0f, 0.0f, 0.0f);
			ourShader.setVec3("spotLight.diffColor", 1.0f, 1.0f, 1.0f);
			ourShader.setVec3("spotLight.specColor", 1.0f, 1.0f, 1.0f);
			ourShader.setFloat("spotLight.constant", 1.0f);
			ourShader.setFloat("spotLight.linear", 0.09);
			ourShader.setFloat("spotLight.quadratic", 0.032);
			ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			ourShader.setFloat("spotLight.outOff", glm::cos(glm::radians(15.0f)));
			ourShader.setVec3("spotLight.dir", camera.Front);

			//glm::vec3 mpos = glm::vec3(1.0f);
			//mpos.x = sunPos.x*cos(glm::radians(angle)) + sunPos.z * sin(glm::radians(angle));
			//mpos.y = sunPos.y;
			//mpos.z = sunPos.z * cos(glm::radians(angle)) - sunPos.x * sin(glm::radians(angle));
			//ourShader.setVec3("sun.sunPos", mpos);

			for (int i = 0; i < 10; i++) {
				model = glm::mat4(1.0f);
				model = glm::translate(model,boxPos[i]);
				float angle = 20 * i;
				model = glm::rotate(model,glm::radians(angle),glm::vec3(1.0,0.3,0.5));

				ourShader.setMat4("model", model);


				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				
			}

			sunShader.use();
			sunShader.setMat4("projection", projection);
			sunShader.setMat4("view", view);

			for (int i = 0; i < sizeof(lightPos); i++) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, lightPos[i]);
				model = glm::scale(model, glm::vec3(0.2f));

				sunShader.setMat4("model", model);

				glBindVertexArray(sunVAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);

			}	

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1,&VAO);
		glDeleteBuffers(1, &VBO);
	
	}


	unsigned int  loadTexture(std::string path, GLint wrap) {
		unsigned int textureID;
		glGenTextures(1, &textureID);

		//加载纹理
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);// 翻转图片
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