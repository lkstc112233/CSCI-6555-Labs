// standard
#include <cassert>
#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "fileop/fileLoader.h"
#include "Shaders/Shader.h"
#include "Texture/Texture.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}


ShaderProgram *program;
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glm::mat4 teapotTrans(1.0f);
		static float angle = 0;
		// teapotTrans = glm::translate(teapotTrans, glm::vec3(0, -0.3, 0));
		// teapotTrans = glm::translate(teapotTrans, glm::vec3(0, 0, 0.6));
		// teapotTrans = glm::rotate(teapotTrans, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
		teapotTrans = glm::rotate(teapotTrans, float(angle += M_PI / 60.), glm::normalize(glm::vec3(1.0, 1.0, 1.0)));
		program->setMatrix("transform", teapotTrans);
}
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

	ShaderProgram shaderProgram {
		Shader::createVertexShader("simpleShader.vert"),
		Shader::createFragmentShader("simpleShader.frag")};
	if (!shaderProgram.isValid())
	{
		return -4;
	}

	ShaderProgram teapotShaderProgram{
		Shader::createVertexShader("cubeShader.vert"),
		Shader::createFragmentShader("cubeShader.frag")};
	if (!teapotShaderProgram.isValid())
	{
		return -4;
	}
	program = &teapotShaderProgram;
	teapotShaderProgram.setMatrix("transform", glm::mat4(1.0f));

	Texture2D texture1("res/wall.jpg");
	Texture2D texture2("res/container.jpg");

	float vertices[] = {
		// positions		// colors           // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); 
	shaderProgram.use();

	shaderProgram.setValue("texture0", 0);
	shaderProgram.setValue("texture1", 1);
	shaderProgram.setValue("ratio", 0.2f);

#include "cube.h"
	glBindVertexArray(teapotVao);
	glBindBuffer(GL_ARRAY_BUFFER, teapotVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(teapotVertices), teapotVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapotEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapotIndices), teapotIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	teapotShaderProgram.use();
	teapotShaderProgram.setValue("texture0", 0);
	teapotShaderProgram.setValue("texture1", 1);
	teapotShaderProgram.setValue("ratio", 0.2f);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		texture1.bind(0);
		texture2.bind(1);
		
		float ratioValue = (sin(glfwGetTime()) / 2.0f) + 0.5f;
		shaderProgram.setValue("ratio", ratioValue);

		glm::mat4 trans(1.0f);
		trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		shaderProgram.setMatrix("transform", trans);

		glBindVertexArray(VAO);
		shaderProgram.use();
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		// // glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(teapotVao);
		teapotShaderProgram.use();
		glDrawElements(GL_TRIANGLES, sizeof(teapotIndices) / sizeof(unsigned), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}