#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <Shader_m.cpp>
#include <Camera.cpp>
#include <FlightDemo.cpp>

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


int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "JanRoid", NULL, NULL);
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

	FlightDemo(width, height, window);


	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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
