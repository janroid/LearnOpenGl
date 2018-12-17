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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mource_callback(GLFWwindow *window, double xpos, double ypos);
void mource_scroll(GLFWwindow *window, double xpos, double ypos);
unsigned int loadTexture(std::string path);

int width = 800;
int height = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	float cubeVers[] = {
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

	glm::vec3 cubePositions[] = {
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

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "JanRoid", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mource_callback);
	glfwSetScrollCallback(window, mource_scroll);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	Shader_m ourShader("D:/VSWorkspace/LearnGL/shader/cubeVerShader.shader", "D:/VSWorkspace/LearnGL/shader/cubeFrameShader.shader");
	Shader_m lightShader("D:/VSWorkspace/LearnGL/shader/lightVerShader.shader", "D:/VSWorkspace/LearnGL/shader/lightFrameShader.shader");

	unsigned int VAO, VBO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVers), cubeVers, GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int diffuseMap = loadTexture("D:/VSWorkspace/LearnGL/res/box.png");
	ourShader.use();
	ourShader.setInt("material.diffuse",0);

	unsigned int specularMap = loadTexture("D:/VSWorkspace/LearnGL/res/box_light_c.png");
	ourShader.setInt("material.specular", 1);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float curTime = glfwGetTime();
		if (lastFrame == 0.0f) {
			lastFrame = curTime;
		}
		else {
			deltaTime = curTime - lastFrame;
			lastFrame = curTime;
		}
		
		// 光源移动
		//lightPos.x = 1.0f + sin(glfwGetTime())*2.0f;
		//lightPos.y = sin(glfwGetTime() / 2.0) * 1.0f;


		glm::mat4 projection, view, model;

		projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		view = camera.GetViewMatrix();

		ourShader.use();//
		ourShader.setMat4("model", model);
		ourShader.setMat4("projection", projection);
		ourShader.setVec3("cameraPos", camera.Position);
		ourShader.setMat4("view", view);
		//model = glm::rotate(model,glm::radians(-45.0f), glm::vec3(1.0f, -1.0f, 0.0f));


		// 定向光
		ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// 点光源
		ourShader.setVec3("pointLight[0].position", pointLightPositions[0]);
		ourShader.setFloat("pointLight[0].Kc", 1.0f);
		ourShader.setFloat("pointLight[0].Kl", 0.09f);
		ourShader.setFloat("pointLight[0].Kq", 0.032f);
		ourShader.setVec3("pointLight[0].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLight[0].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLight[0].specular", 1.0f, 1.0f, 1.0f);

		ourShader.setVec3("pointLight[1].position", pointLightPositions[1]);
		ourShader.setFloat("pointLight[1].Kc", 1.0f);
		ourShader.setFloat("pointLight[1].Kl", 0.09f);
		ourShader.setFloat("pointLight[1].Kq", 0.032f);
		ourShader.setVec3("pointLight[1].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLight[1].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLight[1].specular", 1.0f, 1.0f, 1.0f);

		ourShader.setVec3("pointLight[2].position", pointLightPositions[2]);
		ourShader.setFloat("pointLight[2].Kc", 1.0f);
		ourShader.setFloat("pointLight[2].Kl", 0.09f);
		ourShader.setFloat("pointLight[2].Kq", 0.032f);
		ourShader.setVec3("pointLight[2].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLight[2].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLight[2].specular", 1.0f, 1.0f, 1.0f);

		ourShader.setVec3("pointLight[3].position", pointLightPositions[3]);
		ourShader.setFloat("pointLight[3].Kc", 1.0f);
		ourShader.setFloat("pointLight[3].Kl", 0.09f);
		ourShader.setFloat("pointLight[3].Kq", 0.032f);
		ourShader.setVec3("pointLight[3].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLight[3].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLight[3].specular", 1.0f, 1.0f, 1.0f);
		

		// 设置光照
		//ourShader.setVec3("light.position",lightPos);
		ourShader.setVec3("spotLight.direction", camera.Front);
		ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		ourShader.setFloat("spotLight.outCutOff", glm::cos(glm::radians(17.5f)));
		ourShader.setFloat("spotLight.Kc", 1.0f);
		ourShader.setFloat("spotLight.Kl", 0.09f);
		ourShader.setFloat("spotLight.Kq", 0.032f);
		ourShader.setVec3("spotLight.ambient", 0.0f,0.0f,0.0f);
		ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("spotLight.specular",1.0f, 1.0f, 1.0f);

	
		


		// 设置材质
		ourShader.setVec3("material.ambient",1.0f, 0.5f, 0.31f);
		//ourShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		ourShader.setFloat("material.shininess", 32.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);



		// 绘制
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model,glm::radians(angle), glm::vec3(1.0f,0.3f,0.5f));

			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		
		}
		
		lightShader.use();
		lightShader.setMat4("projection",projection);
		lightShader.setMat4("view",view);
		
		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 4; i++) {
			glm::mat4 model3;
			model3 = glm::translate(model3,pointLightPositions[i]);
			model3 = glm::scale(model3, glm::vec3(0.2f));

			lightShader.setMat4("model", model3);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();


	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {

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

void mource_callback(GLFWwindow *window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);

}

void mource_scroll(GLFWwindow *window, double xpos, double ypos) {
	camera.ProcessMouseScroll(ypos);
}

unsigned int loadTexture(std::string path)
{	
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Faild to load texture" << std::endl;
	}
	stbi_image_free(data);

	return textureID;
}
