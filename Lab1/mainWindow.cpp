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
#include "Camera/Camera.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float PROJECTION_RATIO = float(SCREEN_WIDTH) / SCREEN_HEIGHT;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

ShaderProgram *program;
Camera *activeCamera;
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
	const float cameraSpeed = 0.05;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		activeCamera->moveForward(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		activeCamera->moveBackward(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		activeCamera->moveLeft(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		activeCamera->moveRight(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		activeCamera->moveUp(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		activeCamera->moveDown(cameraSpeed);
	}
	static bool zhandled = false;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		if (!zhandled) {
			zhandled = true;
			if (activeCamera->isViewLocked())
				activeCamera->unlockView();
			else
				activeCamera->lockView(glm::vec3(0, 0, 0));
		}
	} else {
		zhandled = false;
	}
}

double lastMouseX, lastMouseY;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	float xoffset = xpos - lastMouseX;
	float yoffset = lastMouseY - ypos; // reversed since y-coordinates range from bottom to top
	lastMouseX = xpos;
	lastMouseY = ypos;

	float sensitivity = 0.005f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	activeCamera->turnYaw(xoffset);
	activeCamera->turnPitch(yoffset);
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

	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
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
	glViewport(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
	glfwSetCursorPosCallback(window, mouse_callback);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	ShaderProgram shaderProgram{
		Shader::createVertexShader("GlslShaders/simpleShader.vert"),
		Shader::createFragmentShader("GlslShaders/simpleShader.frag")};
	if (!shaderProgram.isValid())
	{
		return -4;
	}

	ShaderProgram teapotShaderProgram{
		Shader::createVertexShader("GlslShaders/cubeShader.vert"),
		Shader::createFragmentShader("GlslShaders/cubeShader.frag")};
	if (!teapotShaderProgram.isValid())
	{
		return -4;
	}
	program = &teapotShaderProgram;
	teapotShaderProgram.setMatrix("transform", glm::mat4(1.0f));

	unsigned int teapotVao;
	glGenVertexArrays(1, &teapotVao);
	unsigned int teapotVbo;
	glGenBuffers(1, &teapotVbo);
	unsigned int EBO, teapotEbo;
	glGenBuffers(1, &teapotEbo);

#include "cube.h"
	glBindVertexArray(teapotVao);
	glBindBuffer(GL_ARRAY_BUFFER, teapotVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(teapotVertices), teapotVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapotEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapotIndices), teapotIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	teapotShaderProgram.use();
	teapotShaderProgram.setValue("texture0", 0);
	teapotShaderProgram.setValue("texture1", 1);
	teapotShaderProgram.setValue("ratio", 0.2f);

	unsigned int lightVao;
	glGenVertexArrays(1, &lightVao);
	glBindVertexArray(lightVao);
	glBindBuffer(GL_ARRAY_BUFFER, teapotVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapotEbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	ShaderProgram lightShaderProgram {
		Shader::createVertexShader("GlslShaders/cubeShader.vert"),
		Shader::createFragmentShader("GlslShaders/lightShader.frag")};
	if (!lightShaderProgram.isValid())
	{
		return -4;
	}
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::mat4 lightTransform = glm::mat4(1.0f);
	lightTransform = glm::translate(lightTransform, lightPos);
	lightTransform = glm::scale(lightTransform, glm::vec3(0.2f)); 
	lightShaderProgram.setMatrix("transform", lightTransform);

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), PROJECTION_RATIO, 0.1f, 100.0f);

	teapotShaderProgram.setMatrix("projection", projection);
	lightShaderProgram.setMatrix("projection", projection);

	teapotShaderProgram.setVector("lightColor", glm::vec3(1.0f));

	Camera camera;
	activeCamera = &camera;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float ratioValue = (sin(glfwGetTime()) / 2.0f) + 0.5f;
		shaderProgram.setValue("ratio", ratioValue);

		glBindVertexArray(teapotVao);
		teapotShaderProgram.use();
		teapotShaderProgram.setMatrix("view", camera.getViewMat());
		teapotShaderProgram.setVector("viewPos", camera.position);
		glDrawElements(GL_TRIANGLES, sizeof(teapotIndices) / sizeof(unsigned), GL_UNSIGNED_INT, 0);

		glBindVertexArray(lightVao);
		lightShaderProgram.use();
		lightShaderProgram.setMatrix("view", camera.getViewMat());
		glDrawElements(GL_TRIANGLES, sizeof(teapotIndices) / sizeof(unsigned), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}