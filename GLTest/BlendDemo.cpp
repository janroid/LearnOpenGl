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

class BlendDemo {
	float curTime, deltaTime;
	float width, height;
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));


public:
	const GLfloat PI = 3.14159265358979323846f;
	//将球横纵划分成50*50的网格
	const int Y_SEGMENTS = 100;
	const int X_SEGMENTS = 100;

	float skyboxVertices[108] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	 float cubeVertices[216] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f

    };
    
	float frameVertices[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};


	BlendDemo(float w, float h)
	{
		width = w;
		height = h;

	}

	void init(GLFWwindow* window) {
		glEnable(GL_DEPTH_TEST);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标脚垫

		// 生成球的顶点与顺序
		vector<float> sphereVertices;
		vector<int> sphereIndexs;

		for (int i = 0; i < Y_SEGMENTS; i++) 
		{
			for (int j = 0; j < X_SEGMENTS; j++) 
			{
				float xSegment = (float)j / (float)X_SEGMENTS;
				float ySegment = (float)i / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				sphereVertices.push_back(xPos);
				sphereVertices.push_back(yPos);
				sphereVertices.push_back(zPos);

			}
		}

		//生成球的Indices
		for (int i = 0; i < Y_SEGMENTS; i++)
		{
			for (int j = 0; j < X_SEGMENTS; j++)
			{
				sphereIndexs.push_back(i * (X_SEGMENTS + 1) + j);
				sphereIndexs.push_back((i + 1) * (X_SEGMENTS + 1) + j);
				sphereIndexs.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
				sphereIndexs.push_back(i* (X_SEGMENTS + 1) + j);
				sphereIndexs.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
				sphereIndexs.push_back(i * (X_SEGMENTS + 1) + j + 1);
			}
		}


		unsigned int cubeTex = loadTexture("D:/VSWorkspace/LearnGL/res/box.png",GL_REPEAT);
		vector<std::string> faces
		{
			"D:/VSWorkspace/LearnGL/res/skybox/right.jpg",
			"D:/VSWorkspace/LearnGL/res/skybox/left.jpg",
			"D:/VSWorkspace/LearnGL/res/skybox/top.jpg",
			"D:/VSWorkspace/LearnGL/res/skybox/bottom.jpg",
			"D:/VSWorkspace/LearnGL/res/skybox/front.jpg",
			"D:/VSWorkspace/LearnGL/res/skybox/back.jpg"
		};
		unsigned int skyTex = loadCubeTexture(faces);

		Shader_m ourShader = Shader_m("D:/VSWorkspace/LearnGL/shader/BlendVerShader.shader", "D:/VSWorkspace/LearnGL/shader/BlendFrameShader.shader");
		Shader_m frameShader = Shader_m("D:/VSWorkspace/LearnGL/shader/FrameVerShader.shader", "D:/VSWorkspace/LearnGL/shader/FrameFrameShader.shader");
		Shader_m skyShader = Shader_m("D:/VSWorkspace/LearnGL/shader/SkyVerShader.shader", "D:/VSWorkspace/LearnGL/shader/SkyFrameShader.shader");

		unsigned int CVAO, CVBO, FVAO, FVBO, SVAO, SVBO, CEBO;

		glGenVertexArrays(1, &CVAO);
		glGenBuffers(1, &CVBO);
		glGenBuffers(1, &CEBO);

		glBindVertexArray(CVAO);
		glBindBuffer(GL_ARRAY_BUFFER, CVBO);
		glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndexs.size() * sizeof(int), &sphereIndexs[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// 天空盒
		glGenVertexArrays(1, &SVAO);
		glGenBuffers(1, &SVBO);

		glBindVertexArray(SVAO);
		glBindBuffer(GL_ARRAY_BUFFER, SVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// uniform 变量
		unsigned int uniID;
		glGenBuffers(1, &uniID);

		glBindBuffer(GL_UNIFORM_BUFFER, uniID);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniID, 0, 2 * sizeof(glm::mat4));


		//// 帧缓存
		glGenVertexArrays(1, &FVAO);
		glGenBuffers(1, &FVBO);

		glBindVertexArray(FVAO);
		glBindBuffer(GL_ARRAY_BUFFER, FVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(frameVertices), frameVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		unsigned int frameBuff;
		glGenFramebuffers(1, &frameBuff);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuff);

		unsigned int texColorBuffer;
		glGenTextures(1, &texColorBuffer);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		ourShader.use();
		ourShader.setInt("aTexture", 0);
		ourShader.setVec3("cenPos", glm::vec3(0.0f, 0.0f, 0.0f));
		unsigned vid = glGetUniformBlockIndex(ourShader.ID, "Matrices");
		glUniformBlockBinding(ourShader.ID, vid, 0);

		skyShader.use();
		skyShader.setInt("skybox", 0);

		unsigned svid = glGetUniformBlockIndex(skyShader.ID, "Matrices");
		glUniformBlockBinding(skyShader.ID, svid, 0);

		frameShader.use();
		frameShader.setInt("fTexture", 0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		while (!glfwWindowShouldClose(window)) {
			deltaTime = glfwGetTime() - curTime;
			curTime = glfwGetTime();

			processInput(window);

			//glBindFramebuffer(GL_FRAMEBUFFER, frameBuff);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
			glm::mat4 model = glm::mat4(1.0f);

			view = camera.GetViewMatrix();
			projection = glm::perspective(glm::radians(camera.Zoom), width / height, 0.1f, 100.0f);

			glBindBuffer(GL_UNIFORM_BUFFER, uniID);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);


			// 画箱子
			ourShader.use();
			ourShader.setVec3("cameraPos", camera.Position);

			glBindVertexArray(CVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyTex);
			model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -0.5f));
			ourShader.setMat4("model",model);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			glDrawElements(GL_TRIANGLES, Y_SEGMENTS * X_SEGMENTS * 6, GL_UNSIGNED_INT, 0);


			/*model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);*/

			// 画天空盒
			glDepthFunc(GL_LEQUAL);

			skyShader.use();
			model = glm::mat4(1.0f);
			skyShader.setMat4("model", model);

			glBindVertexArray(SVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyTex);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			glDepthFunc(GL_LESS);

			// 帧缓冲
			/*glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			frameShader.use();
			glBindVertexArray(FVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texColorBuffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);*/


			glfwSwapBuffers(window);
			glfwPollEvents();
		
		}
		
		glDeleteVertexArrays(1, &CVAO);
		glDeleteBuffers(1, &CVBO);
		glDeleteVertexArrays(1, &FVAO);
		glDeleteBuffers(1, &FVBO);
		glDeleteVertexArrays(1, &SVAO);
		glDeleteBuffers(1, &SVBO);
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

