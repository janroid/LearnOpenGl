#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stb_image.h>

#include <Shader_m.cpp>

#include <iostream>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


class PointDemo
{
public:
	/*float vCoast[72] = {
		-2.0f, 0.0f, -2.0f,
		2.0f, 0.0f, -2.0f,
		0.0f, 4.0f, 0.0f,

		2.0f, 0.0f, -2.0f,
		2.0f, 0.0f, 2.0f,
		0.0f, 4.0f, 0.0f,

		2.0f, 0.0f, 2.0f,
		-2.0f, 0.0f, 2.0f,
		0.0f, 4.0f, 0.0f,

		-2.0f, 0.0f, 2.0f,
		-2.0f, 0.0f, -2.0f,
		0.0f, 4.0f, 0.0f };*/

	float vCoast[9] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};


	PointDemo(int w, int h, GLFWwindow* window) {
		glEnable(GL_DEPTH_TEST);

		for (int i = 0; i < 72; i++) {
			vCoast[i] = vCoast[i] / 5.0f;
		}
		
		Shader_m vShader("D:/VSWorkspace/LearnGL/shader/PointVerShader.shader", "D:/VSWorkspace/LearnGL/shader/PointFrameShader.shader");

		unsigned int VBO, VAO;

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vCoast), vCoast, GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3*sizeof(float),(void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		while (!glfwWindowShouldClose(window)) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			vShader.use();
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 12);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		glfwTerminate();
	}


private:

};

