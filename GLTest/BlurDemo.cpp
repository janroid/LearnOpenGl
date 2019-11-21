#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stb_image.h>

#include <Shader_m.cpp>

#include <iostream>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


class BlurDemo
{
public:
	float width, height;
	float curTime, deltaTime;

	float points[20] = {
		-1.0f, 1.0f, 0.0f,  0.0f,1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,0.0f
	};

	float quadVertices[30] = {
		-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,0.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,0.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,0.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,0.0f,  1.0f, 1.0f
	};

	int indexs[6] = {
		0,1,2,
		0,2,3
	};

	BlurDemo(float w, float h) {
		width = w;
		height = h;

	}

	void init(GLFWwindow* window) {
		glEnable(GL_DEPTH_TEST);

		Shader_m shader = Shader_m("D:/VSWorkspace/LearnGL/shader/BlurVer.shader", "D:/VSWorkspace/LearnGL/shader/BlurFrame.shader");
		Shader_m blurShader = Shader_m("D:/VSWorkspace/LearnGL/shader/BlurVer.shader", "D:/VSWorkspace/LearnGL/shader/BlurFBOFrame.shader");

		// 纹理
		unsigned int texture = loadTexture("D:/VSWorkspace/LearnGL/res/blur.png", GL_REPEAT);
		unsigned int wood = loadTexture("D:/VSWorkspace/LearnGL/res/wood.png", GL_REPEAT);

		unsigned int VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		unsigned int quadVAO, quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);

		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		// FBO
		unsigned int blurFBO[2];
		unsigned int blurBuffer[2];

		glGenFramebuffers(2, blurFBO);
		glGenTextures(2, blurBuffer);
		for (int i = 0; i < 2; i++) {
			glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[i]);
			glBindTexture(GL_TEXTURE_2D, blurBuffer[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurBuffer[i], 0);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}


		shader.use();
		shader.setInt("aTexture",0);

		blurShader.use();
		blurShader.setInt("image", 0);

		float multi = 1;
		while (!glfwWindowShouldClose(window))
		{

			multi = abs(glm::sin(glm::radians((float)glfwGetTime()) * 10) * 4);
			std::cout << "multi = " << multi << std::endl;
			processInput(window);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[0]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			bool horizontal = GL_TRUE, isFirst = true;
			blurShader.use();
			blurShader.setFloat("mul", multi);
			for (int i = 0; i < 10; i++) {
				glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[horizontal]);
				blurShader.setBool("horizontal", horizontal);
;				glBindTexture(GL_TEXTURE_2D,isFirst ? texture : blurBuffer[!horizontal]);
				glBindVertexArray(quadVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);
				if (isFirst)
					isFirst = false;

				horizontal = !horizontal;
			
			};
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shader.use();
			glBindTexture(GL_TEXTURE_2D, blurBuffer[!horizontal]);
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			glfwSwapBuffers(window);
			glfwPollEvents();

		}
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		glfwTerminate();

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

	void processInput(GLFWwindow *window) {

		/*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)*/


	}
	void mource_callback(GLFWwindow *window, double xpos, double ypos) {

	}

	void mource_scroll(GLFWwindow *window, double xpos, double ypos) {

	}


private:



};
