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

class HdrDemo {
	float curTime, deltaTime;
	float width, height;
	bool hdr = true;
	bool hdrKeyPressed = false;
	float exposure = 1.0f;
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

public:
	float vertices[288] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	float quadVertices[20] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};


	HdrDemo(float w, float h) {
		width = w;
		height = h;
	}

	void init(GLFWwindow* window) {
		glEnable(GL_DEPTH_TEST);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标脚垫

		unsigned int mtexture = loadTexture("D:/VSWorkspace/LearnGL/res/wood.png", GL_REPEAT);
		Shader_m lightShader = Shader_m("D:/VSWorkspace/LearnGL/shader/HDRVer.shader", "D:/VSWorkspace/LearnGL/shader/HDRFrame.shader");
		Shader_m shader = Shader_m("D:/VSWorkspace/LearnGL/shader/HDRLightVer.shader", "D:/VSWorkspace/LearnGL/shader/HDRLightFrame.shader");

		// setup plane VAO
		unsigned int quadVAO, quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		unsigned int cubeVAO, cubeVBO;
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		// configure floating point framebuffer
	// ------------------------------------
		unsigned int hdrFBO;
		glGenFramebuffers(1, &hdrFBO);
		// create floating point color buffer
		unsigned int colorBuffer;
		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// create depth buffer (renderbuffer)
		unsigned int rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		// attach buffers
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// lighting info
		// -------------
		// positions
		std::vector<glm::vec3> lightPositions;
		lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
		lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
		lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
		lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
		// colors
		std::vector<glm::vec3> lightColors;
		lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
		lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
		lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
		lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

		// shader configuration
		// --------------------
		shader.use();
		shader.setInt("diffuseTexture", 0);
		lightShader.use();
		lightShader.setInt("hdrBuffer", 0);

		while (!glfwWindowShouldClose(window))
		{
			deltaTime = glfwGetTime() - curTime;
			curTime = glfwGetTime();

			processInput(window);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// 1. render scene into floating point framebuffer
			// -----------------------------------------------
			glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			shader.use();
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mtexture);
			// set lighting uniforms
			for (unsigned int i = 0; i < lightPositions.size(); i++)
			{
				shader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
				shader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
			}
			//shader.setVec3("viewPos", camera.Position);
			// render tunnel
			glm::mat4 model = glm::mat4();
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
			model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
			shader.setMat4("model", model);
			shader.setInt("inverse_normals", true);
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
			// --------------------------------------------------------------------------------------------------------------------------
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			lightShader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, colorBuffer);
			lightShader.setInt("hdr", hdr);
			lightShader.setFloat("exposure", exposure);
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

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

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			if (exposure > 0.0f)
				exposure -= 0.001f;
			else
				exposure = 0.0f;
		}
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			exposure += 0.001f;
		}
	}

private:



};
