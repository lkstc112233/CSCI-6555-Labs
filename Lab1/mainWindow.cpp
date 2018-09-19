// standard
#include <cassert>
#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphics/Shaders/Shader.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/Camera/Camera.h"
#include "Interface/KeyHandler.h"
#include "Interface/MouseHandler.h"
#include "Interface/InterfaceRelatedStuffHolder.hpp"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float PROJECTION_RATIO = float(SCREEN_WIDTH) / SCREEN_HEIGHT;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
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
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	MouseHandlerContainer mouseHandlers(mouseX, mouseY);
	MouseCallbackWrapper::handler = &mouseHandlers;
	glfwSetCursorPosCallback(window, &MouseCallbackWrapper::callback);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	ShaderProgram shaderProgram{
		Shader::createVertexShader("res/shaders/simpleShader.vert"),
		Shader::createFragmentShader("res/shaders/simpleShader.frag")};
	if (!shaderProgram.isValid())
	{
		return -4;
	}

	ShaderProgram teapotShaderProgram{
		Shader::createVertexShader("res/shaders/cubeShader.vert"),
		Shader::createFragmentShader("res/shaders/cubeShader.frag")};
	if (!teapotShaderProgram.isValid())
	{
		return -4;
	}
	teapotShaderProgram.setMatrix("transform", glm::mat4(1.0f));

	unsigned int teapotVao;
	glGenVertexArrays(1, &teapotVao);
	unsigned int teapotVbo;
	glGenBuffers(1, &teapotVbo);
	unsigned int EBO, teapotEbo;
	glGenBuffers(1, &teapotEbo);

		float teapotVertices[] = {
		//   X     Y     Z  NX  NY  NZ
		  -0.5, -0.5, -0.5,  0,  0, -1
		, -0.5, -0.5, -0.5,  0, -1,  0
		, -0.5, -0.5, -0.5, -1,  0,  0

		, -0.5, -0.5,  0.5,  0,  0,  1
		, -0.5, -0.5,  0.5,  0, -1,  0
		, -0.5, -0.5,  0.5, -1,  0,  0

		, -0.5,  0.5, -0.5,  0,  0, -1
		, -0.5,  0.5, -0.5,  0,  1,  0
		, -0.5,  0.5, -0.5, -1,  0,  0

		, -0.5,  0.5,  0.5,  0,  0,  1
		, -0.5,  0.5,  0.5,  0,  1,  0
		, -0.5,  0.5,  0.5, -1,  0,  0

		,  0.5, -0.5, -0.5,  0,  0, -1
		,  0.5, -0.5, -0.5,  0, -1,  0
		,  0.5, -0.5, -0.5,  1,  0,  0

		,  0.5, -0.5,  0.5,  0,  0,  1
		,  0.5, -0.5,  0.5,  0, -1,  0
		,  0.5, -0.5,  0.5,  1,  0,  0

		,  0.5,  0.5, -0.5,  0,  0, -1
		,  0.5,  0.5, -0.5,  0,  1,  0
		,  0.5,  0.5, -0.5,  1,  0,  0

		,  0.5,  0.5,  0.5,  0,  0,  1
		,  0.5,  0.5,  0.5,  0,  1,  0
		,  0.5,  0.5,  0.5,  1,  0,  0
	};
	unsigned int teapotIndices[] = {
		2,  5,  8,  8,  5, 11,
	   10,  7, 19, 10, 19, 22,
		9, 21, 15,  9, 15,  3,
		4, 16,  1,  1, 16, 13,
	   18,  6, 12,  6,  0, 12,
	   20, 14, 17, 23, 20, 17
	};
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
		Shader::createVertexShader("res/shaders/cubeShader.vert"),
		Shader::createFragmentShader("res/shaders/lightShader.frag")};
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

	mouseHandlers.emplace_handler([&camera](int mouseFlags, float, float, float diffx, float diffy) {
		//if (mouseFlags & MOUSE_RIGHTBUTTON_HOLD) {
			float sensitivity = 0.005f;
			diffx *= sensitivity;
			diffy *= sensitivity;

			camera.turnYaw(diffx);
			camera.turnPitch(diffy);
		//}
	});

	KeyHandlerContainer keyHandlers(window);

	const float cameraSpeed = 0.05;
	keyHandlers.emplace_handler(GLFW_KEY_ESCAPE, [window]() {
		glfwSetWindowShouldClose(window, true);
	});
	keyHandlers.emplace_handler(GLFW_KEY_SPACE, [&teapotShaderProgram]() {
		glm::mat4 teapotTrans(1.0f);
		static float angle = 0;
		teapotTrans = glm::rotate(teapotTrans, float(angle += M_PI / 60.), glm::normalize(glm::vec3(1.0, 1.0, 1.0)));
		teapotShaderProgram.setMatrix("transform", teapotTrans);
	});
	keyHandlers.emplace_handler(GLFW_KEY_W, [&camera, cameraSpeed]() {
		camera.moveForward(cameraSpeed);
	});
	keyHandlers.emplace_handler(GLFW_KEY_S, [&camera, cameraSpeed]() {
		camera.moveBackward(cameraSpeed);
	});
	keyHandlers.emplace_handler(GLFW_KEY_A, [&camera, cameraSpeed]() {
		camera.moveLeft(cameraSpeed);
	});
	keyHandlers.emplace_handler(GLFW_KEY_D, [&camera, cameraSpeed]() {
		camera.moveRight(cameraSpeed);
	});
	keyHandlers.emplace_handler(GLFW_KEY_R, [&camera, cameraSpeed]() {
		camera.moveUp(cameraSpeed);
	});
	keyHandlers.emplace_handler(GLFW_KEY_F, [&camera, cameraSpeed]() {
		camera.moveDown(cameraSpeed);
	});
	keyHandlers.emplace_handler(GLFW_KEY_Z, [&camera]() {
		if (camera.isViewLocked())
			camera.unlockView();
		else
			camera.lockView(glm::vec3(0, 0, 0));
	}, true);

	while (!glfwWindowShouldClose(window))
	{
		keyHandlers.handle();
		mouseHandlers.handle();

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