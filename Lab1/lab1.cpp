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
#include "Graphics/Models/Model.h"

#include "Math/Quaternion.h"
#include "Animate/Keyframe.hpp"
#include "Animate/Interpolate.hpp"

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

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " *model.off*" << std::endl;
	}

	if (!glfwInit())
	{
		std::cerr << "GLFW initialize failed" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Error occurred creating window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "GLAD initialize failed" << std::endl;
		return -1;
	}
	glViewport(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
	glfwSetCursorPosCallback(window, mouse_callback);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Model cube = ModelLoader::loadFile(argv[1]);
	if (!cube.isValid()) {
		std::cerr << "Loading model file '" << argv[1] << "' failed." << std::endl;
		return -1;
	}

	ShaderProgram shaderProgram{
		Shader::createVertexShader("res/shaders/simpleShader.vert"),
		Shader::createFragmentShader("res/shaders/simpleShader.frag")};
	if (!shaderProgram.isValid())
	{
		return -4;
	}
	program = &shaderProgram;
	shaderProgram.setMatrix("transform", glm::mat4(1.0f));

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), PROJECTION_RATIO, 0.1f, 100.0f);

	shaderProgram.setMatrix("projection", projection);

	Camera camera;
	activeCamera = &camera;

	Keyframe<Quaternion> prebegin(100,100,400,Quaternion(0.5, 0.5, 0.5, 0.5));
	Keyframe<Quaternion> begin(1,1,4,Quaternion(1, 0, 0, 0));
	Keyframe<Quaternion> end(-1,-1,4,Quaternion(0.5, 0.5, 0.5, 0.5));
	Keyframe<Quaternion> postend(-100,-100,400,Quaternion(1, 0, 0, 0));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program->setMatrix("transform", 
		// simpleLinearInterpolate(abs(sin(glfwGetTime())), begin, end)
		catmullRomInterpolate(abs(fmod(glfwGetTime(), 2) - 1),prebegin, begin, end, postend)
		.getTranscationMatrix());

		shaderProgram.use();
		shaderProgram.setMatrix("view", camera.getViewMat());
		cube.draw(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}