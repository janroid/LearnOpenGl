#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <Camera.cpp>
#include <Shader_m.cpp>

#include <Model.cpp>
#include <Mesh.cpp>

using namespace std;

class ObjectDemo
{
	float curTime, deltaTime;
	float width, height;
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 155.0f));

public:
	ObjectDemo(float w, float h) {
		width = w;
		height = h;
	}

	void init(GLFWwindow* window) {
		glEnable(GL_DEPTH_TEST);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标脚垫

		Shader_m saturnShader = Shader_m("D:/VSWorkspace/LearnGL/shader/ObjVerShader.shader", "D:/VSWorkspace/LearnGL/shader/ObjFrameShader.shader");
		Shader_m mimasShader = Shader_m("D:/VSWorkspace/LearnGL/shader/ObjsVerShader.shader", "D:/VSWorkspace/LearnGL/shader/ObjsFrameShader.shader");
		Model staturnModel = Model("D:/VSWorkspace/LearnGL/res/planet/planet.obj");
		Model mimasModel = Model("D:/VSWorkspace/LearnGL/res/rock/rock.obj");

		unsigned int amount = 10000;
		glm::mat4* modelMatrices;
		modelMatrices = new glm::mat4[amount];
		srand(glfwGetTime()); // initialize random seed	
		float radius = 150.0;
		float offset = 25.0f;
		for (unsigned int i = 0; i < amount; i++)
		{
			glm::mat4 model;
			// 1. translation: displace along circle with 'radius' in range [-offset, offset]
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. scale: Scale between 0.05 and 0.25f
			float scale = (rand() % 20) / 100.0f + 0.05;
			model = glm::scale(model, glm::vec3(scale));

			// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. now add to list of matrices
			modelMatrices[i] = model;
		}

		unsigned int OVBO;
		glGenBuffers(1, &OVBO);

		glBindBuffer(GL_ARRAY_BUFFER, OVBO);
		glBufferData(GL_ARRAY_BUFFER, amount*sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

		for (int i = 0; i < mimasModel.meshes.size(); i++) {
			unsigned int VAO = mimasModel.meshes[i].VAO;

			glBindVertexArray(VAO);

			GLsizei vec4Size = sizeof(glm::vec4);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)vec4Size);

			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2*vec4Size));

			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3*vec4Size));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}


		curTime = glfwGetTime();
		while (!glfwWindowShouldClose(window)) {
			deltaTime = glfwGetTime() - curTime;
			curTime = glfwGetTime();

			processInput(window);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 view = camera.GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);

			mimasShader.use();
			mimasShader.setMat4("view", view);
			mimasShader.setMat4("projection", projection);

			saturnShader.use();
			saturnShader.setMat4("view", view);
			saturnShader.setMat4("projection", projection);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
			saturnShader.setMat4("model",model);
			
			staturnModel.draw(&saturnShader);

			mimasShader.use();
			mimasShader.setInt("texture_diffuse1", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mimasModel.textureLoaded[0].id);
			for (int i = 0; i < mimasModel.meshes.size(); i++) {
				glBindVertexArray(mimasModel.meshes[i].VAO);
				glDrawElementsInstanced(GL_TRIANGLES, mimasModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
				glBindVertexArray(0);
			}


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

