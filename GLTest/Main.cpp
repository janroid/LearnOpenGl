#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <Shader_m.cpp>
#include <Camera.cpp>
//#include <FlightDemo.cpp>
//#include <PointDemo.cpp>
//#include <TextureDemo.cpp>
//#include <CameraDemo.cpp>
//#include <LightingDemo.cpp>
//#include <AssimpDemo.cpp>
//#include <BlendDemo.cpp>
//#include <GeometryDemo.cpp>
//#include <ObjectDemo.cpp>
//#include <MultiDemo.cpp>
//#include <MappingDemo.cpp>
//#include <NormalMapDemo.cpp>
//#include <ParallaxDemo.cpp>
//#include <HdrDemo.cpp>
//#include <BloomDemo.cpp>
//#include <BlurDemo.cpp>
//#include <DelayDemo.cpp>
//#include <SSAODemo.cpp>
#include <BuffDemo.cpp>

#include <iostream>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include<assimp\scene.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mource_callback(GLFWwindow *window, double xpos, double ypos);
void mource_scroll(GLFWwindow *window, double xpos, double ypos);
unsigned int loadTexture(std::string path);

int width = 1280;
int height = 720;

float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

BuffDemo objDemo = BuffDemo(width,height);


int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

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

	//FlightDemo(width, height, window);
	//PointDemo(width, height, window);
	//TextureDemo(width, height, window);
	objDemo.init(window);

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mource_callback(GLFWwindow *window, double xpos, double ypos) {
	objDemo.mource_callback(window, xpos, ypos);

}

void mource_scroll(GLFWwindow *window, double xpos, double ypos) {
	objDemo.mource_scroll(window, xpos, ypos);
}
