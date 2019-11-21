#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stb_image.h>

#include <Shader_m.cpp>

#include <iostream>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


class TextureDemo
{
public:
	Shader_m tureShader;
	float prop = 0.3f;
	float width, height;

	
	float point[32] = {
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,1.0f,
		0.5f, 0.5f, 0.0f,0.0f, 1.0f, 0.0f, 1.0f,1.0f, 
		0.5f, -0.5f, 0.0f,0.0f, 0.0f, 1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, 0.0f,1.0f, 1.0f, 0.0f, 0.0f,0.0f
	};

	int indexs[6] = {
		0,1,2,
		0,2,3
	};
	TextureDemo(float w, float h) {
		width = w;
		height = h;
	
	}

	void init(GLFWwindow* window) {
		glEnable(GL_DEPTH_TEST);

		tureShader = Shader_m("D:/VSWorkspace/LearnGL/shader/TextureVerShader.shader", "D:/VSWorkspace/LearnGL/shader/TextureFrameShader.shader");

		// 纹理
		unsigned int texture1 = loadTexture("D:/VSWorkspace/LearnGL/res/blur.png",GL_REPEAT);
		unsigned int texture2 = loadTexture("D:/VSWorkspace/LearnGL/res/awesomeface.png", GL_REPEAT);
		
		// 顶点
		unsigned int VBO, VAO, EBO;
	
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &EBO);
		glGenBuffers(1,&VBO);
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);
		// 坐标
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// 颜色
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// 纹理坐标
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		// 纹理
		tureShader.use();
		tureShader.setInt("aTexture1", 0);
		tureShader.setInt("aTexture2", 1);
		tureShader.setFloat("prop", prop);

		// 变换
		//glm::mat4 trans1 = glm::mat4(1.0f);
		
		//trans1 = glm::translate(trans1, glm::vec3(0.5f, 0.5f, 0.0f));
		//trans1 = glm::scale(trans1, glm::vec3(0.5f, 0.5f, 1.0f));
		//trans1 = glm::rotate(trans1, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 model,view,projection;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
		tureShader.setMat4("view", view);
		tureShader.setMat4("projection", projection);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);

			tureShader.use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.5f,1.0f,1.0f));
			tureShader.setMat4("model", model);
			//tureShader.setMat4("trans", trans1);			

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.5f, 1.0f, 1.0f));
			tureShader.setMat4("model", model);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);


			//glm::mat4 trans2 = glm::mat4(1.0f);
			//trans2 = glm::scale(trans2, glm::vec3(sin(glfwGetTime()), sin(glfwGetTime()), sin(glfwGetTime())));
			//tureShader.setMat4("trans", trans2);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glfwSwapBuffers(window);
			glfwPollEvents();

		}
		
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		glfwTerminate();

	}


	unsigned int  loadTexture(std::string path,GLint wrap) {
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

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			prop = prop + 0.0005f;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			prop = prop - 0.0005f;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			prop = prop - 0.0005f;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			prop = prop + 0.0005f;

		if (prop <= 0.0f) {
			prop = 0.0f;
		}
		else if (prop >= 1.0f) {
			prop = 1.0f;
		}

		//std::cout << "prop is : " << prop << std::endl;
		tureShader.setFloat("prop", prop);
	}
	void mource_callback(GLFWwindow *window, double xpos, double ypos) {
		
	}

	void mource_scroll(GLFWwindow *window, double xpos, double ypos) {

	}


private:

	

};
