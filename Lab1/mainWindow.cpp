// standard
#include <cassert>
#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "fileop/fileLoader.h"
#include "Shaders/Shader.h"

#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void update()
{
}

int main()
{
	if (!glfwInit())
	{
		return -3;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -2;
	}
	glViewport(0, 0, 800 * 2, 600 * 2);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	ShaderProgram shaderProgram{
		Shader::createVertexShader("simpleShader.vert"),
		Shader::createFragmentShader("simpleShader.frag")};
	if (!shaderProgram.isValid())
	{
		return -4;
	}

	int textureWidth, textureHeight, textureChannels;
	unsigned char *textureData = stbi_load(
		"res/wall.jpg",
		&textureWidth,
		&textureHeight,
		&textureChannels,
		0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(textureData);

	float vertices[] = {
		// positions		// colors           // texture coords
		0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {
		// note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VAO, teapotVao;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &teapotVao);
	glBindVertexArray(VAO);
	unsigned int VBO, teapotVbo;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &teapotVbo);
	unsigned int EBO, teapotEbo;
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &teapotEbo);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); 
	shaderProgram.use();

#include "teapot.h"
	glBindVertexArray(teapotVao);
	glBindBuffer(GL_ARRAY_BUFFER, teapotVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(teapotVertices), teapotVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapotEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapotIndices), teapotIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	shaderProgram.use();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// // glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glDrawElements(GL_TRIANGLES, sizeof(teapotIndices) / sizeof(unsigned), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}