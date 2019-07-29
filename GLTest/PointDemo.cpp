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
	float vCoast[30] = {
		0.0f,0.0f, 0.0f,1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
		-0.5, -0.5, 0.0f,0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.0f,0.0f, 0.0f, 1.0f,
	};

	unsigned indices[6] = {
		0,1,2,
		0,3,4,
	};

	PointDemo(int w, int h, GLFWwindow* window) {
		glEnable(GL_DEPTH_TEST);
		
		Shader_m vShader("D:/VSWorkspace/LearnGL/shader/PointVerShader.shader", "D:/VSWorkspace/LearnGL/shader/PointFrameShader.shader");

		unsigned int VBO, VAO, EBO;

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vCoast), vCoast, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 6*sizeof(float),(void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		//ÏßÄ£Ê½
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		while (!glfwWindowShouldClose(window)) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			vShader.use();

			glBindVertexArray(VAO);

			glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT,0);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		glfwTerminate();
	}


private:

};

