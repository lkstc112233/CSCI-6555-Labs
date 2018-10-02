#include "WindowInitializer.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	int bufferwidth, bufferheight;
	glfwGetFramebufferSize(window, &bufferwidth, &bufferheight);
	glViewport(0, 0, bufferwidth, bufferheight);
}

GLFWwindow *initializeWindow(const char* title, int width, int height) {
	if (!glfwInit())
	{
		std::cerr << "GLFW initialize failed" << std::endl;
		return nullptr;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Error occurred creating window." << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "GLAD initialize failed" << std::endl;
		return nullptr;
	}
	int bufferwidth, bufferheight;
	glfwGetFramebufferSize(window, &bufferwidth, &bufferheight);
	glViewport(0, 0, bufferwidth, bufferheight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return window;
}